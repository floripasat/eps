#include <msp430.h>
#include "eps_timer.h"
#include "eps_onewire.h"
#include "eps_i2c.h"
#include "watchdog.h"
#include "ADS1248.h"
#include "pid.h"
#include "eps_ADC.h"
#include "eps_uart.h"
#include <stdlib.h>
#include <stdio.h>

volatile float duty_cycle = 0;
volatile long t = 0;
volatile float temperature = 0;
volatile extern uint8_t EPS_data[23];

volatile uint16_t adc0 = 0x00;
volatile uint16_t adc1 = 0x00;
volatile uint16_t adc2 = 0x00;
volatile uint16_t adc3 = 0x00;
volatile uint16_t adc4 = 0x00;
volatile uint16_t adc5 = 0x00;
volatile uint16_t adc6 = 0x00;
volatile uint16_t adc7 = 0x00;
volatile uint16_t adc12 = 0x00;
volatile uint16_t adc13 = 0x00;
volatile uint16_t adc14 = 0x00;
volatile uint16_t msp_ts = 0x00;

struct Pid parameters = {0, 0, 1, 250, 20, 0 , INT_MAX, 150};


/********** INTERRUPTS **********/

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0_a0_isr(void){

    #ifdef _DEBUG
//    P1OUT ^= 0x01;		// Toggle P1.0
	#endif

}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer1_a0_isr(void){

	#ifdef _DEBUG
	P3OUT ^= 0x01;		// Toggle P3.0
	#endif

    adc0 = read_adc(0);
    EPS_data[3] = (uint8_t) (adc0 >> 8);
    EPS_data[4] = (uint8_t) (adc0 & 0xff);

    adc1 = read_adc(1);
    EPS_data[5] = (uint8_t) (adc1 >> 8);
    EPS_data[6] = (uint8_t) (adc1 & 0xff);

    adc2 = read_adc(2);
    EPS_data[7] = (uint8_t) (adc2 >> 8);
    EPS_data[8] = (uint8_t) (adc2 & 0xff);

    adc3 = read_adc(3);
    EPS_data[9] = (uint8_t) (adc3 >> 8);
    EPS_data[10] = (uint8_t) (adc3 & 0xff);

    adc4 = read_adc(4);
    EPS_data[11] = (uint8_t) (adc4 >> 8);
    EPS_data[12] = (uint8_t) (adc4 & 0xff);

    adc5 = read_adc(5);
    EPS_data[13] = (uint8_t) (adc5 >> 8);
    EPS_data[14] = (uint8_t) (adc5 & 0xff);

    adc6 = read_adc(6);
    EPS_data[15] = (uint8_t) (adc6 >> 8);
    EPS_data[16] = (uint8_t) (adc6 & 0xff);

    adc12 = read_adc(12);
    EPS_data[17] = (uint8_t) (adc12 >> 8);
    EPS_data[18] = (uint8_t) (adc12 & 0xff);

    adc13 = read_adc(13);
    EPS_data[19] = (uint8_t) (adc13 >> 8);
    EPS_data[20] = (uint8_t) (adc13 & 0xff);

    adc14 = read_adc(14);
    EPS_data[21] = (uint8_t) (adc14 >> 8);
    EPS_data[22] = (uint8_t) (adc14 & 0xff);
}


/*
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	__bis_SR_register(GIE);
	if(cont==9){												// period = CCR0 * 2 * cont / clock => 1 = 50000*2*cont/(10^6) => cont = 9 (starts at 0)
		cont = 0;												// reset cont


		measurement_data_DS2775();
		wdt_reset_counter();
		read_ADS1248(6);
		wdt_reset_counter();
		temperature = (read_ADS1248(6)*0.00111342/8 - 1000)/3.85;
		duty_cycle = Pid_Control(42,temperature,parameters);
		TBCCR3 = (1-duty_cycle)*PWM_PERIOD/2;
		wdt_reset_counter();
		make_frame();
		wdt_reset_counter();
	}else{
		cont ++;											// increments cont to achieve desired output period
		wdt_reset_counter();
	}
}
*/
void make_frame(void)
{
	EPSData[0] = '{';
	EPSData[1] = '{';
	EPSData[2] = '{';
	EPSData[3] = cr_msb;
	EPSData[4] = cr_lsb;
	EPSData[5] = vr_msb1;
	EPSData[6] = vr_lsb1;
	EPSData[7] = vr_msb2;
	EPSData[8] = vr_lsb2;
	EPSData[9] = tr_msb;
	EPSData[10] = tr_lsb;
	EPSData[11] = acr_msb;
	EPSData[12] = acr_lsb;
	EPSData[13] =  RG_Protection;
//	EPSData[14] = temp[0];
//	EPSData[15] = temp[1];
//	EPSData[16] = temp[2];
	EPSData[17] = 0;
	EPSData[18] = 0;
	EPSData[19] = 0;
	EPSData[20] = '}';
	EPSData[21] = '\n';
	EPSData[22] = '\r';
}
