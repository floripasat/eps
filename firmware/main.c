#include <msp430.h> 
#include <stdlib.h>
#include "eps_onewire.h"
#include "eps_i2c.h"
#include "ADC.h"
#include "ADS1248.h"
#include "pid.h"
#include "timer.h"
#include "uart.h"
#include "SPI.h"



void config_msp430(void);


void main(void){

 	WDTCTL = WDTPW + WDTHOLD;

	config_msp430();
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


void config_msp430(void){

	/*Clock Configuration:
	 * MCKL = default DCO = 1.045MHz
	 * SMCKL = default DCO = 1.045MHz
	 * ACKL = REFOCLK = 32.768kHz
	 */
	UCSCTL4 |= SELA_2 + SELS_3;	// SELA_2: ACLK source is REFOCLK (32768Hz), SELS_3: SMCL source is DCOCLK (1.045MHz)

	uart_config();

	#ifdef _DEBUG
		uart_tx("system booting\r\n");
	#endif

	timer_config();		// call timer configuration function

	adc_config();		// call MSP ADC configuration function

	spi_config();		// call SPI configuration function
/*

	P1DIR |= BIT6;
	P1OUT ^= BIT6;
	P3DIR |= BIT6;

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

	*** ADS1248 configuration ***
	P5DIR |= BIT0 + BIT4;
	P5OUT = BIT0;                             // Set slave reset - P3.
	P5SEL |= 0x0E;                            // P5.1,2,3 USCI_B1 option select
	P4DIR |= BIT6;
	P4OUT |= BIT6;

	*/
	__bis_SR_register(GIE);       // enable interrupts

	#ifdef _DEBUG
		uart_tx("system boot complete\r\n");
	#endif
}


