/*
 * timer.c
 *
 *  Created on: 26/09/2017
 *      Author: Bruno Eiterer
 */
#include "timer.h"

void timerConfig(void){
	TA0CCTL0 = CCIE;                          					// CCR0 interrupt enabled
	TA0CCR0 = 65513;											// 65513*2/(1045000/8) = 1 second
	TA0CTL = TASSEL__SMCLK + MC__UPDOWN + ID__8 + TACLR;        // SMCLK, updown mode, divide clock by 8, clear TAR

	TA1CCTL0 = CCIE;
	TA1CCR0 = 65513;
	TA1CTL = TASSEL__SMCLK + MC__UPDOWN + ID__8 + TACLR;        // SMCLK, updown mode, divide clock by 8, clear TAR
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){
	volatile static uint8_t counter10seconds = 0;

	if(counter10seconds == 9){							// verify if 10 seconds have passed
		counter10seconds = 0;							// reset counter
		P1OUT |= BIT3;                          // Set transistor base
		__delay_cycles(0.6*1045000);			// delay for 0.6 seconds
		P1OUT &= ~BIT3;							// turn off transistor base
	}
	else{
		counter10seconds++;		// increment counter
	}
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void){
	volatile static uint8_t counter30seconds = 0;

	__enable_interrupt();

	if(counter30seconds == 59){							// verify if 60 seconds have passed
		counter30seconds = 0;							// reset counter
		P1OUT |= BIT4;                          // Set transistor base
		__delay_cycles(2*1045000);			// delay for 2 seconds
		P1OUT &= ~BIT4;							// turn off transistor base
	}
	else{
		counter30seconds++;		// increment counter
	}
}
