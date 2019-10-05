/**
 * \file
 *
 * \brief Timer driver source
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com> and Daniel Igiski Baron <danieligiskibaron@hotmail.com>
 */

#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <intrinsics.h>
#include "watchdog.h"
#include "ADS1248.h"
#include "pid.h"
#include "hal.h"
#include "ADC.h"
#include "timer.h"
#include "I2C.h"
#include "onewire.h"
#include "uart.h"
#include "mppt.h"
#include "energy_level_algorithm.h"
#include "fsp.h"
#include "flash.h"
#include "misc.h"
#include "config.h"

volatile extern uint8_t EPS_data[70];

/**
 * \brief Timer A0.0 interrupt handler
 *
 * 1 second timebase. Reads the solar panels boost converters output voltage, the main bus voltage, the EPS-Beacon current, the MCU internal temperature, the batteries monitor data,
 * the RTDs temperatures, executes the batteries temperature control algorithm.
 *
 * Every 10 seconds it sends data to Beacon.
 *
 * Every 12 hours it resets the MCU.
 *
 * \param -
 *
 * \returns -
 */

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0_a0_isr(void){

    __enable_interrupt();

    volatile static uint8_t counter_1s = 0;         // increments every 100 milliseconds; resets every 1 second
    volatile static uint8_t counter_10s = 0;        // increments every second; resets every 10 seconds
    volatile static uint16_t counter_10min = 0;     // increments every 100 milliseconds when in battery charge reset mode; resets every 10 minutes
    volatile static uint32_t counter_12h = 0;       // increments every 100 milliseconds; resets every 12 hours
    volatile static uint8_t flash_counter = 0;

    static struct Pid parameters_heater1 = {0, 0, 1, 150, 20, 0 , INT_MAX, 10};
    static struct Pid parameters_heater2 = {0, 0, 1, 150, 20, 0 , INT_MAX, 10};

    volatile uint16_t adc6 = 0;
    volatile uint16_t adc7 = 0;
    volatile uint16_t adc10 = 0;
    volatile uint16_t adc15 = 0;
    volatile uint16_t msp_ts = 0;
    volatile uint32_t rtd0_measure = 0, rtd1_measure = 0, rtd2_measure = 0, rtd3_measure = 0;
    volatile uint32_t rtd4_measure = 0, rtd5_measure = 0, rtd6_measure = 0;
    volatile uint32_t battery1_temp = 0, battery2_temp = 0;
    volatile float heater1_temp = 0, heater2_temp = 0;
    static uint8_t heater1_duty_cycle = 0, heater2_duty_cycle = 0;

#if FIRST_CHARGE_RESET_ROUTINE == 1
    if(flash_read_single(FIRST_CHARGE_RESET_ADDR_FLASH) == FIRST_CHARGE_RESET_ACTIVE){
        if(counter_10min++ >= COUNTER_EQUIVALENT_TO_10MIN){
            counter_10min = 0;

            flash_counter = flash_read_long(FLASH_COUNTER_ADDR_FLASH);                          // increment counter stored on the flash memory
            flash_counter++;
            flash_erase(FLASH_COUNTER_ADDR_FLASH);
            flash_write_long(flash_counter, FLASH_COUNTER_ADDR_FLASH);

            if(flash_read_long(FLASH_COUNTER_ADDR_FLASH) >= COUNTER_EQUIVALENT_TO_1H){
                flash_erase(RESET_BATTERY_CHARGE_ADDR_FLASH);
                flash_write_single(RESET_BATTERY_CHARGE_COMMAND, RESET_BATTERY_CHARGE_ADDR_FLASH);

                flash_erase(FLASH_COUNTER_ADDR_FLASH);
                flash_write_long(0x00, FLASH_COUNTER_ADDR_FLASH);

                flash_erase(FIRST_CHARGE_RESET_ADDR_FLASH);
                flash_write_long(FIRST_CHARGE_RESET_DONE, FIRST_CHARGE_RESET_ADDR_FLASH);
            }
        }
    }    
#endif //FIRST_CHARGE_RESET_ROUTINE

    if(flash_read_single(RESET_BATTERY_CHARGE_ADDR_FLASH) == RESET_BATTERY_CHARGE_COMMAND){         // enter if the reset battery charge mode flag is active

        if(counter_10min++ >= COUNTER_EQUIVALENT_TO_10MIN){                                         // enter every 10 minutes
            counter_10min = 0;                                                                      // reset the counter

            flash_counter = flash_read_long(FLASH_COUNTER_ADDR_FLASH);                          // increment counter stored on the flash memory
            flash_counter++;
            flash_erase(FLASH_COUNTER_ADDR_FLASH);
            flash_write_long(flash_counter, FLASH_COUNTER_ADDR_FLASH);

            if(flash_read_long(FLASH_COUNTER_ADDR_FLASH) >= COUNTER_EQUIVALENT_TO_1D){              // enter if 1 day is passed
                write_accumulated_current_max_value();                                              // record the battery charge maximum value in batteries monitor

                flash_erase(RESET_BATTERY_CHARGE_ADDR_FLASH);                                       // turn off the reset battery charge mode flag

                flash_erase(FLASH_COUNTER_ADDR_FLASH);                                              // reset the 10-minute counter stored on the flash memory
                flash_write_long(0x00, FLASH_COUNTER_ADDR_FLASH);
            }
        }
    }


    if(counter_12h++ >= COUNTER_EQUIVALENT_TO_12H){         // enter every 12 hours to reset the MCU
        counter_12h = 0;                                    // reset the counter
        WDTCTL = 0xDEAD;                                    // reset the MCU
    }


    if(counter_1s++ >= COUNTER_EQUIVALENT_TO_1S){               // enter every 1 second to normal operation
        counter_1s = 0;

#if defined(_DEBUG) || defined(_VERBOSE)
        timer_debug_port_1s ^= timer_debug_pin_1s;      // Toggle 1s debug pin
#endif

        adc15 = adc_read(vpanels_voltage);              // Vpanels voltage measurement
        EPS_data[vpanels_voltage_LSB] = (uint8_t) (adc15 & 0xff);       // bitwise and with 0xff to get LSB
        EPS_data[vpanels_voltage_MSB] = (uint8_t) (adc15 >> 8);         // shift data 8 bits to get MSB

#ifdef _VERBOSE_DEBUG
        uart_tx_debug("**** Misc ADC ****");
        uart_tx_debug("\r\n");
        uart_tx_debug("Vpanels Voltage: ");
        float_send(adc15*0.00244140625);
        uart_tx_debug("\r\n");
#elif defined(_DEBUG)
        float_send(adc15*0.00244140625);
        uart_tx_debug(",");
#endif

        watchdog_reset_counter();

        adc7 = adc_read(bus_voltage);               // bus voltage measurement
        EPS_data[bus_voltage_LSB] = (uint8_t) (adc7 & 0xff);        // bitwise and with 0xff to get LSB
        EPS_data[bus_voltage_MSB] = (uint8_t) (adc7 >> 8);      // shift data 8 bits to get MSB

#ifdef _VERBOSE_DEBUG
        uart_tx_debug("Vbus Voltage: ");
        float_send(adc7*0.00244140625);
        uart_tx_debug("\r\n");
#elif defined(_DEBUG)
        float_send(adc7*0.00244140625);
        uart_tx_debug(",");
#endif

        watchdog_reset_counter();

        adc6 = adc_read(beacon_eps_current);        // beacon/eps current measurement
        EPS_data[beacon_eps_current_LSB] = (uint8_t) (adc6 & 0xff);     // bitwise and with 0xff to get LSB
        EPS_data[beacon_eps_current_MSB] = (uint8_t) (adc6 >> 8);       // shift data 8 bits to get MSB

        watchdog_reset_counter();

#ifdef _VERBOSE_DEBUG
        uart_tx_debug("Beacon/EPS Current: ");
        float_send(adc6*0.0001229927582);
        uart_tx_debug("\r\n");
#elif defined(_DEBUG)
        float_send(adc6*0.0001229927582);
        uart_tx_debug(",");
#endif

        adc10 = adc_read(msp_temperature);
        EPS_data[msp_temperature_LSB] = (uint8_t) (adc10 & 0xff);   // bitwise and with 0xff to get LSB
        EPS_data[msp_temperature_MSB] = (uint8_t) (adc10 >> 8);     // shift data 8 bits to get MSB

        watchdog_reset_counter();

        EPS_data[battery_average_current_LSB] = DS2775_read_register(average_current_LSB_register);     // read battery average current LSB
        EPS_data[battery_average_current_MSB] = DS2775_read_register(average_current_MSB_register);     // read battery average current MSB

        watchdog_reset_counter();

        EPS_data[battery_monitor_temeperature_LSB] = DS2775_read_register(temperature_LSB_register);        // read battery temperature LSB
        EPS_data[battery_monitor_temeperature_MSB] = DS2775_read_register(temperature_MSB_register);        // read battery temperature MSB

        watchdog_reset_counter();

        EPS_data[battery1_voltage_LSB] = DS2775_read_register(voltage_LSB1_register);       // read battery 1 voltage LSB
        EPS_data[battery1_voltage_MSB] = DS2775_read_register(voltage_MSB1_register);       // read battery 1 voltage MSB

        watchdog_reset_counter();

        EPS_data[battery2_voltage_LSB] = DS2775_read_register(voltage_LSB2_register);       // read battery 2 voltage LSB
        EPS_data[battery2_voltage_MSB] = DS2775_read_register(voltage_MSB2_register);       // read battery 2 voltage MSB

        watchdog_reset_counter();

        EPS_data[battery_current_LSB] = DS2775_read_register(current_LSB_register);     // read battery current LSB
        EPS_data[battery_current_MSB] = DS2775_read_register(current_MSB_register);     // read battery current MSB

        watchdog_reset_counter();

        if(flash_read_single(RESET_BATTERY_CHARGE_ADDR_FLASH) == RESET_BATTERY_CHARGE_COMMAND){        // enter if the reset battery charge mode flag is active
            EPS_data[battery_accumulated_current_LSB] = 0x00;               // set battery accumulated current LSB to zero
            EPS_data[battery_accumulated_current_MSB] = 0x00;               // set battery accumulated current MSB to zero

        }
        else{
            EPS_data[battery_accumulated_current_LSB] = DS2775_read_register(accumulated_current_LSB_register);     // read battery current LSB
            EPS_data[battery_accumulated_current_MSB] = DS2775_read_register(accumulated_current_MSB_register);     // read battery current MSB
        }

        watchdog_reset_counter();

        EPS_data[protection_register_LSB] = DS2775_read_register(protection_register);      // read protection register

        /** Measurement of RTDs' temperatures (RTD0 to RTD6). **/
        rtd0_measure = read_ADS1248(0);

        EPS_data[RTD0_B3] = rtd0_measure & 0xff;
        EPS_data[RTD0_B2] = (rtd0_measure >> 8) & 0xff;
        EPS_data[RTD0_B1] = (rtd0_measure >> 16) & 0xff;

        rtd1_measure = read_ADS1248(1);

        EPS_data[RTD1_B3] = rtd1_measure & 0xff;
        EPS_data[RTD1_B2] = (rtd1_measure >> 8) & 0xff;
        EPS_data[RTD1_B1] = (rtd1_measure >> 16) & 0xff;

        rtd2_measure = read_ADS1248(2);

        EPS_data[RTD2_B3] = rtd2_measure & 0xff;
        EPS_data[RTD2_B2] = (rtd2_measure >> 8) & 0xff;
        EPS_data[RTD2_B1] = (rtd2_measure >> 16) & 0xff;

        rtd3_measure = read_ADS1248(3);

        EPS_data[RTD3_B3] = rtd3_measure & 0xff;
        EPS_data[RTD3_B2] = (rtd3_measure >> 8) & 0xff;
        EPS_data[RTD3_B1] = (rtd3_measure >> 16) & 0xff;

        rtd4_measure = read_ADS1248(4);

        EPS_data[RTD4_B3] = rtd4_measure & 0xff;
        EPS_data[RTD4_B2] = (rtd4_measure >> 8) & 0xff;
        EPS_data[RTD4_B1] = (rtd4_measure >> 16) & 0xff;

        rtd5_measure = read_ADS1248(5);

        EPS_data[RTD5_B3] = rtd5_measure & 0xff;
        EPS_data[RTD5_B2] = (rtd5_measure >> 8) & 0xff;
        EPS_data[RTD5_B1] = (rtd5_measure >> 16) & 0xff;

        rtd6_measure = read_ADS1248(6);

        EPS_data[RTD6_B3] = rtd6_measure & 0xff;
        EPS_data[RTD6_B2] = (rtd6_measure >> 8) & 0xff;
        EPS_data[RTD6_B1] = (rtd6_measure >> 16) & 0xff;

        //Heater management
    #if BATTERY_MONITOR_AS_HEATER_REFERENCE == 1
        battery1_temp = ((uint16_t)EPS_data[battery_monitor_temeperature_MSB] << 8) | EPS_data[battery_monitor_temeperature_LSB];   //Temperature of battery 1 is given by the battery monitor
        battery2_temp = battery1_temp;
    #else
        battery1_temp = rtd_value_verification(rtd5_measure, rtd6_measure);       //Temperature of battery 1 is given by RTDs 5 and 6
        battery2_temp = rtd_value_verification(rtd2_measure, rtd2_measure);       //Temperature of battery 2 is given by RTDs 2 and 3
    #endif // BATTERY_MONITOR_AS_HEATER_REFERENCE

    #if BATTERY_MONITOR_AS_HEATER_REFERENCE == 0
        //Use the other battery in case of malfunction
        if(battery1_temp != 0x007FFFFF){
            heater1_temp = battery1_temp;          
        }
        else{
            heater1_temp = battery2_temp;
        }

        //Use the other battery in case of malfunction
        if(battery2_temp != 0x007FFFFF){
            heater2_temp = battery2_temp;          
        }
        else{
            heater2_temp = battery1_temp;
        }
    #endif // BATTERY_MONITOR_AS_HEATER_REFERENCE

    #if BATTERY_MONITOR_AS_HEATER_REFERENCE == 1
        heater1_temp = (int16_t)(battery1_temp) * 0.125 / 32.0;
        heater2_temp = heater1_temp;
    #else
        heater1_temp = (heater1_temp*0.000196695 - 1000)/3.85;      // Converting temperature values to Celsius (according to ADC parameters)
        heater2_temp = (heater2_temp*0.000196695 - 1000)/3.85;
    #endif // BATTERY_MONITOR_AS_HEATER_REFERENCE

    #if BATTERY_MONITOR_AS_HEATER_REFERENCE == 1
        if (heater1_temp <= HEATER_TEMPERATURE_SETPOINT)
        {
            heater1_duty_cycle = 160*BATTERY_MONITOR_AS_HEATER_REF_DUTY_CYCLE;
            heater2_duty_cycle = 160*BATTERY_MONITOR_AS_HEATER_REF_DUTY_CYCLE;
        }
        else
        {
            heater1_duty_cycle = 0;
            heater2_duty_cycle = 0;
        }
    #else
        heater1_duty_cycle = Pid_Control(HEATER_TEMPERATURE_SETPOINT, heater1_temp, &parameters_heater1) * 160;
        heater2_duty_cycle = Pid_Control(HEATER_TEMPERATURE_SETPOINT, heater2_temp, &parameters_heater2) * 160;
    #endif // BATTERY_MONITOR_AS_HEATER_REFERENCE

    #if HEATER_ENABLED == 1
        TA1CCR2 = heater1_duty_cycle;
        TA1CCR1 = heater2_duty_cycle;
    #else
        TA1CCR2 = 0;    // Duty cycle = 0
        TA1CCR1 = 0;    // Duty cycle = 0
    #endif // HEATER_ENABLED

#ifdef _VERBOSE_DEBUG
        uart_tx_debug("**** ADS1248 Mesurements ****\r\n");
        uart_tx_debug("channel 2: ");
        float_send((temp_2*0.000196695 - 1000)/(3.85));
        uart_tx_debug("\r\n");
        uart_tx_debug("channel 6: ");
        float_send((temp_6*0.000196695 - 1000)/(3.85));
        uart_tx_debug("\r\n");
#elif defined(_DEBUG)
        //float_send((temp_2*0.000196695 - 1000)/(3.85));
        uart_tx_debug(",");
        //float_send((temp_6*0.000196695 - 1000)/(3.85));
        uart_tx_debug(",");
#endif

        watchdog_reset_counter();

        EPS_data[eps_status] = energyLevelAlgorithm(EPS_data[eps_status], (EPS_data[battery_accumulated_current_LSB] | (EPS_data[battery_accumulated_current_MSB] << 8))*0.625);

        watchdog_reset_counter();

#ifdef _VERBOSE_DEBUG
        uint8_t string[5];
        sprintf(string, "%#04x", EPS_data[eps_status]);
        uart_tx_debug("Energy Level: ");
        uart_tx_debug(string);
        uart_tx_debug("\r\n");
#elif defined(_DEBUG)
        uint8_t string[5];
        sprintf(string, "%#04x", EPS_data[eps_status]);
        uart_tx_debug(string);
        uart_tx_debug(",");
#endif

        if(counter_10s++ >= COUNTER_EQUIVALENT_TO_10S){     // enter every 10 seconds to send data to beacon
            counter_10s = 0;

            static fsp_packet_t beacon_packet_fsp_struct;
            volatile uint8_t beacon_packet_fsp_array[38] = {0};
            volatile uint8_t beacon_packet[31] = {0};

            beacon_packet[0] = EPS_data[battery1_voltage_MSB];
            beacon_packet[1] = EPS_data[battery1_voltage_LSB];
            beacon_packet[2] = EPS_data[battery2_voltage_MSB];
            beacon_packet[3] = EPS_data[battery2_voltage_LSB];
        #if BATTERY_MONITOR_AS_HEATER_REFERENCE == 1
            beacon_packet[4] = EPS_data[battery_monitor_temeperature_MSB];
            beacon_packet[5] = EPS_data[battery_monitor_temeperature_LSB];
            beacon_packet[6] = EPS_data[battery_current_MSB];
            beacon_packet[7] = EPS_data[battery_current_LSB];
            beacon_packet[8] = EPS_data[msp_temperature_MSB];
            beacon_packet[9] = EPS_data[msp_temperature_LSB];
        #else
            beacon_packet[4] = EPS_data[RTD2_B1];
            beacon_packet[5] = EPS_data[RTD2_B2];
            beacon_packet[6] = EPS_data[RTD2_B3];
            beacon_packet[7] = EPS_data[RTD3_B1];
            beacon_packet[8] = EPS_data[RTD3_B2];
            beacon_packet[9] = EPS_data[RTD3_B3];
        #endif // BATTERY_MONITOR_AS_HEATER_REFERENCE
            beacon_packet[10] = EPS_data[battery_accumulated_current_MSB];
            beacon_packet[11] = EPS_data[battery_accumulated_current_LSB];
            beacon_packet[12] = EPS_data[negative_y_panel_current_MSB];
            beacon_packet[13] = EPS_data[negative_y_panel_current_LSB];
            beacon_packet[14] = EPS_data[positive_x_panel_current_MSB];
            beacon_packet[15] = EPS_data[positive_x_panel_current_LSB];
            beacon_packet[16] = EPS_data[negative_x_panel_current_MSB];
            beacon_packet[17] = EPS_data[negative_x_panel_current_LSB];
            beacon_packet[18] = EPS_data[positive_z_panel_current_MSB];
            beacon_packet[19] = EPS_data[positive_z_panel_current_LSB];
            beacon_packet[20] = EPS_data[negative_z_panel_current_MSB];
            beacon_packet[21] = EPS_data[negative_z_panel_current_LSB];
            beacon_packet[22] = EPS_data[positive_y_panel_current_MSB];
            beacon_packet[23] = EPS_data[positive_y_panel_current_LSB];
            beacon_packet[24] = EPS_data[negative_y_positive_x_panel_voltage_MSB];
            beacon_packet[25] = EPS_data[negative_y_positive_x_panel_voltage_LSB];
            beacon_packet[26] = EPS_data[negative_x_positive_z_panel_voltage_MSB];
            beacon_packet[27] = EPS_data[negative_x_positive_z_panel_voltage_LSB];
            beacon_packet[28] = EPS_data[negative_z_positive_y_panel_voltage_MSB];
            beacon_packet[29] = EPS_data[negative_z_positive_y_panel_voltage_LSB];
            beacon_packet[30] = EPS_data[eps_status];
            fsp_gen_data_pkt(beacon_packet, sizeof(beacon_packet), FSP_ADR_TTC, FSP_PKT_WITHOUT_ACK, &beacon_packet_fsp_struct);

            uint8_t packet_length;
            fsp_encode(&beacon_packet_fsp_struct, beacon_packet_fsp_array, &packet_length);

            uint8_t i = 0;

            for(i = 0; i < packet_length; i++){
                uart_tx_beacon(beacon_packet_fsp_array[i]);
                __delay_cycles(clock*0.001);
            }
        }


#ifdef _VERBOSE_DEBUG
        uint8_t protection_register_string[30] = {0};

        watchdog_reset_counter();
        uart_tx_debug("**** DS2775 Measurements: ****\r\n");
        uart_tx_debug("Battery I voltage: ");
        float_send(voltage_unit*(((EPS_data[battery1_voltage_MSB] >> 5) << 8) | (EPS_data[battery1_voltage_LSB] >> 5) | ((EPS_data[battery1_voltage_MSB] << 3) & 0xf8)));
        uart_tx_debug("\r\n");
        uart_tx_debug("Battery II voltage: ");
        float_send(voltage_unit*(((EPS_data[battery2_voltage_MSB] >> 5) << 8) | (EPS_data[battery2_voltage_LSB] >> 5) | ((EPS_data[battery2_voltage_MSB] << 3) & 0xf8)));
        uart_tx_debug("\r\n");
        uart_tx_debug("Battery Current: ");
        float_send(current_unit*((EPS_data[battery_current_MSB] << 8) + EPS_data[battery_current_LSB]));
        uart_tx_debug("\r\n");
        uart_tx_debug("Battery Average Current: ");
        float_send(current_unit*((EPS_data[battery_average_current_MSB] << 8) + EPS_data[battery_average_current_LSB]));
        uart_tx_debug("\r\n");
        uart_tx_debug("Battery Accumulated Current: ");
        float_send((accumulated_current_unit)*((EPS_data[battery_accumulated_current_MSB] << 8) + EPS_data[battery_accumulated_current_LSB]));
        uart_tx_debug("\r\n");
        sprintf(protection_register_string, "%#04x", EPS_data[protection_register_LSB] & 0x0f);
        uart_tx_debug("Protection Register: ");
        uart_tx_debug(protection_register_string);
        uart_tx_debug("\r\n\n");
#elif defined(_DEBUG)
        uint8_t protection_register_string[30] = {0};

        float_send(voltage_unit*(((EPS_data[battery1_voltage_MSB] >> 5) << 8) | (EPS_data[battery1_voltage_LSB] >> 5) | ((EPS_data[battery1_voltage_MSB] << 3) & 0xf8)));
        uart_tx_debug(",");
        float_send(voltage_unit*(((EPS_data[battery2_voltage_MSB] >> 5) << 8) | (EPS_data[battery2_voltage_LSB] >> 5) | ((EPS_data[battery2_voltage_MSB] << 3) & 0xf8)));
        uart_tx_debug(",");
        float_send(current_unit*((EPS_data[battery_current_MSB] << 8) + EPS_data[battery_current_LSB]));
        uart_tx_debug(",");
        float_send(current_unit*((EPS_data[battery_average_current_MSB] << 8) + EPS_data[battery_average_current_LSB]));
        uart_tx_debug(",");
        float_send((accumulated_current_unit)*((EPS_data[battery_accumulated_current_MSB] << 8) + EPS_data[battery_accumulated_current_LSB]));
        uart_tx_debug(",");
        sprintf(protection_register_string, "%#04x", EPS_data[protection_register_LSB] & 0x0f);
        uart_tx_debug(protection_register_string);
        uart_tx_debug("\r\n");
#endif

    }
}

