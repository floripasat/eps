#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    P1DIR |= BIT6;
    P1OUT |= BIT6;

    P4DIR &= ~(BIT1 + BIT2 + BIT3);

    while(1);
	
	return 0;
}
