/**
 * \file uart.h
 *
 * \brief UART driver header
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
 *
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

/**
 * \brief Maps multi-function port 2 pins to UART
 *
 */

void port_mapping(void);

/**
 * \brief Configures the UART peripheral
 */

void uart_config(void);

/**
 * \brief Sends data to the debug port
 *
 */

void uart_tx_debug(uint8_t *tx_data);

/**
 * \brief Sends data to Beacon
 *
 */

void uart_tx_beacon(uint8_t tx_data);

/**
 * \brief Sends floating point data to the debug port
 *
 */

void float_send(float c);

#endif /* UART_H_ */
