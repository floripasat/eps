#include <msp430.h> 
#include "eps_ADC.h"

/*
 * main.c
 */

int16_t read_adc(uint8_t channel){

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
