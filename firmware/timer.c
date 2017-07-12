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

volatile float duty_cycle = 0;
volatile long t = 0;
volatile float temperature = 0;
volatile extern uint8_t EPS_data[70];

volatile uint16_t adc0 = 0;
volatile uint16_t adc1 = 0;
volatile uint16_t adc2 = 0;
volatile uint16_t adc3 = 0;
volatile uint16_t adc4 = 0;
volatile uint16_t adc5 = 0;
volatile uint16_t adc6 = 0;
volatile uint16_t adc7 = 0;
volatile uint16_t adc10 = 0;
volatile uint16_t adc12 = 0;
volatile uint16_t adc13 = 0;
volatile uint16_t adc14 = 0;
volatile uint16_t adc15 = 0;
volatile uint16_t msp_ts = 0;
volatile uint32_t temp_1 = 0;

volatile uint16_t negative_y_panel_current_mean = 0;				// take mean of adc0
volatile uint16_t positive_x_panel_current_mean = 0;				// take mean of adc1
volatile uint16_t negative_x_panel_current_mean = 0;				// take mean of adc2
volatile uint16_t positive_z_panel_current_mean = 0;				// take mean of adc3
volatile uint16_t negative_z_panel_current_mean = 0;				// take mean of adc4
volatile uint16_t positive_y_panel_current_mean = 0;				// take mean of adc5
volatile uint16_t negative_y_positive_x_panel_voltage_mean = 0;		// take mean of adc12
volatile uint16_t negative_x_positive_z_panel_voltage_mean = 0;		// take mean of adc13
volatile uint16_t negative_z_positive_y_panel_voltage_mean = 0;		// take mean of adc14

const struct Pid parameters = {0, 0, 1, 250, 20, 0 , INT_MAX, 150};


