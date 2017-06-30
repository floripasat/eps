#include <msp430.h> 
#include "clock.h"

int main(void) {
	  WDTCTL = WDTPW|WDTHOLD;                   // Stop WDT

	  P5DIR |= BIT4;
	  P5OUT |= BIT4;

	  clock_config();

	  TA1CTL |= TASSEL_2 + MC_1;
	  TA1CTL |= TACLR;

	  P3SEL |= BIT2 | BIT3;   // P3.2 and P3.3 option select
	  P3DIR |= BIT2 | BIT3;   // P3.2 and P3.3 outputs

	  TA1CCR0 = 160;                      // PWM Period
	  TA1CCTL1 = OUTMOD_7;                       // CCR2 reset/set
	  TA1CCR1 = 40;                  // CCR2 PWM duty cycle
	  TA1CCTL2 = OUTMOD_7;                       // CCR3 reset/set
	  TA1CCR2 = 40;                  // CCR3 PWM duty cycle

	  while(1){

	  __delay_cycles(120000000);

	  TA1CCR1 = 80;
	  TA1CCR2 = 80;

	  __delay_cycles(120000000);

	  TA1CCR1 = 120;
	  TA1CCR2 = 120;

	  __delay_cycles(120000000);

	  TA1CCR1 = 40;
	  TA1CCR2 = 40;

	  }
	
	return 0;
}
