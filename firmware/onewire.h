/*
 * eps_onewire_test.h
 *
 *  Created on: 12/04/2016
 *      Author: Bruno Eiterer
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include <stdint.h>

/************ REGISTERS *********/

#define protection_register 0x00
#define protector_threshold_register 0x7F
#define status_register 0x01
#define control_register 0x60
#define accumulated_current_MSB_register 0x10
#define accumulated_current_LSB_register 0x11
#define temperature_MSB_register 0x0A
#define temperature_LSB_register 0x0B
#define average_current_MSB_register 0x08
#define average_current_LSB_register 0x09
#define overcurrent_thresholds_register 0x78
#define current_gain_LSB_register  0x79
#define current_MSB_register 0x0E
#define current_LSB_register 0x0F
#define voltage_MSB1_register 0x0C
#define voltage_LSB1_register 0x0D
#define voltage_MSB2_register 0x1C
#define voltage_LSB2_register 0x1D

/*********** UNITS ***********/

#define	rsense			0.01
#define voltage_unit	0.004883
#define current_unit	0.0000015625/rsense
#define accumulated_current_unit	6.25*0.000001/rsense

void config_DS2775(void);

void measurement_data_DS2775(void);

void outp(int bit);
int inp(void);
int OneWireReset(void);
void OneWireWrite(int bit);
int OneWireReadBit(void);
void OWWriteByte(int data);
uint8_t OWReadByte(void);
uint8_t DS2775_read_register(uint8_t register_address);

#endif /* EPS_ONEWIRE_TEST_H_ */


