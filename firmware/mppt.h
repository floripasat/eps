/*
 * mppt.h
 *
 *  Created on: 25/05/2017
 *      Author: Bruno Eiterer
 */

#ifndef MPPT_H_
#define MPPT_H_

#include <msp430.h>

#define HWREG16(x)                                                             \
    (*((volatile uint16_t *)((uint16_t)x)))

#define solar_panel_current_unit 0.0001479640152
#define solar_panel_voltage_unit 0.001178588867

void mppt_algorithm(uint16_t current, uint16_t voltage, uint16_t address);



#endif /* MPPT_H_ */
