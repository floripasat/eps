/**
 * \file
 *
 * \brief SPI driver header
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
 *
 */

#ifndef SPI_H_
#define SPI_H_

/**
 * \brief Configures the SPI peripheral
 */

void spi_config(void);

/**
 * \brief Sends data via the SPI peripheral
 */

void spi_send(int data);

/**
 * \brief Reads data from the SPI peripheral
 */

int spi_read(void);


#endif /* SPI_H_ */
