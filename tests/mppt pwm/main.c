#include <msp430.h> 
#include "clock.h"

int main(void) {
	  WDTCTL = WDTPW|WDTHOLD;                   // Stop WDT

	  P5DIR |= BIT4;
	  P5OUT |= BIT4;

	  clock_config();

	  TBCTL |= TBSSEL_2 + MC_1;
	  TBCTL |= TBCLR;

	  P4SEL |= BIT1 | BIT2 | BIT3;   // P4.1, P4.2 and P4.3 option select
	  P4DIR |= BIT1 | BIT2 | BIT3;   // P4.1, P4.2 and P4.3 outputs

	  TBCCR0 = 16;                      		// PWM Period
	  TBCCTL1 = OUTMOD_7;                       // CCR1 reset/set
	  TBCCR1 = 4;                  				// CCR1 PWM duty cycle
	  TBCCTL2 = OUTMOD_7;                       // CCR2 reset/set
	  TBCCR2 = 4;                  				// CCR2 PWM duty cycle
	  TBCCTL3 = OUTMOD_7;						// CCR3 reset/set
	  TBCCR3 = 4;								// CCR3 PWM duty cycle

	  while(1){

	  __delay_cycles(120000000);

	  TBCCR1 = 8;
	  TBCCR2 = 8;
	  TBCCR3 = 8;

	  __delay_cycles(120000000);

	  TBCCR1 = 12;
	  TBCCR2 = 12;
	  TBCCR3 = 12;

	  __delay_cycles(120000000);

	  TBCCR1 = 4;
	  TBCCR2 = 4;
	  TBCCR3 = 4;

	  }
	
	return 0;
}
