#include <msp430.h> 
#include "eps_ADC.h"

/*
 * main.c
 */

int16_t read_adc(uint8_t channel){

	ADC12CTL0 |= ADC12SC;			// Start convn - software trigger

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
	default:
		break;
	}

	return -1;		// returns -1 in case no adc channel was read

}