/********** INTERRUPTS **********/

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0_a0_isr(void){

	__enable_interrupt();

	volatile static uint8_t counter_30s = 0;

    #ifdef _DEBUG
	timer_debug_port_1s ^= timer_debug_pin_1s;		// Toggle 1s debug piun
	#endif

	adc15 = adc_read(vpanels_voltage);				// Vpanels voltage measurement
	EPS_data[vpanels_voltage_LSB] = (uint8_t) (adc15 & 0xff);		// bitwise and with 0xff to get LSB
    EPS_data[vpanels_voltage_MSB] = (uint8_t) (adc15 >> 8);			// shift data 8 bits to get MSB

#ifdef _DEBUG
    uart_tx_debug("**** Misc ADC ****");
    uart_tx_debug("\r\n");
    uart_tx_debug("Vpanels Voltage: ");
    float_send(adc15*0.00244140625);
    uart_tx_debug("\r\n");
#endif

    watchdog_reset_counter();

    adc7 = adc_read(bus_voltage);				// bus voltage measurement
    EPS_data[bus_voltage_LSB] = (uint8_t) (adc7 & 0xff);		// bitwise and with 0xff to get LSB
    EPS_data[bus_voltage_MSB] = (uint8_t) (adc7 >> 8);		// shift data 8 bits to get MSB

#ifdef _DEBUG
    uart_tx_debug("Vbus Voltage: ");
    float_send(adc7*0.00244140625);
    uart_tx_debug("\r\n");
#endif

    watchdog_reset_counter();


	adc6 = adc_read(beacon_eps_current);		// beacon/eps current measurement
    EPS_data[beacon_eps_current_LSB] = (uint8_t) (adc6 & 0xff);		// bitwise and with 0xff to get LSB
    EPS_data[beacon_eps_current_MSB] = (uint8_t) (adc6 >> 8);		// shift data 8 bits to get MSB

    watchdog_reset_counter();

#ifdef _DEBUG

    uart_tx_debug("Beacon/EPS Current: ");
    float_send(adc6*0.0001229927582);
    uart_tx_debug("\r\n");

#endif


    adc10 = adc_read(msp_temperature);
    EPS_data[msp_temperature_LSB] = (uint8_t) (adc10 & 0xff);	// bitwise and with 0xff to get LSB
    EPS_data[msp_temperature_MSB] = (uint8_t) (adc10 >> 8);		// shift data 8 bits to get MSB

    watchdog_reset_counter();

    EPS_data[battery_average_current_LSB] = DS2775_read_register(average_current_LSB_register);		// read battery average current LSB
    EPS_data[battery_average_current_MSB] = DS2775_read_register(average_current_MSB_register);		// read battery average current MSB

    watchdog_reset_counter();

    EPS_data[battery_monitor_temeperature_LSB] = DS2775_read_register(temperature_LSB_register);		// read battery temperature LSB
    EPS_data[battery_monitor_temeperature_MSB] = DS2775_read_register(temperature_MSB_register);		// read battery temperature MSB

    watchdog_reset_counter();

    EPS_data[battery1_voltage_LSB] = DS2775_read_register(voltage_LSB1_register);		// read battery 1 voltage LSB
    EPS_data[battery1_voltage_MSB] = DS2775_read_register(voltage_MSB1_register);		// read battery 1 voltage MSB

    watchdog_reset_counter();

    EPS_data[battery2_voltage_LSB] = DS2775_read_register(voltage_LSB2_register);		// read battery 2 voltage LSB
    EPS_data[battery2_voltage_MSB] = DS2775_read_register(voltage_MSB2_register);		// read battery 2 voltage MSB

    watchdog_reset_counter();

    EPS_data[battery_current_LSB] = DS2775_read_register(current_LSB_register);		// read battery current LSB
    EPS_data[battery_current_MSB] = DS2775_read_register(current_MSB_register);		// read battery current MSB

    watchdog_reset_counter();

    EPS_data[battery_accumulated_current_LSB] = DS2775_read_register(accumulated_current_LSB_register);		// read battery current LSB
    EPS_data[battery_accumulated_current_MSB] = DS2775_read_register(accumulated_current_MSB_register);		// read battery current MSB

    watchdog_reset_counter();

	EPS_data[protection_register_LSB] = DS2775_read_register(protection_register);		// read protection register

    temp_1 = read_ADS1248(6);


    /*

    EPS_data[47] = temp_1 & 0xff;
    EPS_data[48] = (temp_1 & 0xff00) >> 8;
    EPS_data[49] = (temp_1 & 0xff0000) >> 16;



	#ifdef _DEBUG
    	uart_tx_debug("**** ADS1248 Mesurements ****\r\n");
    	uart_tx_debug("channel 6: ");
    	float_send(2*1650*0.000002/(2^24)*temp_1);
	#endif

*/

    watchdog_reset_counter();

    EPS_data[eps_status] = energyLevelAlgorithm(EPS_data[eps_status]);

    watchdog_reset_counter();

    if(counter_30s == 29){
    	volatile uint8_t beacon_packet[33] = {0};
    	counter_30s = 0;

    	beacon_packet[0] = 0x7E;
    	beacon_packet[1] = EPS_data[battery1_voltage_LSB];
    	beacon_packet[2] = EPS_data[battery1_voltage_MSB];
    	beacon_packet[3] = EPS_data[battery2_voltage_LSB];
    	beacon_packet[4] = EPS_data[battery2_voltage_MSB];
    	beacon_packet[5] = EPS_data[negative_y_panel_current_LSB];
    	beacon_packet[6] = EPS_data[negative_y_panel_current_MSB];
    	beacon_packet[7] = EPS_data[positive_x_panel_current_LSB];
    	beacon_packet[8] = EPS_data[positive_x_panel_current_MSB];
    	beacon_packet[9] = EPS_data[negative_x_panel_current_LSB];
    	beacon_packet[10] = EPS_data[negative_x_panel_current_MSB];
    	beacon_packet[11] = EPS_data[positive_z_panel_current_LSB];
    	beacon_packet[12] = EPS_data[positive_z_panel_current_MSB];
    	beacon_packet[13] = EPS_data[negative_z_panel_current_LSB];
    	beacon_packet[14] = EPS_data[negative_z_panel_current_MSB];
    	beacon_packet[15] = EPS_data[positive_y_panel_current_LSB];
    	beacon_packet[16] = EPS_data[positive_y_panel_current_MSB];
    	beacon_packet[17] = EPS_data[negative_y_positive_x_panel_voltage_LSB];
    	beacon_packet[18] = EPS_data[negative_y_positive_x_panel_voltage_MSB];
    	beacon_packet[19] = EPS_data[negative_x_positive_z_panel_voltage_LSB];
    	beacon_packet[20] = EPS_data[negative_x_positive_z_panel_voltage_MSB];
    	beacon_packet[21] = EPS_data[negative_z_positive_y_panel_voltage_LSB];
    	beacon_packet[22] = EPS_data[negative_z_positive_y_panel_voltage_MSB];
    	beacon_packet[23] = EPS_data[RTD1_B1];
    	beacon_packet[24] = EPS_data[RTD1_B2];
    	beacon_packet[25] = EPS_data[RTD1_B3];
    	beacon_packet[26] = EPS_data[RTD2_B1];
    	beacon_packet[27] = EPS_data[RTD2_B2];
    	beacon_packet[28] = EPS_data[RTD2_B3];
    	beacon_packet[29] = EPS_data[battery_accumulated_current_LSB];
    	beacon_packet[30] = EPS_data[battery_accumulated_current_MSB];
    	beacon_packet[31] = EPS_data[eps_status];
    	beacon_packet[32] = crc8(0x03, 0x92, beacon_packet, 32);

    	volatile uint8_t i = 0;

    	for(i = 0; i <= 31; i++){
    		uart_tx_beacon(beacon_packet[i]);
    	}
    }
    else{
    	counter_30s++;
    }

	#ifdef _DEBUG
    uint8_t protection_register_string[30] = {0};

    watchdog_reset_counter();
    uart_tx_debug("**** DS2775 Measurements: ****\r\n");
    uart_tx_debug("Battery I voltage: ");
    float_send(voltage_unit*((EPS_data[36] << 8) + EPS_data[35]));
    uart_tx_debug("\r\n");
    uart_tx_debug("Battery II voltage: ");
    float_send(voltage_unit*((EPS_data[38] << 8) + EPS_data[37]));
    uart_tx_debug("\r\n");
    uart_tx_debug("Battery Current: ");
    float_send(current_unit*((EPS_data[40] << 8) + EPS_data[39]));
    uart_tx_debug("\r\n");
    uart_tx_debug("Battery Average Current: ");
    float_send(current_unit*((EPS_data[32] << 8) + EPS_data[31]));
    uart_tx_debug("\r\n");
    uart_tx_debug("Battery Accumulated Current: ");
    float_send(accumulated_current_unit*((EPS_data[42] << 8) + EPS_data[41]));
    uart_tx_debug("\r\n");
    sprintf(protection_register_string, "%#04x", EPS_data[43] & 0x0f);
    uart_tx_debug("Protection Register: ");
    uart_tx_debug(protection_register_string);
    uart_tx_debug("\r\n\n");

	#endif

}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer1_a0_isr(void){

	__enable_interrupt();

	#ifdef _DEBUG
	timer_debug_port_100ms ^= timer_debug_pin_100ms;	// Toggle 100ms debug pin
	#endif

	static volatile uint8_t mean_counter = 0;

	watchdog_reset_counter();

	if(mean_counter == 0){

    	negative_y_panel_current_mean = 0;					// reset adc0
    	positive_x_panel_current_mean = 0;					// reset adc1
    	negative_x_panel_current_mean = 0;					// reset adc2
    	positive_z_panel_current_mean = 0;					// reset adc3
    	negative_z_panel_current_mean = 0;					// reset adc4
    	positive_y_panel_current_mean = 0;					// reset adc5
    	negative_y_positive_x_panel_voltage_mean = 0;		// reset adc12
    	negative_x_positive_z_panel_voltage_mean = 0;		// reset adc13
    	negative_z_positive_y_panel_voltage_mean = 0;		// reset adc14
	}

    adc0 = adc_read(negative_y_panel_current);		// -Y panel current measurement
    EPS_data[3] = (uint8_t) (adc0 & 0xff);			// bitwise and with 0xff to get LSB
    EPS_data[4] = (uint8_t) (adc0 >> 8);			// shift data 8 bits to get MSB
    negative_y_panel_current_mean += adc0;			// add measurement value to mean calculation

    watchdog_reset_counter();

    adc1 = adc_read(positive_x_panel_current);		// +X panel current measurement
    EPS_data[5] = (uint8_t) (adc1 & 0xff);			// bitwise and with 0xff to get LSB
    EPS_data[6] = (uint8_t) (adc1 >> 8);			// shift data 8 bits to get MSB
    positive_x_panel_current_mean += adc1;			// add measurement value to mean calculation

    watchdog_reset_counter();

    adc2 = adc_read(negative_x_panel_current);		// -X panel current measurement
    EPS_data[7] = (uint8_t) (adc2 & 0xff);			// bitwise and with 0xff to get LSB
    EPS_data[8] = (uint8_t) (adc2 >> 8);			// shift data 8 bits to get MSB
    negative_x_panel_current_mean += adc2;			// add measurement value to mean calculation

    watchdog_reset_counter();

    adc3 = adc_read(positive_z_panel_current);		// +Z panel current measurement
    EPS_data[9] = (uint8_t) (adc3 & 0xff);			// bitwise and with 0xff to get LSB
    EPS_data[10] = (uint8_t) (adc3 >> 8);			// shift data 8 bits to get MSB
    positive_z_panel_current_mean += adc3;			// add measurement value to mean calculation

    watchdog_reset_counter();

    adc4 = adc_read(negative_z_panel_current);		// -Z panel current measurement
    EPS_data[11] = (uint8_t) (adc4 & 0xff);			// bitwise and with 0xff to get LSB
    EPS_data[12] = (uint8_t) (adc4 >> 8);			// shift data 8 bits to get MSB
    negative_z_panel_current_mean += adc4;			// add measurement value to mean calculation

    watchdog_reset_counter();

    adc5 = adc_read(positive_y_panel_current);		// +Y panel current measurement
    EPS_data[13] = (uint8_t) (adc5 & 0xff);			// bitwise and with 0xff to get LSB
    EPS_data[14] = (uint8_t) (adc5 >> 8);			// shift data 8 bits to get MSB
    positive_y_panel_current_mean += adc5;			// add measurement value to mean calculation

    watchdog_reset_counter();

    adc12 = adc_read(negative_y_positive_x_panel_voltage);		// -Y and +X panels voltage
    EPS_data[17] = (uint8_t) (adc12 & 0xff);					// bitwise and with 0xff to get LSB
    EPS_data[18] = (uint8_t) (adc12 >> 8);						// shift data 8 bits to get MSB
    negative_y_positive_x_panel_voltage_mean += adc12;			// add measurement value to mean calculation

    watchdog_reset_counter();

    adc13 = adc_read(negative_x_positive_z_panel_voltage);		// -X and +Z panels voltage
    EPS_data[19] = (uint8_t) (adc13 & 0xff);					// bitwise and with 0xff to get LSB
    EPS_data[20] = (uint8_t) (adc13 >> 8);						// shift data 8 bits to get MSB
    negative_x_positive_z_panel_voltage_mean += adc13;			// add measurement value to mean calculation

    watchdog_reset_counter();

    adc14 = adc_read(negative_z_positive_y_panel_voltage);		// -Z and +Y panels voltage
    EPS_data[21] = (uint8_t) (adc14 & 0xff);					// bitwise and with 0xff to get LSB
    EPS_data[22] = (uint8_t) (adc14 >> 8);						// shift data 8 bits to get MSB
    negative_z_positive_y_panel_voltage_mean += adc14;			// add measurement value to mean calculation

    watchdog_reset_counter();

    if(mean_counter == 9){

    	mean_counter = 0;		// reset mean counter

    	negative_y_panel_current_mean /= 10;				// take mean of adc0
    	positive_x_panel_current_mean /= 10;				// take mean of adc1
    	negative_x_panel_current_mean /= 10;				// take mean of adc2
    	positive_z_panel_current_mean /= 10;				// take mean of adc3
    	negative_z_panel_current_mean /= 10;				// take mean of adc4
    	positive_y_panel_current_mean /= 10;				// take mean of adc5
    	negative_y_positive_x_panel_voltage_mean /= 10;		// take mean of adc12
    	negative_x_positive_z_panel_voltage_mean /= 10;		// take mean of adc13
    	negative_z_positive_y_panel_voltage_mean /= 10;		// take mean of adc14

#ifdef _DEBUG
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
#endif

    mppt_algorithm((negative_y_panel_current_mean + positive_x_panel_current_mean), negative_y_positive_x_panel_voltage_mean, 0x03D4);

    }
    else{
    	mean_counter++;
    }
}