/**
 * \brief Timer A1.1 interrupt handler.
 *
 * 100 milliseconds timebase. Reads the solar panels voltages and currents 10 times and takes the mean. Then uses this data to execute the MPPT algorithm.
 *
 * \param -
 *
 * \returns -
 */

#pragma vector=TIMER0_A1_VECTOR
__interrupt void timer0_a1_isr(void){

    TA0CCTL1 &= ~CCIFG;
    __enable_interrupt();
    TA0CCTL0 &= ~CCIE;
    interruption_occurring = 1;


#ifdef _DEBUG
    timer_debug_port_100ms ^= timer_debug_pin_100ms;    // Toggle 100ms debug pin
#endif

    volatile uint16_t adc0 = 0;
    volatile uint16_t adc1 = 0;
    volatile uint16_t adc2 = 0;
    volatile uint16_t adc3 = 0;
    volatile uint16_t adc4 = 0;
    volatile uint16_t adc5 = 0;
    volatile uint16_t adc12 = 0;
    volatile uint16_t adc13 = 0;
    volatile uint16_t adc14 = 0;

    static volatile uint8_t mean_counter = 0;
    static volatile uint16_t negative_y_panel_current_mean = 0;             // take mean of adc0
    static volatile uint16_t positive_x_panel_current_mean = 0;             // take mean of adc1
    static volatile uint16_t negative_x_panel_current_mean = 0;             // take mean of adc2
    static volatile uint16_t positive_z_panel_current_mean = 0;             // take mean of adc3
    static volatile uint16_t negative_z_panel_current_mean = 0;             // take mean of adc4
    static volatile uint16_t positive_y_panel_current_mean = 0;             // take mean of adc5
    static volatile uint16_t negative_y_positive_x_panel_voltage_mean = 0;      // take mean of adc12
    static volatile uint16_t negative_x_positive_z_panel_voltage_mean = 0;      // take mean of adc13
    static volatile uint16_t negative_z_positive_y_panel_voltage_mean = 0;      // take mean of adc14

    static mppt_parameters_t panel12_parameters = {0,0,1};
    static mppt_parameters_t panel34_parameters = {0,0,1};
    static mppt_parameters_t panel56_parameters = {0,0,1};


    watchdog_reset_counter();

    if(mean_counter == 0){

        negative_y_panel_current_mean = 0;                  // reset adc0
        positive_x_panel_current_mean = 0;                  // reset adc1
        negative_x_panel_current_mean = 0;                  // reset adc2
        positive_z_panel_current_mean = 0;                  // reset adc3
        negative_z_panel_current_mean = 0;                  // reset adc4
        positive_y_panel_current_mean = 0;                  // reset adc5
        negative_y_positive_x_panel_voltage_mean = 0;       // reset adc12
        negative_x_positive_z_panel_voltage_mean = 0;       // reset adc13
        negative_z_positive_y_panel_voltage_mean = 0;       // reset adc14
    }

    adc0 = adc_read(negative_y_panel_current);      // -Y panel current measurement
    EPS_data[negative_y_panel_current_LSB] = (uint8_t) (adc0 & 0xff);           // bitwise and with 0xff to get LSB
    EPS_data[negative_y_panel_current_MSB] = (uint8_t) (adc0 >> 8);         // shift data 8 bits to get MSB
    negative_y_panel_current_mean += adc0;          // add measurement value to mean calculation

    watchdog_reset_counter();

    adc1 = adc_read(positive_x_panel_current);      // +X panel current measurement
    EPS_data[positive_x_panel_current_LSB] = (uint8_t) (adc1 & 0xff);           // bitwise and with 0xff to get LSB
    EPS_data[positive_x_panel_current_MSB] = (uint8_t) (adc1 >> 8);         // shift data 8 bits to get MSB
    positive_x_panel_current_mean += adc1;          // add measurement value to mean calculation

    watchdog_reset_counter();

    adc2 = adc_read(negative_x_panel_current);      // -X panel current measurement
    EPS_data[negative_x_panel_current_LSB] = (uint8_t) (adc2 & 0xff);           // bitwise and with 0xff to get LSB
    EPS_data[negative_x_panel_current_MSB] = (uint8_t) (adc2 >> 8);         // shift data 8 bits to get MSB
    negative_x_panel_current_mean += adc2;          // add measurement value to mean calculation

    watchdog_reset_counter();

    adc3 = adc_read(positive_z_panel_current);      // +Z panel current measurement
    EPS_data[positive_z_panel_current_LSB] = (uint8_t) (adc3 & 0xff);           // bitwise and with 0xff to get LSB
    EPS_data[positive_z_panel_current_MSB] = (uint8_t) (adc3 >> 8);         // shift data 8 bits to get MSB
    positive_z_panel_current_mean += adc3;          // add measurement value to mean calculation

    watchdog_reset_counter();

    adc4 = adc_read(negative_z_panel_current);      // -Z panel current measurement
    EPS_data[negative_z_panel_current_LSB] = (uint8_t) (adc4 & 0xff);           // bitwise and with 0xff to get LSB
    EPS_data[negative_z_panel_current_MSB] = (uint8_t) (adc4 >> 8);         // shift data 8 bits to get MSB
    negative_z_panel_current_mean += adc4;          // add measurement value to mean calculation

    watchdog_reset_counter();

    adc5 = adc_read(positive_y_panel_current);      // +Y panel current measurement
    EPS_data[positive_y_panel_current_LSB] = (uint8_t) (adc5 & 0xff);           // bitwise and with 0xff to get LSB
    EPS_data[positive_y_panel_current_MSB] = (uint8_t) (adc5 >> 8);         // shift data 8 bits to get MSB
    positive_y_panel_current_mean += adc5;          // add measurement value to mean calculation

    watchdog_reset_counter();

    adc12 = adc_read(negative_y_positive_x_panel_voltage);      // -Y and +X panels voltage
    EPS_data[negative_y_positive_x_panel_voltage_LSB] = (uint8_t) (adc12 & 0xff);                   // bitwise and with 0xff to get LSB
    EPS_data[negative_y_positive_x_panel_voltage_MSB] = (uint8_t) (adc12 >> 8);                     // shift data 8 bits to get MSB
    negative_y_positive_x_panel_voltage_mean += adc12;          // add measurement value to mean calculation

    watchdog_reset_counter();

    adc13 = adc_read(negative_x_positive_z_panel_voltage);      // -X and +Z panels voltage
    EPS_data[negative_x_positive_z_panel_voltage_LSB] = (uint8_t) (adc13 & 0xff);                   // bitwise and with 0xff to get LSB
    EPS_data[negative_x_positive_z_panel_voltage_MSB] = (uint8_t) (adc13 >> 8);                     // shift data 8 bits to get MSB
    negative_x_positive_z_panel_voltage_mean += adc13;          // add measurement value to mean calculation

    watchdog_reset_counter();

    adc14 = adc_read(negative_z_positive_y_panel_voltage);      // -Z and +Y panels voltage
    EPS_data[negative_z_positive_y_panel_voltage_LSB] = (uint8_t) (adc14 & 0xff);                   // bitwise and with 0xff to get LSB
    EPS_data[negative_z_positive_y_panel_voltage_MSB] = (uint8_t) (adc14 >> 8);                     // shift data 8 bits to get MSB
    negative_z_positive_y_panel_voltage_mean += adc14;          // add measurement value to mean calculation

    watchdog_reset_counter();

    if(mean_counter == 9){

        mean_counter = 0;       // reset mean counter

        negative_y_panel_current_mean /= 10;                // take mean of adc0
        positive_x_panel_current_mean /= 10;                // take mean of adc1
        negative_x_panel_current_mean /= 10;                // take mean of adc2
        positive_z_panel_current_mean /= 10;                // take mean of adc3
        negative_z_panel_current_mean /= 10;                // take mean of adc4
        positive_y_panel_current_mean /= 10;                // take mean of adc5
        negative_y_positive_x_panel_voltage_mean /= 10;     // take mean of adc12
        negative_x_positive_z_panel_voltage_mean /= 10;     // take mean of adc13
        negative_z_positive_y_panel_voltage_mean /= 10;     // take mean of adc14

#ifdef _VERBOSE_DEBUG
        uart_tx_debug("**** Solar Panel Currents ****");
        uart_tx_debug("\r\n");
        uart_tx_debug("-Y panel current: ");
        float_send(negative_y_panel_current_mean*.0001479640152);
        uart_tx_debug("\r\n");
        uart_tx_debug("+X panel current: ");
        float_send(positive_x_panel_current_mean*.0001479640152);
        uart_tx_debug("\r\n");
        uart_tx_debug("-X panel current: ");
        float_send(negative_x_panel_current_mean*.0001479640152);
        uart_tx_debug("\r\n");
        uart_tx_debug("+Z panel current: ");
        float_send(positive_z_panel_current_mean*.0001479640152);
        uart_tx_debug("\r\n");
        uart_tx_debug("-Z panel current: ");
        float_send(negative_z_panel_current_mean*.0001479640152);
        uart_tx_debug("\r\n");
        uart_tx_debug("+Y panel current: ");
        float_send(positive_y_panel_current_mean*.0001479640152);
        uart_tx_debug("\r\n\n");

        uart_tx_debug("**** Solar Panel Voltages ****");
        uart_tx_debug("\r\n");
        uart_tx_debug("-Y/+X panel voltage: ");
        float_send(negative_y_positive_x_panel_voltage_mean*0.001178588867);
        uart_tx_debug("\r\n");
        uart_tx_debug("-X/+Z panel voltage: ");
        float_send(negative_x_positive_z_panel_voltage_mean*0.001178588867);
        uart_tx_debug("\r\n");
        uart_tx_debug("-Z/+Y panel voltage: ");
        float_send(negative_z_positive_y_panel_voltage_mean*0.001178588867);
        uart_tx_debug("\r\n");
#elif defined(_DEBUG)
        float_send(negative_y_panel_current_mean*.0001479640152);
        uart_tx_debug(",");
        float_send(positive_x_panel_current_mean*.0001479640152);
        uart_tx_debug(",");
        float_send(negative_x_panel_current_mean*.0001479640152);
        uart_tx_debug(",");
        float_send(positive_z_panel_current_mean*.0001479640152);
        uart_tx_debug(",");
        float_send(negative_z_panel_current_mean*.0001479640152);
        uart_tx_debug(",");
        float_send(positive_y_panel_current_mean*.0001479640152);
        uart_tx_debug(",");
        float_send(negative_y_positive_x_panel_voltage_mean*0.001178588867);
        uart_tx_debug(",");
        float_send(negative_x_positive_z_panel_voltage_mean*0.001178588867);
        uart_tx_debug(",");
        float_send(negative_z_positive_y_panel_voltage_mean*0.001178588867);
        uart_tx_debug(",");
#endif

        mppt_algorithm((negative_y_panel_current_mean + positive_x_panel_current_mean), negative_y_positive_x_panel_voltage_mean, 0x03D4, &panel12_parameters);
        mppt_algorithm((negative_x_panel_current_mean + positive_z_panel_current_mean), negative_x_positive_z_panel_voltage_mean, 0x03D6, &panel34_parameters);
        mppt_algorithm((negative_z_panel_current_mean + positive_y_panel_current_mean), negative_z_positive_y_panel_voltage_mean, 0x03D8, &panel56_parameters);
    }
    else{
        mean_counter++;
    }
    TA0CCTL0 = CCIE;
}

