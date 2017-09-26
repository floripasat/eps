#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR |= BIT3;
    P1DIR |= BIT6;

    while(1){
        __delay_cycles(20*1045000);		// delay for 20 seconds
        P1OUT |= BIT3;					// set transistor base
        P1OUT |= BIT6;					// set transistor base
        __delay_cycles(104500);			// delay for 0.1 seconds
        P1OUT &= ~BIT3;					// pull transistor base low
        P1OUT &= ~BIT6;					// puul transistor base low
    }


	return 0;
}
