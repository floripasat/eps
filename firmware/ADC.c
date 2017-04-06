#include <msp430.h>
#include "ADC.h"

/*
 * main.c
 */

void adc_config(void){

	P6SEL |= 0xff;                             			// select port 6 function as ADC
	P7SEL |= BIT4 + BIT5 + BIT6 + BIT7;					// select p7.4,p7.5,p7.6 and p7.7 function as ADC
	ADC12CTL0 = ADC12ON + ADC12MSC + ADC12SHT0_15; 		// turn on ADC12, enable multiple sample and conversion, extend sampling time to avoid overflow of results
	ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1;       		// use sampling timer, sequence of channels
	ADC12MCTL0 = ADC12INCH_0;              				// ref+=AVcc, channel = A0
	ADC12MCTL1 = ADC12INCH_1;              				// ref+=AVcc, channel = A1
	ADC12MCTL2 = ADC12INCH_2;              				// ref+=AVcc, channel = A2
	ADC12MCTL3 = ADC12INCH_3;        					// ref+=AVcc, channel = A3
	ADC12MCTL4 = ADC12INCH_4;        					// ref+=AVcc, channel = A4
	ADC12MCTL5 = ADC12INCH_5;        					// ref+=AVcc, channel = A5
	ADC12MCTL6 = ADC12INCH_6;        					// ref+=AVcc, channel = A6
	ADC12MCTL7 = ADC12INCH_7;        					// ref+=AVcc, channel = A7
	ADC12MCTL8 = ADC12INCH_8;        					// ref+=AVcc, channel = A8
	ADC12MCTL9 = ADC12INCH_9;        					// ref+=AVcc, channel = A9
	ADC12MCTL10 = ADC12INCH_10;        					// ref+=AVcc, channel = A10
	ADC12MCTL11 = ADC12INCH_11;        					// ref+=AVcc, channel = A11
	ADC12MCTL12 = ADC12INCH_12;        					// ref+=AVcc, channel = A12
	ADC12MCTL13 = ADC12INCH_13;        					// ref+=AVcc, channel = A13
	ADC12MCTL14 = ADC12INCH_14;        					// ref+=AVcc, channel = A14
	ADC12MCTL15 = ADC12INCH_15 + ADC12EOS;   			// ref+=AVcc, channel = A15
	ADC12CTL0 |= ADC12ENC;                    			// Enable conversions

}

int16_t adc_read(uint8_t channel){

	ADC12CTL0 &= ~ADC12SC;
	ADC12CTL0 |= ADC12SC;

	while(!(ADC12IFG & (1 << channel)));

	switch(channel){
	case 0:
		return ADC12MEM0;
	case 1:
		return ADC12MEM1;
	case 2:
		return ADC12MEM2;
	case 3:
		return ADC12MEM3;
	case 4:
		return ADC12MEM4;
	case 5:
		return ADC12MEM5;
	case 6:
		return ADC12MEM6;
	case 7:
		return ADC12MEM7;
	case 8:
		return ADC12MEM8;
	case 9:
		return ADC12MEM9;
	case 10:
		return ADC12MEM10;
	case 11:
		return ADC12MEM11;
	case 12:
		return ADC12MEM12;
	case 13:
		return ADC12MEM13;
	case 14:
		return ADC12MEM14;
	case 15:
		return ADC12MEM15;
	default:
		break;
	}

	return -1;		// returns -1 in case no adc channel was read

}
