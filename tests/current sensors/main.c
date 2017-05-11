#include <msp430.h> 

/*
 * main.c
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    P1DIR |= BIT6;
    P1OUT &= ~BIT6;		// disable 555

    P5DIR |= BIT4;
    P5OUT |= BIT4;		// turn on system_on led

    P4DIR |= BIT1 + BIT2 + BIT3;
    P4OUT &= ~(BIT1 + BIT2 + BIT3);		// turn off all MPPT mosfets

	while(1);
}
