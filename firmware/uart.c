#include <msp430.h>
#include <stdio.h>
#include <intrinsics.h>
#include "uart.h"

void uart_config(){

	port_mapping();

	P9SEL |= 0x0C;                            // Assign P9.2 to UCA2TXD and P9.3 to UCA2RXD
	UCA2CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA2CTL1 |= UCSSEL_1;                     // CLK = SMCLK
	UCA2BR0 = 0x03;                           // baud rate selection 32768/UCA2BRO=9600 => UCA0BRO = 3
	UCA2BR1 = 0x00;
	UCA2MCTL = UCBRS_3|UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
	UCA2CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

	P2SEL |= BIT4;                            // Assign P2.4 to UCA0TXD
	P2DIR |= BIT4;							  // set P2.4 as output
	UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA0CTL1 |= UCSSEL_1;                     // CLK = SMCLK
	UCA0BR0 = 0x03;                           // baud rate selection 32768/UCA2BRO=9600 => UCA0BRO = 3
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRS_3|UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**


}

void uart_tx_debug(uint8_t *tx_data){

	uint8_t i = 0;				  	      // used to determine when array is finished
	while(tx_data[i])                     // Increment through array, look for null pointer (0) at end of string
	{
		while ((UCA2STAT & UCBUSY));      // Wait if line TX/RX module is busy with data
		UCA2TXBUF = tx_data[i];           // Send out element i of tx_data array on UART bus
		i++;                              // Increment variable for array address
	}

}


void uart_tx_beacon(uint8_t tx_data){

	while ((UCA0STAT & UCBUSY));      // Wait if line TX/RX module is busy with data
	UCA0TXBUF = tx_data;           // Send out element i of tx_data array on UART bus

}


void port_mapping(void){

	PMAPPWD = 0x02D52;		// Enable Write-access to modify port mapping registers

	P2MAP4 = PM_UCA0TXD;

	PMAPPWD = 0;                              // Disable Write-Access to modify port mapping registers


}

void float_send(float c){

    volatile long int d;
	volatile  unsigned int hundreds, tens, units, tenths, hundredths, thousandths, tenthousandths,thousandth, ten_thousandths;
	volatile long int remainder;
    unsigned char string[30];

    c *= 10000;
    d = (long int)c;
    tens = d/100000;
    remainder =d - tens*100000;
    units = remainder/10000;
    remainder = remainder - units*10000;
	tenths = remainder/1000;
    remainder = remainder - tenths*1000;
    hundredths = remainder/100;
    remainder = remainder - hundredths *100;
    thousandth = remainder/10;
	remainder = remainder -thousandth*10;
	ten_thousandths=remainder;
    sprintf(string, "%d%d.%d%d%d%d", tens, units, tenths, hundredths,thousandth,ten_thousandths);
	uart_tx_debug(string);

}

