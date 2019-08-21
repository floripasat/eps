/**
 * \file
 *
 * \brief DS2775 batteries monitor drivers source
 *
 * \author Sara Vega Martinez <vegamartinezsara@gmail.com> and Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
 */

#include <msp430.h>
#include <stdio.h>
#include "onewire.h"
#include "intrinsics.h"
#include "uart.h"
#include "I2C.h"
#include "config.h"

#define clock 8000000

#define ACCUMULATED_CURRENT_MSB (uint8_t)(BAT_MONITOR_CHARGE_VALUE>>8)
#define ACCUMULATED_CURRENT_LSB (uint8_t)BAT_MONITOR_CHARGE_VALUE

#define P_WireOUT P9OUT
#define P_WireIN P9IN
#define DIR_P_Wire P9DIR
#define BitWire BIT1

volatile int avc_msb=0x00;
volatile int avc_lsb=0x00;
volatile int vr_msb1=0x00;
volatile int vr_lsb1=0x00;
volatile int vr_msb2=0x00;
volatile int vr_lsb2=0x00;
volatile int tr_msb=0x00;
volatile int tr_lsb=0x00;
volatile int cr_msb=0x00;
volatile int cr_lsb=0x00;
volatile int acr_msb=0x00;
volatile int acr_lsb=0x00;

volatile unsigned int RG_Protection;
volatile extern uint8_t EPS_data[70];

unsigned char protection_register_msb = 0x00;
unsigned char protection_register_lsb = 0x00;

volatile unsigned int count = 0;

volatile unsigned int median = 0;
volatile unsigned int vector_msb[3];
volatile unsigned int vector_lsb[3];
volatile unsigned int n = 0;


/**
 * \brief Puts a bit on the one-wire line.
 *
 * Sets the one-wire pin as output and drives it low if the bit is 0
 * or sets it as input to release the bus if the bit is 1.
 *
 * \param bit to be sent.
 *
 * \returns -
 */

void outp(int bit){


    DIR_P_Wire |= BitWire;          //sets P5.5 as output

    if(bit == 0){
        P_WireOUT &= ~BitWire;           //drives P1.0 low
    }
    else{
        //P_WireOUT |= BitWire;           //releases the bus
    DIR_P_Wire &= ~BitWire;
    }
}

/**
 * \brief Reads bit from the one-wire line.
 *
 * Sets the one-wire pin as input and reads it.
 *
 * \param -
 *
 * \returns the bit read.
 */

int inp(void){
volatile unsigned int result=0;

     DIR_P_Wire &= ~BitWire;          //sets pin as input
     result= P_WireIN & BitWire;         //prepares the bit on pin to be returned

     return result;             //returns the bit on pin

    }

/**
 * \brief Generates reset on one-wire line.
 *
 * Drives the line high, delays for 0 seconds, drives the line low,
 * delays for 480 us, drives the line high, delays for 70 us, reads
 * the response from the slave and delays for 410 us.
 *
 * \param -
 *
 * \returns 0 if device is present, 1 if not present.
 */

int OneWireReset(void){

    int result=0;

      outp(1);                              //drives pin to high
    __delay_cycles(0);                      // delay of 0
      outp(0);                              //drives pin low
    __delay_cycles(clock*0.000480);         //delay of 480us, 8,12MHz* 480us=3897

      outp(1);                      //releases the bus
      result = inp();                   //prepares the result of present detection to be returned
      __delay_cycles(clock*0.000070);           //delay of 70us, 8,12MHz*70us=568

      outp(1);
    __delay_cycles(clock*0.000410);         //delay of 410us, 8,12MHz*410us=3329

    return result;                  //returns the presence detection result

}

/**
 * \brief Sends one-wire bit to one-wire slave.
 *
 * If the bit is 1, drives the pin low and delays for 6 us, then sets
 * the pin high and delays for 64 us. If the bit is 0, drives the pin
 * low and delays for 60 us then drives the pin high and delays for 80 us.
 *
 * \param bit to be sent.
 *
 * \returns -
 */

void OneWireWrite(int bit){



        if(bit == 1){
            outp(0);                //drives pin low
        __delay_cycles(clock*0.000006);         //delay of 6us, 8,12 MHz*6us= 44

            outp(1);                //releases the bus
        __delay_cycles(clock*0.000064);     //delay of 64us, 8,12MHz*64us=517

        }else{
           // Write '0' bit
            outp(0);                //drives pin low
        __delay_cycles(clock*0.000060);     //delay of 60us, 8,12MHz*60us=487
            outp(1);                //releases the bus
        __delay_cycles(clock*0.000080);         // delay of 10us, 8MHz*10us=80

        }

}

