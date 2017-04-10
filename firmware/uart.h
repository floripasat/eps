#ifndef EPS_SERIAL_TEST_H_
#define EPS_SERIAL_TEST_H_

#include <stdint.h>


/********** UART functions **********/

void port_mapping(void);

void uart_config(void);

void uart_tx_debug(uint8_t *tx_data);
void uart_tx_beacon(uint8_t tx_data);

#endif /* EPS_SERIAL_TEST_H_ */
