/*
 ** \file
 *
 * \brief ADC driver source
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
 */

#include <msp430.h>
#include <stdint.h>
#include "ADC.h"
#include "avoid_infinit_loops.h"

/**
 * \brief Configures the ADC peripheral
 *
 * Configures all port 6 pins and pins 4, 5, 6 and 7 from port 7 as ADC inputs, allows the reference control to the ADC12 registers in the REFCTL0 register, then configures the ADC12CTL0
 * register to turn on the ADC12, enables multiple sample and conversion, configures maximum sampling time and turns on the ADC12 internal reference. The configures the ADC12CTL1
 * register to use pulse sample mode and to read a sequence of channels. Then configures each ADC12MCTLx register to store the result of channel x, and to use the external reference
 * connected to +VeRef. On the last register (ADC12MCTL15), the end-of-sequence bit is also set. Then the ADC12ENC bit is set in the ADC12CTL0 register to enable the conversions.
 *
 * \param -
 *
 * \returns -
 */

void adc_config(void){

	P6SEL |= 0xff;                             			// select port 6 function as ADC
	P7SEL |= BIT4 + BIT5 + BIT6 + BIT7;					// select p7.4,p7.5,p7.6 and p7.7 function as ADC

	REFCTL0 &= ~REFMSTR;								// allows reference control in the ADC12 registers

	ADC12CTL0 = ADC12ON | ADC12MSC | ADC12SHT0_15 | ADC12SHT1_15 | ADC12REFON;      // turn on ADC12, enable multiple sample and conversion, extend sampling time to avoid overflow of results and turns on the internal reference
	ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1;       						// use sampling timer, sequence of channels
	ADC12MCTL0 = ADC12INCH_0 + ADC12SREF_2;             				// ref+=VeRef, channel = A0
	ADC12MCTL1 = ADC12INCH_1 + ADC12SREF_2;              				// ref+=VeRef, channel = A1
	ADC12MCTL2 = ADC12INCH_2 + ADC12SREF_2;              				// ref+=VeRef, channel = A2
	ADC12MCTL3 = ADC12INCH_3 + ADC12SREF_2;        						// ref+=VeRef, channel = A3
	ADC12MCTL4 = ADC12INCH_4 + ADC12SREF_2;        						// ref+=VeRef, channel = A4
	ADC12MCTL5 = ADC12INCH_5 + ADC12SREF_2;        						// ref+=VeRef, channel = A5
	ADC12MCTL6 = ADC12INCH_6 + ADC12SREF_2;        						// ref+=VeRef, channel = A6
	ADC12MCTL7 = ADC12INCH_7 + ADC12SREF_2;        						// ref+=VeRef, channel = A7
	ADC12MCTL8 = ADC12INCH_8 + ADC12SREF_2;        						// ref+=VeRef, channel = A8
	ADC12MCTL9 = ADC12INCH_9 + ADC12SREF_2;        						// ref+=VeRef, channel = A9
	ADC12MCTL10 = ADC12INCH_10 + ADC12SREF_2;        					// ref+=VeRef, channel = A10
	ADC12MCTL11 = ADC12INCH_11 + ADC12SREF_2;        					// ref+=VeRef, channel = A11
	ADC12MCTL12 = ADC12INCH_12 + ADC12SREF_2;        					// ref+=VeRef, channel = A12
	ADC12MCTL13 = ADC12INCH_13 + ADC12SREF_2;        					// ref+=VeRef, channel = A13
	ADC12MCTL14 = ADC12INCH_14 + ADC12SREF_2;        					// ref+=VeRef, channel = A14
	ADC12MCTL15 = ADC12INCH_15 + ADC12EOS + ADC12SREF_2;   				// ref+=VeRef, channel = A15
	ADC12CTL0 |= ADC12ENC;                    							// Enable conversions

}

/**
 * \brief Reads an ADC channel
 *
 * Clears the ADC12SC bit in the ADC12CTL0 and then sets it to start a conversion. Waits for the conversion of the selected channel to finish, then returns the result of the
 * conversion.
 *
 * \param channel is the channel to be read
 *
 * \returns The result of the conversion. Returns -1 if no channel could be read.
 */

int16_t adc_read(uint8_t channel){

	ADC12CTL0 &= ~ADC12SC;
	ADC12CTL0 |= ADC12SC;

	config_avoid_infinit_loops(1000);  // Maximum time on the loop: (TA2CCR0/clock): 1000/250000: 4ms
	while(!(ADC12IFG & (1 << channel)) && !avoid_infinit_loops());

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
