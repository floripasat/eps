#include <msp430.h>
#include <stdio.h>
#include <intrinsics.h>
#include "uart.h"

void uart_config(){

	P9SEL |= 0x0C;                            // Assign P9.2 to UCA2TXD and P9.3 to UCA2RXD

	UCA2CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA2CTL1 |= UCSSEL_1;                     // CLK = SMCLK
	UCA2BR0 = 0x03;                           // baud rate selection 32768/UCA2BRO=9600 => UCA0BRO = 3
	UCA2BR1 = 0x00;
	UCA2MCTL = UCBRS_3|UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
	UCA2CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

void uart_tx(uint8_t *tx_data){

	uint8_t i = 0;				  	      // used to determine when array is finished
	while(tx_data[i])                     // Increment through array, look for null pointer (0) at end of string
	{
		while ((UCA2STAT & UCBUSY));      // Wait if line TX/RX module is busy with data
		UCA2TXBUF = tx_data[i];           // Send out element i of tx_data array on UART bus
		i++;                              // Increment variable for array address
	}
}

