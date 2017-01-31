#include <msp430.h>
#include "test_PWM.h"
#include "eps_ADC.h"


/*
 * main.c
 */

#define Rsense 0.05
#define Gain 0.025
#define Rout 1650
#define increase 1
#define decrease 0

volatile unsigned int cont_20m = 0;
volatile unsigned int cont_100m = 0;
volatile float voltage = 0;
volatile float current = 0;
volatile unsigned int action = 0;
volatile float power = 0;
volatile float previous_power = 0;

void config_MSP430(void);
void mppt(void);

void main(void) {

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	config_MSP430();
	while(1);
}


void config_MSP430(void)
{
	BCSCTL1 = 	CALBC1_8MHZ;                    // Set DCO
	DCOCTL = CALDCO_8MHZ;					  // Set DCO

	P6SEL = 0xff;

	P1DIR |= BIT3;                            	// P4.3 output
	P1SEL |= BIT3;                            	// P4.3 timer pwm
	TACCR0 = PWM_PERIOD/2;                      // PWM Period/2
	TACCTL2 = OUTMOD_6;                         // CCR2 toggle/set
	TACCR2 = PWM_DUTY_CYCLE*PWM_PERIOD/2;       // CCR2 PWM duty cycle
	TACTL = TASSEL_2 + MC_3;                  	// SMCLK, up-down mode

	P3DIR |= BIT4 + BIT5;					  // used to debug timer
	TBCCTL0 = CCIE;                             // TBCCR0 interrupt enabled
	TBCCR0 = 40000;							  // timer B capture and compare register
	TBCTL = TBSSEL_2 + MC_3;                  // SMCLK, contmode

	ADC12CTL0 = ADC12ON + MSC + SHT0_15 + REFON + REF2_5V;  		// Turn on ADC12, set sampling time, 16 ADC12CLK cycles, Vref=2.5V
	ADC12CTL1 = SHP + CONSEQ_1 + ADC12SSEL_3;										// sampling signal source is sampling timer, conversion mode is sequence of channels
	ADC12MCTL0 = INCH_0+SREF_1;                   					// ref+=VREF+, channel = A0
	ADC12MCTL1 = INCH_1+SREF_1;                   					// ref+=VREF+, channel = A1
	ADC12MCTL2 = INCH_2+SREF_1;                   					// ref+=VREF+, channel = A2
	ADC12MCTL3 = INCH_3+SREF_1;                   					// ref+=VREF+, channel = A3
	ADC12MCTL4 = INCH_4+SREF_1;                   					// ref+=VREF+, channel = A4
	ADC12MCTL5 = INCH_5+SREF_1;              						// ref+=VREF+, channel = A5
	ADC12MCTL6 = INCH_10+SREF_1+EOS;                				// ref+=VREF+, channel = temperature diode, end of sequence
	ADC12CTL0 |= ENC;                       						// Enable conversions

	__bis_SR_register(GIE);					  						// enables interrupts
}

#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
	if(cont_20m == 1)										// period = CCR0 * 2 * cont_20m / clock => 20m = 40000*2*cont_20m/(8*10^6) => cont = 1 (starts at 0)
	{
		P3OUT ^= BIT4;										// toggle P3.4 to determine period of timer (20ms)

		measurement_ADC();									// read solar panels values

		voltage += ((ADC12MEM0*0.000610351*1.93) + 0.095);								// add voltage to calculate mean value
		current += ((ADC12MEM1*0.000610351)/(Rsense*Gain*Rout) + 0.015);			    // add current to calculate mean value

		cont_20m = 0;										// reset cont_20m
	}
	else
	{
		cont_20m++;								// increments cont_20m to achieve desired output period
	}
	if(cont_100m == 19)							// period = CCR0 * 2 * cont_100m / clock => 200m = 40000*2*cont_100m/(8*10^6) => cont = 19 (starts at 0)
	{
		P3OUT ^= BIT5;							// toggle P3.5 to determine period of timer (200ms)
		voltage = voltage/10;					// take mean of 10 values of voltage
		current = current/10;					// take mean of 10 values of current

		mppt();

		voltage = 0;
		current = 0;

		cont_100m = 0;							// reset cont_100m
	}
	else
	{
		cont_100m++;							// increments cont_100m to achieve desired output period
	}
}

void mppt(void)
{
	power = voltage*current;								// take input power

	if(power > previous_power && action == increase)		// verify action to be made
	{
		action = increase;									// set test bit for next iteration
	   	TACCR2 = (PWM_DUTY_CYCLE+0.05)*PWM_PERIOD/2;		// increase duty cycle
	}
	else if(power > previous_power && action == decrease)	// verify action to be made
	{
		action = decrease;									// set test bit for next iteration
	   	TACCR2 = (PWM_DUTY_CYCLE-0.05)*PWM_PERIOD/2;		// decrease duty cycle
	}
	else if(power < previous_power && action == increase)	// verify action to be made
	{
		action = decrease;									// set test bit for next iteration
	   	TACCR2 = (PWM_DUTY_CYCLE-0.05)*PWM_PERIOD/2;		// decrease duty cycle
	}
	else if(power < previous_power && action == decrease)	// verify action to be made
	{
		action = increase;									// set test bit for next iteration
	   	TACCR2 = (PWM_DUTY_CYCLE+0.05)*PWM_PERIOD/2;		// increase duty cycle
	}

	previous_power = power;									// save power value for next iteration
}
