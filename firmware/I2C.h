/**
 * \file
 *
 * \brief I<sup>2</sup>C driver header
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include "hal.h"

volatile int interruption_occurring;         // Variavel Teste para Debug (APAGAR DEPOIS DE DEBUGAR)

#define DATA_REQUEST_COMMAND 0x02
#define RESET_BATTERY_CHARGE_COMMAND 0x05

/**
 * \brief I<sup>2</sup>C peripheral configuration
 */

void I2C_config(void);

/**
 * \brief Sends data via I<sup>2</sup>C protocol
 */

void I2C_tx(uint8_t tx_data);

#endif /* I2C_H_ */