void timer_config(void){

	timer_debug_port_dir |= timer_debug_pin_1s;		// P1.0 output
	TA0CCR0 = 32768;								// timer A0 CCR0 interrupt period = 32768 * 1/32768 = 1s
	TA0CCTL0 = CCIE;                        		// timer A0 CCR0 interrupt enabled
	TA0CTL = TASSEL_1 + MC_1 + TACLR;       		// SMCLK, upmode, timer A interrupt enable, clear TAR

	timer_debug_port_dir |= timer_debug_pin_100ms;	// P1.1 output
	TA1CCR0 = 3277;									// timer A1 CCR0 interrupt period = 3277 * 1/32768 = 100.006ms
	TA1CCTL0 = CCIE;								// timer A1 CCR0 interrupt enabled
	TA1CTL = TASSEL_1 + MC_1 + TACLR;       		// SMCLK, upmode, timer A interrupt enable, clear TAR

	TBCTL |= TBSSEL_2 + MC_1;
	TBCTL |= TBCLR;

	P4SEL |= BIT1 | BIT2 | BIT3;   // P4.1, P4.2 and P4.3 option select
	P4DIR |= BIT1 | BIT2 | BIT3;   // P4.1, P4.2 and P4.3 outputs

	TBCCR0 = 70;                      		// PWM Period
	TBCCTL1 = OUTMOD_7;                       // CCR1 reset/set
	TBCCR1 = 32;                  				// CCR1 PWM duty cycle
	TBCCTL2 = OUTMOD_7;                       // CCR2 reset/set
	TBCCR2 = 0;                  				// CCR2 PWM duty cycle
	TBCCTL3 = OUTMOD_7;						// CCR3 reset/set
	TBCCR3 = 0;								// CCR3 PWM duty cycle
}