/**
 * \brief Configures the timer peripheral
 *
 * <b> Time Bases Configuration </b> <br>
 * Timers A0.1 and A0.2 are used to generate the time bases. <br>
 * Sets the debug pins as output, then configures the TA0CCR0 and TA0CCR1 registers as 50000, generating interrupts every 100 ms. Then enables the interrupts in the TA0CCTL0 and
 * TA0CCTL1 registers. Then it configures the TA0CTL register to select SMCLK (32 MHz) as the clock source, counting in up mode and with a clock divider of 8. Then it configures
 * the TA0EX0 register to again divide the clock by 8. Then it clears the counter by setting the TACLR bit in the TA0CTL register. <br>
 * <br>
 *
 * <b> Timeout Timer Configuration </b> <br>
 * Timer A2 is used to generate timeouts. <br>
 * Configures the TA2CTL register to use ACLK (16 MHz) as clock source, counting in up mode and sets the clock divider to 8. Then configures the TA2EX0 register to again divide
 * the clock by 8. Then clears the counter by setting the TACLR bit in the TA2CTL register. <br>
 * <br>
 *
 * <b> PWM Configuration </b> <br>
 * Timers A1 and B0 are used to generate PWM signals. <br>
 * Configures TBCTL to use SMCLK as clock source (32 MHz) and count in up mode. Then clears the counter by setting the TBCLR bit in the TBCTL register. The it configures port 4
 * pins 1, 2 and 3 as PWM function. It configures the TBCCR0 register as 70, resulting in a PWM frequency of ~457 kHz. Then it configures the TBCCTL1, TBCCTL2 and TBCCTL3 registers
 * in reset/set output mode. The TBCCR1, TBCCR2 and TBCCR3 are initially configured in an arbitrary value. <br>
 * The second PWM frequency is configured by configuring TA1CTL to use SMCLK as the clock source, counting in up mode and dividing the clock by 4. Then the counter is cleared
 * by setting the TACLR in the TA1CTL register. Port 3 pins 2 and 3 functions are selected as PWM. Then TA1CCR0 is configured as 160, resulting in a PWM frequency of 50 kHz. Then
 * TA1CCTL1 and TA1CCTL2 are configured in reset/set output mode. Finally TA1CCR1 and TA1CCR2 are initially configured in arbitrary values.
 *
 * \param -
 *
 * \returns -
 */

