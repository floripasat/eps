/*
 * hal.h
 *
 *  Created on: 09/02/2017
 *      Author: Bruno Eiterer
 */

#ifndef HAL_H_
#define HAL_H_

#include <msp430.h>

/**** Timer Debug ****/

#define timer_debug_port_1s P1OUT
#define timer_debug_pin_1s BIT0
#define timer_debug_port_100ms P3OUT
#define timer_debug_pin_100ms BIT0

/**** ADC ****/

#define negative_y_panel_current 0
#define positive_x_panel_current 1
#define negative_x_panel_current 2
#define positive_z_panel_current 3
#define negative_z_panel_current 4
#define positive_y_panel_current 5

#define negative_y_positive_x_panel_voltage 12
#define negative_x_positive_z_panel_voltage 13
#define negative_z_positive_y_panel_voltage 14

#define beacon_eps_current 6

#define bus_voltage 7

#define vpanels_voltage 15



#endif /* HAL_H_ */
