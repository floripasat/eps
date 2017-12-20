#include <msp430.h>
#include "timer.h"

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    timerConfig();
	
    P1DIR |= BIT4;
    P1DIR |= BIT3;

    __enable_interrupt();

    while(1);

	return 0;
}