void timer_config(void){

    timer_debug_port_dir |= timer_debug_pin_1s;     // P1.0 output
    timer_debug_port_dir |= timer_debug_pin_100ms;  // P1.1 output
    TA0CCR0 = 50000;                                // timer A0 CCR0 interrupt period = 50000*10* 1/500000 = 1s
    TA0CCR1 = 50000;
    TA0CCTL0 = CCIE;                                // timer A0 CCR0 interrupt enabled
    TA0CCTL1 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID__8;               // SMCLK, upmode, timer A interrupt enable, divide clock by 8
    TA0EX0 = TAIDEX_7;                              // divide clock by 8
    TA0CTL |= TACLR;                                // clear TAR

    TA2CTL = TASSEL_1 + MC_1 + ID__8;               // ACLK, up to CCR0, divide clock by 8
    TA2EX0 = TAIDEX_7;                              // divide clock by 8
    TA2CTL |= TACLR;                                // clear TAR


    TBCTL |= TBSSEL_2 + MC_1;
    TBCTL |= TBCLR;

    P4SEL |= BIT1 | BIT2 | BIT3;   // P4.1, P4.2 and P4.3 option select
    P4DIR |= BIT1 | BIT2 | BIT3;   // P4.1, P4.2 and P4.3 outputs

    TBCCR0 = 70;                            // PWM Period
    TBCCTL1 = OUTMOD_7;                     // CCR1 reset/set
    TBCCR1 = 10;                            // CCR1 PWM duty cycle
    TBCCTL2 = OUTMOD_7;                     // CCR2 reset/set
    TBCCR2 = 10;                            // CCR2 PWM duty cycle
    TBCCTL3 = OUTMOD_7;                     // CCR3 reset/set
    TBCCR3 = 10;                            // CCR3 PWM duty cycle

    TA1CTL |= TASSEL_2 + MC_1 + ID__4;  // SMCLK, up mode, divide clock by 4
    TA1CTL |= TACLR;

    P3SEL |= BIT2 | BIT3;   // P3.2 and P3.3 option select
    P3DIR |= BIT2 | BIT3;   // P3.2 and P3.3 outputs

    TA1CCR0 = 160;                      // PWM Period = 160/8000000 = 20us => f = 50kHz
    TA1CCTL1 = OUTMOD_7;                // CCR2 reset/set
    TA1CCR1 = 0;                        // CCR2 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                // CCR3 reset/set
    TA1CCR2 = 0;                        // CCR3 PWM duty cycle
}
