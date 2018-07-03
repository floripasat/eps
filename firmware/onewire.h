/**
 * \file
 *
 * \brief DS2775 batteries monitor drivers header
 *
 * \author Sara Vega Martinez <vegamartinezsara@gmail.com> and Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
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

/**
 * \brief Configures the DS2775 batteries monitor.
 */

void config_DS2775(void);

/**
 * \brief Acquires data from the DS2775 batteries monitor [Deprecated]
 */

void measurement_data_DS2775(void);

/**
 * \brief Puts a bit on the one-wire line.
 */

void outp(int bit);

/**
 * \brief Reads bit from the one-wire line.
 */

int inp(void);

/**
 * \brief Generates reset on one-wire line.
 */

int OneWireReset(void);

/**
 * \brief Sends one-wire bit to one-wire slave.
 */

void OneWireWrite(int bit);

/**
 * \brief Reads one-wire bit from one-wire slave.
 */

int OneWireReadBit(void);

/**
 * \brief Writes a byte to one-wire slave.
 */

void OWWriteByte(int data);

/**
 * \brief Reads a byte from the one-wire slave.
 */

uint8_t OWReadByte(void);

/**
 * \brief Reads a register from the DS2775 batteries monitor.
 */

uint8_t DS2775_read_register(uint8_t register_address);

/**
 * \brief Writes accumulated current maximum value (3 Ah) to batteries monitor appropriated register.
 */

void write_accumulated_current_max_value(void);

#endif /* EPS_ONEWIRE_TEST_H_ */