/**
 * \brief Reads one-wire bit from one-wire slave.
 *
 * Drives the one-wire pin low and delays for 6 us, then drives the pin
 * high and delays for 9 us, reads the result and delays for 55 us.
 *
 * \param -
 *
 * \returns the bit read.
 */
int OneWireReadBit(void){

    int result;
    outp(0);                        //drives pin low
    __delay_cycles(clock*0.000006);             //delay of 6us, 8,12 MHz*6us= 44

    outp(1);                        //releases the bus
    __delay_cycles(clock*0.000009);             //delay of 9us, 8,12MHz*9us=73

    result = inp();                 //sample the bit from slave
    __delay_cycles(clock*0.000055);         //delay of 55us, 8,12MHz*55us=446

    return result;

}

/**
 * \brief Writes a byte to one-wire slave.
 *
 * Loops 8 times calling OneWireWrite passing each bit of the byte to
 * be sent.
 *
 * \param data byte to be sent.
 *
 * \returns -
 */

void OWWriteByte(int data)
{

    int loop;

    for (loop = 0; loop < 8; loop++)    // Loop to write each bit in the byte, LS-bit first
    {
        OneWireWrite(data & 0x01);

        data >>= 1;                     // shift the data byte for the next bit
    }
}

/**
 * \brief Reads a byte from the one-wire slave.
 *
 * Loops 8 times calling OneWireReadBit and adding the result.
 *
 * \param -
 *
 * \returns the byte read.
 */

uint8_t OWReadByte(void)
{
    uint8_t loop, result = 0;
    for (loop = 0; loop < 8; loop++)
    {

        result >>= 1;                       // shift the result to get it ready for the next bit

        if (OneWireReadBit())               // if result is one, then set MS bit
            result |= 0x80;
    }
        return result;
}

/**
 * \brief Configures the DS2775 batteries monitor.
 *
 * Configures the DS2775 registers by the following procedure:
 * - Generates a reset on the one-wire line
 * - Sends the 0xCC general address to the line (only one slave is present)
 * - Sends the write command (0x6C)
 * - Sends the register address
 * - Sends the value to be written
 *
 * The registers are configured as follows:
 *
 * <b>Protection Register</b>
 * Enable charge and discharge.
 *
 * <b>Protector Threshold Register</b>
 * 0x61 bias used to the current accumulation.
 *
 * <b>Status Register</b>
 * Clear all flags.
 *
 * <b>Control Register</b>
 * Set the undervoltage threshold to 2.6 V.
 *
 * <b>Overcurrent Threshold Register</b>
 * Set the short-circuit current to 15 A, the discharge overcurrent
 * to 7.5 A and the charge overcurrent to 5A.
 *
 * <b>Current Gain Register</b>
 * Removes gain from the measurements.
 *
 * If the Write Accumulated Current build is being used, the Accumulated
 * Current Register is written with 3 Ah.
 *
 * In Debug mode, the data sent is read back to verify if it was
 * configured properly.
 *
 * \param -
 *
 * \returns -
 */

