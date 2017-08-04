/*
 * eps_i2c.h
 *
 *  Created on: 05/05/2016
 *      Author: Bruno Eiterer
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include "crc.h"
#include "hal.h"

void I2C_config(void);

void I2C_tx(uint8_t tx_data);

#endif /* I2C_H_ */
