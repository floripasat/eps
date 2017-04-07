/*
 * eps_onewire_test.h
 *
 *  Created on: 12/04/2016
 *      Author: Bruno Eiterer
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include <stdint.h>

void config_DS2775(void);

void measurement_data_DS2775(void);

void outp(int bit);
int inp(void);
int OneWireReset(void);
void OneWireWrite(int bit);
int OneWireReadBit(void);
void OWWriteByte(int data);
uint8_t OWReadByte(void);

#endif /* EPS_ONEWIRE_TEST_H_ */


