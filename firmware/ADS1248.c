#include <msp430.h>
#include "ADS1248.h"
#include <stdint.h>
#include "SPI.h"

/*
 * ADS1248 initialization procedure: - send RESET command
 * 									 - wait 0.6ms
 * 									 - send stop read data continous mode (SDATC) command
 * 									 - send write register (WREG) command + 0 offset (will start writing to register MUX0 at address 0x00)
 * 									 - send number of bytes to write - 1 (MUX0 register at address 0x00, Vbias register at address 0x01, MUX1 register at address 0x02h and SYS0 register at 0x03)
 * 									 - send value to be written on register MUX0: turn off burn-out current source and configure channel to be read
 * 									 - send value to be written on register Vbias: disable Vbias
 * 									 - send value to be written on MUX1 register: turn internal reference generator always on, select input reference and normal operation mode
 * 									 - send value to be written on SYS0 register: select PGA gain and data output rate
 * 									 - send write register (WREG) command + 0x0A offset (will start writing to register IDAC0 at address 0x0A)
 * 									 - send number of bytes to write - 1 (IDAC0 register at address 0x0A, IDAC1 register at address 0x0B)
 * 									 - send value to be written on IDAC0 register: set DRDY mode and magnitude of excitation current
 * 									 - send value to be written on IDAC1 register: select output pin of excitation currents
 */

void config_ADS1248(int positive_channel)
{
	#ifdef _DEBUG
		volatile uint8_t initialization_data_sent_back_counter = 0;
		volatile uint8_t initialization_data_sent_back[6] = {0};
	#endif
	volatile unsigned int initialization_data_counter = 0;
	volatile unsigned int i=0;
	const int initialization_data[] = {SDATAC_command,WREG_command,0x03,(positive_channel << 3 | negative_channel),0x00,0x20,0x33,WREG_command + 0x0A,0x01,0x02,0x60,
			RREG_command,0x03,NOP_command,NOP_command,NOP_command,NOP_command,RREG_command + 0x0A,0x01,NOP_command,NOP_command};

	spi_send(RESET_command);                     		    // Send reset command to ensure device is properly powered on
	__delay_cycles(4800);									// delay after reset of 0.6ms

	chip_select_port &= ~chip_select_pin;					// pull chip select low to start communication

	for(initialization_data_counter=0;initialization_data_counter < 21;initialization_data_counter++)										// send all initialization commands/data
	{
		spi_send(initialization_data[initialization_data_counter]);
		for(i = 500; i; i--);                  // Add time between transmissions to make sure slave can keep up
	#ifdef _DEBUG
		switch(initialization_data_counter){
		case 13:
			initialization_data_sent_back[initialization_data_sent_back_counter] = UCB1RXBUF;
			initialization_data_sent_back_counter++;
			break;
		case 14:
			initialization_data_sent_back[initialization_data_sent_back_counter] = UCB1RXBUF;
			initialization_data_sent_back_counter++;
			break;
		case 15:
			initialization_data_sent_back[initialization_data_sent_back_counter] = UCB1RXBUF;
			initialization_data_sent_back_counter++;
			break;
		case 16:
			initialization_data_sent_back[initialization_data_sent_back_counter] = UCB1RXBUF;
			initialization_data_sent_back_counter++;
			break;
		case 19:
			initialization_data_sent_back[initialization_data_sent_back_counter] = UCB1RXBUF;
			initialization_data_sent_back_counter++;
			break;
		case 20:
			initialization_data_sent_back[initialization_data_sent_back_counter] = UCB1RXBUF;
			initialization_data_sent_back_counter++;
			break;
		default:
			break;
		}
	#endif
	}

	if(initialization_data_counter == 21)
	{
		chip_select_port |= chip_select_pin;							// pull chip select high after communication is done
	}
}
long read_ADS1248(int channel)
{
	volatile unsigned int i=0;
	volatile unsigned long temp = 0;

	/* configure positive input channel to read */

	chip_select_port &= ~chip_select_pin;		// pull chip select low to start communication
	spi_send(SDATAC_command);					// send stop read data continuous mode command
	for (i = 500; i; i--);						// Add time between transmissions to make sure slave can keep up
	spi_send(WREG_command);					// send write to register command
	for (i = 500; i; i--);						// Add time between transmissions to make sure slave can keep up
	spi_send(0x00);							// write 1 data byte to 1 register
	for (i = 500; i; i--);						// Add time between transmissions to make sure slave can keep up
	spi_send((channel << 3) | 0x01);			// write positive input channel value (channel << 3) and negative input channel value (0x01)
	for (i = 500; i; i--);						// Add time between transmissions to make sure slave can keep up

	/* start conversion and read outputs */

	spi_send(SYNC_command);			// send command to start conversion
	__delay_cycles(7);					// delay of 7 tclk
	chip_select_port |= chip_select_pin;						// pull chip select high after communication is done

	__delay_cycles(250000);				// dealy to wait the conversion
	chip_select_port &= ~chip_select_pin;						// pull chips select low to start communication
	__delay_cycles(7);					// delay of 7 tclk
	spi_send(RDATA_command);			// send command to read conversion result
	for (i = 500; i; i--);              // Add time between transmissions to make sure slave can keep up

	spi_send(NOP_command);			// send dummy command to send clock and read conversion results
	for (i = 500; i; i--);              // Add time between transmissions to make sure slave can keep up
	temp = (long)spi_read() << 16;				// save output to variable

	spi_send(NOP_command);			// send dummy command to send clock and read conversion results
	for (i = 500; i; i--);              // Add time between transmissions to make sure slave can keep up
	temp |= (long)spi_read() << 8;				// save output to variable

	spi_send(NOP_command);			// send dummy command to send clock and read conversion results
	for (i = 500; i; i--);              // Add time between transmissions to make sure slave can keep up
	temp |= spi_read();				// save output to variable

	__delay_cycles(7);					// delay of 7 tclk

	chip_select_port |= chip_select_pin;						// pull chip select high after communcation is done

	return temp;


}
