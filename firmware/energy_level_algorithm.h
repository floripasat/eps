/**
 * \file
 *
 * \brief Energy level algorithm header
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
 */

#ifndef ENERGY_LEVEL_ALGORITHM_C_
#define ENERGY_LEVEL_ALGORITHM_C_

#include <msp430.h>
#include <stdint.h>
#include "hal.h"
#include "config.h"

#define LEVEL_1 0x01
#define LEVEL_2 0x02
#define LEVEL_3 0x03
#define LEVEL_4 0x04
#define LEVEL_5 0x05

/**
 * \brief Determines the operation level of the satellite based on the remaining charge of the batteries.
 */

uint8_t energyLevelAlgorithm(uint8_t previous_level, uint16_t chargeLevel);


#endif /* ENERGY_LEVEL_ALGORITHM_C_ */
