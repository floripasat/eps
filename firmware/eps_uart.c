#include "eps_uart.h"

#include <msp430.h>
#include <stdio.h>
#include <intrinsics.h>

void uart_tx(uint8_t *tx_data)            // Define a function which accepts a character pointer to an array
{
	uint8_t i = 0;				  	      // used to determine when array is finished
	while(tx_data[i])                     // Increment through array, look for null pointer (0) at end of string
	{
		while ((UCA2STAT & UCBUSY));      // Wait if line TX/RX module is busy with data
		UCA2TXBUF = tx_data[i];           // Send out element i of tx_data array on UART bus
		i++;                              // Increment variable for array address
	}
}

