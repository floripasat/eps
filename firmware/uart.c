/**
 * \file
 *
 * \brief UART driver source
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
 *
 */

#include <msp430.h>
#include <stdio.h>
#include <intrinsics.h>
#include "uart.h"
#include "avoid_infinit_loops.h"

/**
 * \brief Configures the UART peripheral
 *
 * The first step is to call port_mapping.
 *
 * The first interface configuration done is the UCA2, used for debugging. First it assigns P9.2 to UCA2TXD
 * and P9.3 to UCA2RXD. Then it sets the UCSWRST bit in the UCA2CTL1 register, allowing the configuration
 * to start. Then it selects the ACLK (16 MHz) clock source in the UCA2CTL1 register. The baud rate is
 * selected in the UCA2BR0 and UCA2BR1 registers, with values of 0x8A and 0x00, respectively. This results
 * in 16000000/138 = ~115942 bps. Then the modulation is set in the  UCA2MCTL, with UCBRS_7 + UCBRF_0,
 * resulting in a baud rate close of 115200 bps. To finish the configuration the UCSWRST bit is cleared in
 * the UCA2CTL1 register, resuming normal operation.
 *
 * The second interface configuration done is the UCA0, used to communicate with Beacon. First it assigns
 * P2.4 to UCA0TXD, then it sets the UCSWRST bit in the UCA0CTL1 register. The clock source used is also
 * ACLK (16 MHz). The baud rate registers UCA0BR0 and UCA0BR1 are set wth 0xD0 and 0x00, respectively.
 * In the UCA0MCTL register the modulation select is UCBRS_0 and UCBRF_5 with oversampling defined by
 * UCOS16, resulting in a baud rate of ~4800 bps. Then it clears the UCSWRST bit as well, returning to
 * normal operation.
 *
 * \param -
 *
 * \returns -
 */

void uart_config(){

	port_mapping();

	P9SEL |= 0x0C;                            // Assign P9.2 to UCA2TXD and P9.3 to UCA2RXD
	UCA2CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA2CTL1 |= UCSSEL__ACLK;                 // CLK = ACLK
	UCA2BR0 = 0x8A;                           // baud rate selection 16000000/UCA2BRO=115200 => UCA0BRO = 138
	UCA2BR1 = 0x00;
	UCA2MCTL = UCBRS_7|UCBRF_0;               // Modulation UCBRSx=7, UCBRFx=0
	UCA2CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

	P2SEL |= BIT4;                            // Assign P2.4 to UCA0TXD
	P2DIR |= BIT4;							  // set P2.4 as output
	UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA0CTL1 |= UCSSEL__ACLK;                 // CLK = ACLK
	UCA0BR0 = 0xD0;                           // baud rate selection 32768/UCA2BRO=9600 => UCA0BRO = 3
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRS_0|UCBRF_5|UCOS16;        // Modulation UCBRSx=3, UCBRFx=0
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**


}

/**
 * \brief Sends data to the debug port
 *
 * This function sends a byte of data to the debug port. Before each bit is transmitted, it checks
 * if the line is busy through the UCBUSY bit in the UCA2STAT register. Then it sends the bit
 * by putting it in the UCA2TXBUF register. It loops until the end of the data is reached.
 *
 * \param tx_data is a pointer to the data to be sent.
 *
 * \returns -
 */

void uart_tx_debug(uint8_t *tx_data){

	uint8_t i = 0;				  	      // used to determine when array is finished
	config_avoid_infinit_loops(1000);  // Maximum time on the loop: (TA2CCR0/clock): 1000/250000: 4ms
	while(tx_data[i] && !avoid_infinit_loops())  // Increment through array, look for null pointer (0) at end of string
	{
	    config_avoid_infinit_loops(1000);
		while ((UCA2STAT & UCBUSY) && !avoid_infinit_loops());      // Wait if line TX/RX module is busy with data
		UCA2TXBUF = tx_data[i];           // Send out element i of tx_data array on UART bus
		i++;                              // Increment variable for array address
	}
}

/**
 * \brief Sends floating point data to the debug port
 *
 * Sends a byte of data to Beacon. First it checks if the line is busy, using the
 * UCBUSY bit in the UCA0STAT register. Then it places the data to be sent in the
 * UCA0TXBUF register.
 *
 * \param tx_data is an unsigned integer of 8 bits, containing the data to be sent
 * to Beacon
 *
 * \returns -
 */


void uart_tx_beacon(uint8_t tx_data){

    config_avoid_infinit_loops(1000);
	while ((UCA0STAT & UCBUSY) && !avoid_infinit_loops());      // Wait if line TX/RX module is busy with data
	UCA0TXBUF = tx_data;           // Send out element i of tx_data array on UART bus

}

/**
 * \brief Maps multi-function port 2 pins to UART
 *
 * First it enables write-access to the port mapping registers in the PMAPPWD register,
 * then it chooses the UCA0TXD function in the P2MAP4 register, then it disables
 * write-access to port mapping registers.
 *
 * \param -
 *
 * \returns -
 */

void port_mapping(void){

	PMAPPWD = 0x02D52;		// Enable Write-access to modify port mapping registers

	P2MAP4 = PM_UCA0TXD;

	PMAPPWD = 0;                              // Disable Write-Access to modify port mapping registers


}

/**
 * \brief Sends floating point data to the debug port
 *
 * First it verifies if the floating point number passed is negative, if it is it sends a "-" character,
 * then inverts the number. Then it converts the number to a string, and passes the string to
 * uart_tx_debug.
 *
 * \param c is the floating point number to be sent via UART.
 *
 * \returns -
 */

void float_send(volatile float c){
    uint8_t string[30];

    if(c < 0){
    	uart_tx_debug("-");
    	c = -c;
    }

//    sprintf(string, "%f", c);     TODO: Remove printf function
	uart_tx_debug(string);
}
