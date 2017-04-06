#include <msp430.h> 
#include <stdlib.h>
#include "ADC.h"
#include "ADS1248.h"
#include "I2C.h"
#include "onewire.h"
#include "pid.h"
#include "timer.h"
#include "uart.h"
#include "SPI.h"
#include "clock.h"



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

//	UCSCTL4 |= SELA_2 + SELS_3;	// SELA_2: ACLK source is REFOCLK (32768Hz), SELS_3: SMCL source is DCOCLK (1.045MHz)

	clock_config();

	uart_config();

    P9DIR |= BIT2;                 // P5.3 set as led system output

    while(1) {
        P9OUT ^= BIT2;                          // Toggle LED_SYSTEM
        __delay_cycles(16);
    }

	#ifdef _DEBUG
		uart_tx("system booting\r\n");
	#endif

	timer_config();		// call timer configuration function

	adc_config();		// call MSP ADC configuration function

	spi_config();		// call SPI configuration function
/*


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


