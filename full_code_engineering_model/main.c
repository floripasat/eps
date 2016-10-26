#include <msp430.h> 
#include <eps_onewire.h>
#include <eps_i2c.h>
#include <eps_timer.h>
#include <eps_uart.h>
#include <watchdog.h>
#include <ADS1248.h>
#include <PID.h>
#include <stdlib.h>


void config_MSP430(void);


void main(void)
{
	config_MSP430();
	wdt_reset_counter();
	config_ADS1248(6);
	wdt_reset_counter();
	config_DS2775();
	__bis_SR_register(GIE);
	wdt_reset_counter();
	while(1);
}


void config_MSP430(void)
{
	P1DIR |= BIT6;
	P1OUT ^= BIT6;
	P3DIR |= BIT6;

	/*** clock configuration ***/
	BCSCTL1 = CALBC1_8MHZ;                      // Set DCO
	DCOCTL = CALDCO_8MHZ;					  	// Set DCO
	BCSCTL2 = DIVS_3;

	/*** WDT configuration ***/
	watchdog_setup(WATCHDOG,_188_mSEC);

	/*** SPI configuration ***/
	UCB1CTL0 |=  UCMSB + UCMST + UCSYNC;  				// 3-pin, 8-bit SPI master
	UCB1CTL1 |= UCSSEL_2;                     			// SMCLK
	UCB1BR0 |= 0x02;                          			// BRCLK = SMCLK/2
	UCB1BR1 = 0;                              			//
	UCB1CTL1 &= ~UCSWRST;                     			// **Initialize USCI state machine**
	P5OUT &= ~BIT0;                           			// reset slave - RST - active low
	P5OUT |= BIT0;                            			// Now with SPI signals initialized,

	/*** I2C Configuration ***/
	P3SEL |= 0x06;                            // Assign I2C pins to USCI_B0
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
	UCB0I2COA = 0x13;                         // Own Address is 0ABh
	UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
	IE2 |= UCB0TXIE;                          // Enable TX interrupt

	/*** UART Configuration ***/
	P3OUT |= BIT4;
//	P3SEL |= 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 65;                            // 1MHz 9600; (104)decimal = 0x068h
	UCA0BR1 = 3;                              // 1MHz 9600
	UCA0MCTL |= UCBRS0;                        // Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

	/*** timer configuration ***/
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 =50000;							    // timer A capture and compare register
	TACTL = TASSEL_2 + MC_3;                   // SMCLK, contmode

	/*** ADS1248 configuration ***/
	P5DIR |= BIT0 + BIT4;
	P5OUT = BIT0;                             // Set slave reset - P3.
	P5SEL |= 0x0E;                            // P5.1,2,3 USCI_B1 option select
	P4DIR |= BIT6;
	P4OUT |= BIT6;

}