void config_DS2775(void){
    // CONFIGURATION DS2775

    volatile unsigned int reset=0x1;
    reset= OneWireReset();              // PROTECTION REGISTER
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                  // write operation
    OWWriteByte(protection_register);   // register address
    OWWriteByte(0x03);                  // value to be written


    reset= OneWireReset();                          // PROTECTOR THRESHOLD REGISTER
    OWWriteByte(0xCC);                              // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                              // write operation
    OWWriteByte(protector_threshold_register);      // register address
    OWWriteByte(0x61);                              // value to be written


    reset= OneWireReset();              // STATUS REGISTER
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                  // write operation
    OWWriteByte(status_register);       // register address
    OWWriteByte(0x00);                  // value to be written



    reset= OneWireReset();              // CONTROL REGISTER
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                  // write operation
    OWWriteByte(control_register);      // register address
    OWWriteByte(0x0C);                  // value to be written

    reset= OneWireReset();              // Overcurrent Threshold REGISTER and current gain calibration MSB
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                  // write operation
    OWWriteByte(overcurrent_thresholds_register);       // register address
    OWWriteByte(0x24);                  // value to be written

    reset= OneWireReset();              // Overcurrent Threshold REGISTER and current gain calibration MSB
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                  // write operation
    OWWriteByte(current_gain_LSB_register);     // register address
    OWWriteByte(0x00);                  // value to be written

#if RESET_BATTERY_ACCUMULATED_CURRENT == 1

    write_accumulated_current_max_value();

#endif //RESET_BATTERY_ACCUMULATED_CURRENT

#ifdef _VERBOSE_DEBUG
    uint8_t one_wire_data_sent_back[8] = {0};
    one_wire_data_sent_back[0] = DS2775_read_register(protection_register);
    one_wire_data_sent_back[1] = DS2775_read_register(protector_threshold_register);
    one_wire_data_sent_back[2] = DS2775_read_register(status_register) & 0xf0;
    one_wire_data_sent_back[3] = DS2775_read_register(control_register);
    one_wire_data_sent_back[4] = DS2775_read_register(overcurrent_thresholds_register);
    one_wire_data_sent_back[5] = DS2775_read_register(current_gain_LSB_register);
    one_wire_data_sent_back[6] = DS2775_read_register(accumulated_current_MSB_register);
    one_wire_data_sent_back[7] = DS2775_read_register(accumulated_current_LSB_register);

    uint8_t string[4];
    uint8_t i = 0;
    uart_tx_debug("DS2775 data:");
    for(i = 0; i < 8; i++){
        sprintf(string, "%#04x", one_wire_data_sent_back[i]);
        uart_tx_debug(string);
        if(i != 7){
            uart_tx_debug(", ");
        }
        else{
            uart_tx_debug("\r\n");
        }
    }
#endif
}

/**
 * \brief Acquires data from the DS2775 batteries monitor [Deprecated]
 */

void measurement_data_DS2775(void){

    volatile unsigned int aux;
    volatile unsigned int reset=0x00;


   // TEMPERATURE MEASUREMENT

    reset= OneWireReset();                      // TEMPERATURE MEASUREMENT - LSB REGISTER
    OWWriteByte(0xCC);                          // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                          // read operation
    OWWriteByte(temperature_LSB_register);      // register address
    aux=OWReadByte();
    tr_lsb=aux>>5;

    reset= OneWireReset();                      // TEMPERATURE MEASUREMENT - MSB REGISTER
    OWWriteByte(0xCC);                          // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                          // read operation
    OWWriteByte(temperature_MSB_register);      // register address
    aux=OWReadByte();
    tr_msb=aux>>5;
    aux=aux<<3;
    tr_lsb|=aux & 0xF8;




    //AVERAGE CURRENT MEASUREMENT

    reset= OneWireReset();                          // AVERAGE CURRENT MEASUREMENT - LSB REGISTER
    OWWriteByte(0xCC);                              // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                              // read operation
    OWWriteByte(average_current_LSB_register);      // register address
    avc_lsb=OWReadByte();


    reset= OneWireReset();                          // AVERAGE CURRENT MEASUREMENT - MSB REGISTER
    OWWriteByte(0xCC);                              // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                              // read operation
    OWWriteByte(average_current_MSB_register);      // register address
    avc_msb=OWReadByte();



    //ACCUMULATED CURRENT MEASUREMENT

    reset= OneWireReset();                              // ACCUMULATED CURRENT MEASUREMENT - LSB REGISTER
    OWWriteByte(0xCC);                                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                                  // read operation
    OWWriteByte(accumulated_current_LSB_register);      // register address
    acr_lsb=OWReadByte();


    reset= OneWireReset();                          // ACCUMULATED CURRENT MEASUREMENT - MSB REGISTER
    OWWriteByte(0xCC);                              // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                              // read operation
    OWWriteByte(accumulated_current_MSB_register);  // register address
    acr_msb=OWReadByte();



    //CURRENT GAIN CALIBRATION

    reset= OneWireReset();              // Overcurrent Threshold REGISTER and current gain calibration MSB
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                  // write operation
    OWWriteByte(overcurrent_thresholds_register);       // register address
    OWWriteByte(0x24);                  // value to be written

    reset= OneWireReset();              // Overcurrent Threshold REGISTER and current gain calibration MSB
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                  // write operation
    OWWriteByte(current_gain_LSB_register);     // register address
    OWWriteByte(0x00);                  // value to be written



   //CURRENT MEASUREMENT

    reset= OneWireReset();              //  CURRENT MEASUREMENT - LSB REGISTER
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                  // read operation
    OWWriteByte(current_LSB_register);  // register address
    cr_lsb=OWReadByte();



    reset= OneWireReset();              // CURRENT MEASUREMENT - MSB REGISTER
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                  // read operation
    OWWriteByte(current_MSB_register);  // register address
    cr_msb=OWReadByte();


    //VOLTAGE MEASUREMENT

    reset= OneWireReset();              // VOLTAGE 1 MEASUREMENT - LSB REGISTER
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                  // read operation
    OWWriteByte(voltage_LSB1_register);  // register address
    aux=OWReadByte();
    vr_lsb1=aux>>5;


    reset= OneWireReset();              // VOLTAGE 1 MEASUREMENT - MSB REGISTER
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                  // read operation
    OWWriteByte(voltage_MSB1_register); // register address
    aux=OWReadByte();
    vr_msb1=aux>>5;
    aux=aux<<3;
    vr_lsb1|=aux & 0xF8;

    reset= OneWireReset();              // VOLTAGE 2 MEASUREMENT - LSB REGISTER
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                  // read operation
    OWWriteByte(voltage_LSB2_register);  // register address
    aux=OWReadByte();
    vr_lsb2=aux>>5;


    reset= OneWireReset();              // VOLTAGE 2 MEASUREMENT - MSB REGISTER
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                  // read operation
    OWWriteByte(voltage_MSB2_register); // register address
    aux=OWReadByte();
    vr_msb2=aux>>5;
    aux=aux<<3;
    vr_lsb2|=aux & 0xF8;



    reset= OneWireReset();              // PROTECTION REGISTER
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                  // read operation
    OWWriteByte(protection_register);   // register address
    RG_Protection=OWReadByte();



    reset= OneWireReset();              // PROTECTION REGISTER CLEAR
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                  // write operation
    OWWriteByte(protection_register);   // register address
    OWWriteByte(0x03);                  // value to be written


    reset= OneWireReset();              // CONTROL REGISTER
    OWWriteByte(0xCC);                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                  // write operation
    OWWriteByte(control_register);      // register address
    OWWriteByte(0x0C);                  // value to be written
}

