/*
 * eps_spi.c
 *
 *  Created on: 31/01/2017
 *      Author: Bruno Eiterer
 */

#include <msp430.h>
#include "SPI.h"

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

void spi_send(int data){
	while(!(UCA1IFG & UCTXIFG));
	UCA1TXBUF = data;	// send data to spi buffer
	while(!(UCA1IFG & UCRXIFG));
}

int spi_read(void){
	while(!(UCA1IFG & UCRXIFG));
	return UCA1RXBUF;
}


