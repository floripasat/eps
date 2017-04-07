#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include "watchdog.h"
#include "ADS1248.h"
#include "pid.h"
#include "hal.h"
#include "ADC.h"
#include "timer.h"
#include "I2C.h"
#include "onewire.h"
#include "uart.h"

volatile float duty_cycle = 0;
volatile long t = 0;
volatile float temperature = 0;
volatile extern uint8_t EPS_data[69];

volatile uint16_t adc0 = 0;
volatile uint16_t adc1 = 0;
volatile uint16_t adc2 = 0;
volatile uint16_t adc3 = 0;
volatile uint16_t adc4 = 0;
volatile uint16_t adc5 = 0;
volatile uint16_t adc6 = 0;
volatile uint16_t adc7 = 0;
volatile uint16_t adc12 = 0;
volatile uint16_t adc13 = 0;
volatile uint16_t adc14 = 0;
volatile uint16_t adc15 = 0;
volatile uint16_t msp_ts = 0;

volatile uint16_t negative_y_panel_current_mean = 10;				// take mean of adc0
volatile uint16_t positive_x_panel_current_mean = 10;				// take mean of adc1
volatile uint16_t negative_x_panel_current_mean = 10;				// take mean of adc2
volatile uint16_t positive_z_panel_current_mean = 10;				// take mean of adc3
volatile uint16_t negative_z_panel_current_mean = 10;				// take mean of adc4
volatile uint16_t positive_y_panel_current_mean = 10;				// take mean of adc5
volatile uint16_t negative_y_positive_x_panel_voltage_mean = 10;		// take mean of adc12
volatile uint16_t negative_x_positive_z_panel_voltage_mean = 10;		// take mean of adc13
volatile uint16_t negative_z_positive_y_panel_voltage_mean = 10;		// take mean of adc14

const struct Pid parameters = {0, 0, 1, 250, 20, 0 , INT_MAX, 150};


/********** INTERRUPTS **********/

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0_a0_isr(void){

    #ifdef _DEBUG
	timer_debug_port_1s ^= timer_debug_pin_1s;		// Toggle 1s debug piun
	#endif

	adc15 = adc_read(vpanels_voltage);				// Vpanels voltage measurement
    EPS_data[23] = (uint8_t) (adc15 >> 8);			// shift data 8 bits to get MSB
    EPS_data[24] = (uint8_t) (adc15 & 0xff);		// bitwise and with 0xff to get LSB

    adc7 = adc_read(bus_voltage);				// bus voltage measurement
    EPS_data[25] = (uint8_t) (adc7 >> 8);		// shift data 8 bits to get MSB
    EPS_data[26] = (uint8_t) (adc7 & 0xff);		// bitwise and with 0xff to get LSB

	adc6 = adc_read(beacon_eps_current);		// beacon/eps current measurement
    EPS_data[27] = (uint8_t) (adc6 >> 8);		// shift data 8 bits to get MSB
    EPS_data[28] = (uint8_t) (adc6 & 0xff);		// bitwise and with 0xff to get LSB

}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer1_a0_isr(void){

	#ifdef _DEBUG
	timer_debug_port_100ms ^= timer_debug_pin_100ms;	// Toggle 100ms debug pin
	#endif

	static volatile uint8_t mean_counter = 0;

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
    EPS_data[3] = (uint8_t) (adc0 >> 8);			// shift data 8 bits to get MSB
    EPS_data[4] = (uint8_t) (adc0 & 0xff);			// bitwise and with 0xff to get LSB
    negative_y_panel_current_mean += adc0;			// add measurement value to mean calculation

    adc1 = adc_read(positive_x_panel_current);		// +X panel current measurement
    EPS_data[5] = (uint8_t) (adc1 >> 8);			// shift data 8 bits to get MSB
    EPS_data[6] = (uint8_t) (adc1 & 0xff);			// bitwise and with 0xff to get LSB
    positive_x_panel_current_mean += adc1;			// add measurement value to mean calculation

    adc2 = adc_read(negative_x_panel_current);		// -X panel current measurement
    EPS_data[7] = (uint8_t) (adc2 >> 8);			// shift data 8 bits to get MSB
    EPS_data[8] = (uint8_t) (adc2 & 0xff);			// bitwise and with 0xff to get LSB
    negative_x_panel_current_mean += adc2;			// add measurement value to mean calculation

    adc3 = adc_read(positive_z_panel_current);		// +Z panel current measurement
    EPS_data[9] = (uint8_t) (adc3 >> 8);			// shift data 8 bits to get MSB
    EPS_data[10] = (uint8_t) (adc3 & 0xff);			// bitwise and with 0xff to get LSB
    positive_z_panel_current_mean += adc3;			// add measurement value to mean calculation

    adc4 = adc_read(negative_z_panel_current);		// -Z panel current measurement
    EPS_data[11] = (uint8_t) (adc4 >> 8);			// shift data 8 bits to get MSB
    EPS_data[12] = (uint8_t) (adc4 & 0xff);			// bitwise and with 0xff to get LSB
    negative_z_panel_current_mean += adc4;			// add measurement value to mean calculation

    adc5 = adc_read(positive_y_panel_current);		// +Y panel current measurement
    EPS_data[13] = (uint8_t) (adc5 >> 8);			// shift data 8 bits to get MSB
    EPS_data[14] = (uint8_t) (adc5 & 0xff);			// bitwise and with 0xff to get LSB
    positive_y_panel_current_mean += adc5;			// add measurement value to mean calculation

    adc12 = adc_read(negative_y_positive_x_panel_voltage);		// -Y and +X panels voltage
    EPS_data[17] = (uint8_t) (adc12 >> 8);						// shift data 8 bits to get MSB
    EPS_data[18] = (uint8_t) (adc12 & 0xff);					// bitwise and with 0xff to get LSB
    negative_y_positive_x_panel_voltage_mean += adc12;			// add measurement value to mean calculation

    adc13 = adc_read(negative_x_positive_z_panel_voltage);		// -X and +Z panels voltage
    EPS_data[19] = (uint8_t) (adc13 >> 8);						// shift data 8 bits to get MSB
    EPS_data[20] = (uint8_t) (adc13 & 0xff);					// bitwise and with 0xff to get LSB
    negative_x_positive_z_panel_voltage_mean += adc13;			// add measurement value to mean calculation

    adc14 = adc_read(negative_z_positive_y_panel_voltage);		// -Z and +Y panels voltage
    EPS_data[21] = (uint8_t) (adc14 >> 8);						// shift data 8 bits to get MSB
    EPS_data[22] = (uint8_t) (adc14 & 0xff);					// bitwise and with 0xff to get LSB
    negative_z_positive_y_panel_voltage_mean += adc14;			// add measurement value to mean calculation

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

    }
    else{
    	mean_counter++;
    }
}

void timer_config(void){

	P1DIR |= 0x01;                          // P1.0 output
	TA0CCR0 = 32768;						// timer A0 CCR0 interrupt period = 32768 * 1/32768 = 1s
	TA0CCTL0 = CCIE;                        // timer A0 CCR0 interrupt enabled
	TA0CTL = TASSEL_1 + MC_1 + TACLR;       // SMCLK, upmode, timer A interrupt enable, clear TAR

	P3DIR |= 0x01;							// P3.0 output
	TA1CCR0 = 3277;							// timer A1 CCR0 interrupt period = 3277 * 1/32768 = 100.006ms
	TA1CCTL0 = CCIE;						// timer A1 CCR0 interrupt enabled
	TA1CTL = TASSEL_1 + MC_1 + TACLR;       // SMCLK, upmode, timer A interrupt enable, clear TAR
}

