/*
 * crc.h
 *
 *  Created on: 30/06/2017
 *      Author: Bruno Eiterer
 */

#ifndef CRC_H_
#define CRC_H_

#include <msp430.h>
#include <stdint.h>

uint8_t crc8(uint8_t initial_value, uint8_t polynomial, uint8_t *data, uint8_t len);

#endif /* CRC_H_ */
