#include <msp430.h> 
#include <stdlib.h>
#include "eps_onewire.h"
#include "eps_i2c.h"
#include "eps_timer.h"
#include "eps_uart.h"
#include "ADS1248.h"
#include "pid.h"



void vconfig_msp430(void);


void main(void){

	WDTCTL = WDTPW + WDTHOLD;

	vconfig_msp430();
	config_ADS1248(6);
	config_DS2775();
	__bis_SR_register(GIE);
	while(1);
}


/**********************************
 * Function name: config_msp430
 *
 * Arguments: void
 *
 * Returns: void
 *
 */


void vconfig_msp430(void){

	UCSCTL4 |= SELA_2;	// SELA_2: ACLK source is REFOCLK (32768Hz)

	/* Timer A0 configuration
	 * Interrupt period: 1s
	 */

	P1DIR |= 0x01;                          // P1.0 output
	TA0CCR0 = 32768;						// timer A0 CCR0 interrupt period = 32768 * 1/32768 = 1s
	TA0CCTL0 = CCIE;                        // timer A0 CCR0 interrupt enabled
	TA0CTL = TASSEL_1 + MC_1 + TACLR;       // SMCLK, upmode, timer A interrupt enable, clear TAR

	/* Timer A0 configuration
	 * Interrupt period: 100.006ms
	 */

	P3DIR |= 0x01;							// P3.0 output
	TA1CCR0 = 3277;							// timer A1 CCR0 interrupt period = 3277 * 1/32768 = 100.006ms
	TA1CCTL0 = CCIE;						// timer A1 CCR0 interrupt enabled
	TA1CTL = TASSEL_1 + MC_1 + TACLR;       // SMCLK, upmode, timer A interrupt enable, clear TAR
/*

	P1DIR |= BIT6;
	P1OUT ^= BIT6;
	P3DIR |= BIT6;

	*** clock configuration ***
	BCSCTL1 = 0x8D;                      		// Set DCO
	DCOCTL = 0x89;					  			// Set DCO
	BCSCTL2 = DIVS_3;							// Set SMCLK = DCO/8 = 1MHz

	*** SPI configuration ***
	UCB1CTL0 |=  UCMSB + UCMST + UCSYNC;  				// 3-pin, 8-bit SPI master
	UCB1CTL1 |= UCSSEL_2;                     			// SMCLK
	UCB1BR0 |= 0x02;                          			// BRCLK = SMCLK/2
	UCB1BR1 = 0;                              			//
	UCB1CTL1 &= ~UCSWRST;                     			// **Initialize USCI state machine**
	P5OUT &= ~BIT0;                           			// reset slave - RST - active low
	P5OUT |= BIT0;                            			// Now with SPI signals initialized,

	*** I2C Configuration ***
	P3SEL |= 0x06;                            // Assign I2C pins to USCI_B0
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
	UCB0I2COA = 0x13;                         // Own Address is 0ABh
	UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
	IE2 |= UCB0TXIE;                          // Enable TX interrupt

	*** UART Configuration ***

	P3SEL |= 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 65;                            // 1MHz 9600; (104)decimal = 0x068h
	UCA0BR1 = 3;                              // 1MHz 9600
	UCA0MCTL |= UCBRS0;                        // Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**


	*** ADS1248 configuration ***
	P5DIR |= BIT0 + BIT4;
	P5OUT = BIT0;                             // Set slave reset - P3.
	P5SEL |= 0x0E;                            // P5.1,2,3 USCI_B1 option select
	P4DIR |= BIT6;
	P4OUT |= BIT6;

	*/

	__bis_SR_register(GIE);       // enable interrupts

}


