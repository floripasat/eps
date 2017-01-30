#include <msp430.h>
#include "ADS1248.h"

volatile unsigned int w=0;
volatile long temp[]= {0,0,0};
volatile unsigned int i=0;


void config_ADS1248(int channel)
{
	int data[] = {SDATAC_command,WREG_command,0x03,(channel << 3 | 0x01),0x00,0x20,0x33,WREG_command + 0x0A,0x01,0x02,0x60,RREG_command,0x03,
			NOP_command,NOP_command,NOP_command,NOP_command,RREG_command + 0x0A,0x01,NOP_command,NOP_command};
	for (i = 50; i > 0; i--);                	 			// Wait for slave to initialize
	UCB1TXBUF = RESET_command;                     		    // Send reset command to ensure device is properly powered on
	__delay_cycles(4800);									// delay after reset of 0.6ms

	chip_select_port &= ~BIT4;		// pull chip select low to start communication

	for(w=0;w < 21;w++)		// send all initialization commands/data
	{
		UCB1TXBUF = data[w];
		for (i = 500; i; i--);                  // Add time between transmissions to make sure slave can keep up
	}

	if(w == 21)
	{
		chip_select_port |= BIT4;							// pull chip select high after communication is done
	}
}
void read_ADS1248(int channel)
{

	/* configure positive input channel to read */

	chip_select_port &= ~BIT4;						// pull chip select low to start communication
	UCB1TXBUF = SDATAC_command;			// send stop read data continuous mode command
	for (i = 500; i; i--);				// Add time between transmissions to make sure slave can keep up
	UCB1TXBUF = WREG_command;			// send write to register command
	for (i = 500; i; i--);				// Add time between transmissions to make sure slave can keep up
	UCB1TXBUF = 0x00;					// write 1 data byte to 1 register
	for (i = 500; i; i--);				// Add time between transmissions to make sure slave can keep up
	UCB1TXBUF = (channel << 3) | 0x01;	// write positive input channel value ( channel << 3) and negative input channel value (0x01)
	for (i = 500; i; i--);				// Add time between transmissions to make sure slave can keep up

	/* start conversion and read outputs */

	UCB1TXBUF = SYNC_command;			// send command to start conversion
	__delay_cycles(7);					// delay of 7 tclk
	chip_select_port |= BIT4;						// pull chip select high after communication is done

	__delay_cycles(250000);				// dealy to wait the conversion
	chip_select_port &= ~BIT4;						// pull chips select low to start communication
	__delay_cycles(7);					// delay of 7 tclk
	UCB1TXBUF = RDATA_command;			// send command to read conversion result
	for (i = 500; i; i--);              // Add time between transmissions to make sure slave can keep up

	UCB1TXBUF = NOP_command;			// send dummy command to send clock and read conversion results
	for (i = 500; i; i--);              // Add time between transmissions to make sure slave can keep up
	temp[0] = UCB1RXBUF;				// save output to variable

	UCB1TXBUF = NOP_command;			// send dummy command to send clock and read conversion results
	for (i = 500; i; i--);              // Add time between transmissions to make sure slave can keep up
	temp[1] = UCB1RXBUF;				// save output to variable

	UCB1TXBUF = NOP_command;			// send dummy command to send clock and read conversion results
	for (i = 500; i; i--);              // Add time between transmissions to make sure slave can keep up
	temp[2] = UCB1RXBUF;				// save output to variable

	__delay_cycles(7);					// delay of 7 tclk
	chip_select_port |= BIT4;						// pull chip select high after communcation is done


}
