/*
 * eps_spi.c
 *
 *  Created on: 31/01/2017
 *      Author: Bruno Eiterer
 */

#include <msp430.h>
#include "SPI.h"

void spi_send(int data)
{
	UCB1TXBUF = data;	// send data to spi buffer
}

int spi_read(void)
{
	return UCB1RXBUF;
}