/**
 * \brief Reads a register from the DS2775 batteries monitor.
 *
 * Generates a reset on the one-wire line, sends the CC general address,
 * sends the read register command (0x69), sends the address of the
 * register to be read.
 *
 * \param register_address is the address of the register to be read.
 *
 * \returns the value read.
 */

uint8_t DS2775_read_register(uint8_t register_address){

    interruption_occurring = 0;             // this variable is set while an interruption is occurring

    uint8_t result;                         // return variable for the function
    uint8_t reset = 0;                      // declare variable to hold reset value

    reset= OneWireReset();                  // reset one wire to start communication
    OWWriteByte(0xCC);                      // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x69);                      // read operation
    OWWriteByte(register_address);          //
    result = OWReadByte();

    if(interruption_occurring == 1){        // Prevents the interrupt from corrupting the data:
                                            // When a interrupt occurs during the communication returns the previous value
        interruption_occurring = 0;

        switch(register_address){

            case    average_current_LSB_register:
                result =  EPS_data[battery_average_current_LSB];
                break;

            case   average_current_MSB_register:
                result =  EPS_data[battery_average_current_MSB];
                break;

            case    temperature_LSB_register:
                result =  EPS_data[battery_monitor_temeperature_LSB];
                break;

            case    temperature_MSB_register:
                result =  EPS_data[battery_monitor_temeperature_MSB];
                break;

            case    voltage_LSB1_register:
                result =  EPS_data[battery1_voltage_LSB];
                break;

            case    voltage_MSB1_register:
                result =  EPS_data[battery1_voltage_MSB];
                break;

            case    voltage_LSB2_register:
                result =  EPS_data[battery2_voltage_LSB];
                break;

            case    voltage_MSB2_register:
                result =  EPS_data[battery2_voltage_MSB];
                break;

            case    current_LSB_register:
                result =  EPS_data[battery_current_LSB];
                break;

            case    current_MSB_register:
                result =  EPS_data[battery_current_MSB];
                break;

            case    accumulated_current_LSB_register:
                result =  EPS_data[battery_accumulated_current_LSB];
                break;

            case    accumulated_current_MSB_register:
                result =  EPS_data[battery_accumulated_current_MSB];
                break;

            case    protection_register:
                result =  EPS_data[protection_register_LSB];
                break;

            default:
                result = 0;
                break;
        }
    }

    switch(register_address){                                                      // This block takes the result and organizes into a vector, calculates the median between...
                                                                                   // the current result and the two previous results and returns the median of these three values.
        case    accumulated_current_LSB_register:                                  // When the result is a corrupt data, the median on the vector will be the earlier result.


            vector_lsb[0]  = vector_lsb[1];                                                 // Organize the vector
            vector_lsb[1]  = vector_lsb[2];
            vector_lsb[2]  = result;

            if((vector_lsb[0] >= vector_lsb[1] && vector_lsb[1] >= vector_lsb[2]) ||        // Calculate the median
                (vector_lsb[2] >= vector_lsb[1] && vector_lsb[1] >= vector_lsb[0]) )
            {
                median = vector_lsb[1];
            }

            if((vector_lsb[1] >= vector_lsb[2] && vector_lsb[2] >= vector_lsb[0]) ||
               (vector_lsb[0] >= vector_lsb[2] && vector_lsb[2] >= vector_lsb[1]) )
            {
                median = vector_lsb[2];
            }
            if((vector_lsb[2] >= vector_lsb[0] && vector_lsb[0] >= vector_lsb[1]) ||
               (vector_lsb[1] >= vector_lsb[0] && vector_lsb[0] >= vector_lsb[2]) )
            {
                median = vector_lsb[0];
            }

            if(n <= 6) n++;                                                                  // Wait the vector to be full
            if(n > 6) result = median;                                                    // When the vector is ready, return the median


            break;


        case    accumulated_current_MSB_register:


            vector_msb[0]  = vector_msb[1];                                                 // Organize the vector
            vector_msb[1]  = vector_msb[2];
            vector_msb[2]  = result;

            if((vector_msb[0] >= vector_msb[1] && vector_msb[1] >= vector_msb[2]) ||         // Calculate the median
               (vector_msb[2] >= vector_msb[1] && vector_msb[1] >= vector_msb[0]) )
            {
                median = vector_msb[1];
            }
            if( (vector_msb[1] >= vector_msb[2] && vector_msb[2] >= vector_msb[0]) ||
                (vector_msb[0] >= vector_msb[2] && vector_msb[2] >= vector_msb[1]) )
            {
                median = vector_msb[2];
            }
            if( (vector_msb[2] >= vector_msb[0] && vector_msb[0] >= vector_msb[1]) ||
                (vector_msb[1] >= vector_msb[0] && vector_msb[0] >= vector_msb[2]) )
            {
                median = vector_msb[0];
            }

            if(n <= 6) n++;                                                                     // Wait the vector to be full
            if(n > 6) result = median;                                                        // When the vector is ready, return the median


            break;

    }




    return result;  // return value read from register

}


