/**
 * \file
 *
 * \brief SPI driver source
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "SPI.h"
#include "avoid_infinit_loops.h"

/**
 * \brief Configures the SPI peripheral
 *
 * Configures port 8 pins 1, 2 and 3 as SPI CLK, MOSI and MISO, repectively. Then configures port 8 pins 1, 2, 4, 5, 6 as output. Pin 4 is used as the ADS1248 reset, pin 5 as the
 * ADS1248 reset and pin 6 as the ADS1248 start. Then sets the UCWRST bit in the UCA1CTL1 register to allow configuration of the UCA1. Then configures it in the UCA1CTL0 register as
 * 3-pin SPI in master mode. Also sets the UCSYNC bit to signal a synchronous protocol. Then selects the clock source as the SMCLK (32 MHz), and configures UCA1BR0 as 0x08 and
 * UCA1BR1 and UCA1MCTL as 0x00, resulting in 4 a MHz clock for the SPI peripheral. Then clears the UCSWRST bit in the UCA1CTL1 register to resume normal operation.
 *
 * \param -
 *
 * \returns -
 */

void spi_config(void){
    P8SEL |= BIT1 + BIT2 + BIT3;
    P8DIR |= BIT1 + BIT2 + BIT4 + BIT5 + BIT6;

    UCA1CTL1 |= UCSWRST;
	UCA1CTL0 |=  UCMSB + UCMST + UCSYNC;  				// 3-pin, 8-bit SPI master
	UCA1CTL1 |= UCSSEL_2;                     			// SMCLK
	UCA1BR0 |= 0x08;                          			// BRCLK = SMCLK/2
	UCA1BR1 = 0;                              			//
	UCA1MCTL = 0;
	UCA1CTL1 &= ~UCSWRST;                     			// **Initialize USCI state machine**
}

/**
 * \brief Sends data via the SPI peripheral
 *
 * Waits for the TX interrupt flag to be set, indicating that the peripheral is available to be used. Then puts the data to be sent in the TX buffer.
 *
 * \param data is an 8 bit integer to be sent via SPI
 *
 * \returns -
 */

void spi_send(int data){

    config_avoid_infinit_loops(1000);  // Maximum time on the loop: (TA2CCR0/clock): 1000/250000: 4ms
	while(!(UCA1IFG & UCTXIFG) && !avoid_infinit_loops());
	UCA1TXBUF = data;	// send data to spi buffer
	config_avoid_infinit_loops(1000);
	while(!(UCA1IFG & UCRXIFG) && !avoid_infinit_loops());
}

/**
 * \brief Reads data from the SPI peripheral
 *
 * Waits until the RX flag is set, indicating data has been received. Then returns the value in the UCA1RXBUF register.
 *
 * \param -
 *
 * \returns The value received in the UCA1RXBUF
 */

int spi_read(void){

    config_avoid_infinit_loops(1000);
	while(!(UCA1IFG & UCRXIFG) && !avoid_infinit_loops());
	return UCA1RXBUF;
}


