/*
 * ADS1248.h
 *
 *  Created on: 08/06/2016
 *      Author: Bruno Eiterer
 */

#ifndef ADS1248_H_
#define ADS1248_H_

/**** COMMANDS ****/

#define RESET_command 0x06				// reset command
#define RDATA_command 0x12				// command to read conversion data once
#define SDATAC_command 0x16				// stop read data continous mode command
#define RDATAC_command 0x14				// read data continous mode command
#define WREG_command 0x40				// write to register command
#define RREG_command 0x20				// read from register command
#define NOP_command 0xff				// no operation command, used to clock out data
#define SYNC_command 0x04				// command to start ADC conversion

/**** PINS and Channels ****/

#define chip_select_port P8OUT
#define chip_select_pin BIT5
#define negative_channel 0x01
#define ADS1248_START_port P8OUT
#define ADS1248_START_pin BIT6

void config_ADS1248(int channel);
long read_ADS1248(int channel);

#endif /* ADS1248_H_ */