/**
 * \brief Writes accumulated current maximum value (3 Ah) to batteries monitor appropriated register.
 *
 * \param
 *
 * \returns
 */

void write_accumulated_current_max_value(void){         // write 3Ah to battery accumulated current
    volatile unsigned int reset=0x1;

    reset = OneWireReset();                              // ACCUMULATED CURRENT - MSB REGISTER
    OWWriteByte(0xCC);                                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                                  // write operation
    OWWriteByte(accumulated_current_MSB_register);      // register address
    OWWriteByte(ACCUMULATED_CURRENT_MSB);               // value to be written

    reset = OneWireReset();
    OWWriteByte(0xCC);                                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x48);                                  // copy data command
    OWWriteByte(accumulated_current_MSB_register);      // register address

    reset = OneWireReset();                              // ACCUMULATED CURRENT - LSB REGISTER
    OWWriteByte(0xCC);                                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x6C);                                  // write operation
    OWWriteByte(accumulated_current_LSB_register);      // register address
    OWWriteByte(ACCUMULATED_CURRENT_LSB);               // value to be written

    reset = OneWireReset();
    OWWriteByte(0xCC);                                  // eeprom address (only one slave on bus, CC is used)
    OWWriteByte(0x48);                                  // copy data command
    OWWriteByte(accumulated_current_LSB_register);      // register address
}
