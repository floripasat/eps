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

#define timer_debug_port_dir P1DIR
#define timer_debug_port_1s P1OUT
#define timer_debug_pin_1s BIT0
#define timer_debug_port_100ms P1OUT
#define timer_debug_pin_100ms BIT1

/**** Data Packet Offsets ****/

#define negative_y_panel_current_LSB 0
#define negative_y_panel_current_MSB 1
#define positive_x_panel_current_LSB 2
#define positive_x_panel_current_MSB 3
#define negative_x_panel_current_LSB 4
#define negative_x_panel_current_MSB 5
#define positive_z_panel_current_LSB 6
#define positive_z_panel_current_MSB 7
#define negative_z_panel_current_LSB 8
#define negative_z_panel_current_MSB 9
#define positive_y_panel_current_LSB 10
#define positive_y_panel_current_MSB 11
#define negative_y_positive_x_panel_voltage_LSB 12
#define negative_y_positive_x_panel_voltage_MSB 13
#define negative_x_positive_z_panel_voltage_LSB 14
#define negative_x_positive_z_panel_voltage_MSB 15
#define negative_z_positive_y_panel_voltage_LSB 16
#define negative_z_positive_y_panel_voltage_MSB 17

#define vpanels_voltage_LSB 18
#define vpanels_voltage_MSB 19
#define bus_voltage_LSB 20
#define bus_voltage_MSB 21
#define beacon_eps_current_LSB 22
#define beacon_eps_current_MSB 23
#define msp_temperature_LSB 24
#define msp_temperature_MSB 25

#define battery_average_current_LSB 26
#define battery_average_current_MSB 27
#define battery_monitor_temeperature_LSB 28
#define battery_monitor_temeperature_MSB 29
#define battery1_voltage_LSB 30
#define battery1_voltage_MSB 31
#define battery2_voltage_LSB 32
#define battery2_voltage_MSB 33
#define battery_current_LSB 34
#define battery_current_MSB 35
#define battery_accumulated_current_LSB 36
#define battery_accumulated_current_MSB 37
#define protection_register_LSB 38
#define status_register_LSB 39
#define cycle_counter_LSB 40
#define RAAC_LSB 41
#define RAAC_MSB 42
#define RSAC_LSB 43
#define RSAC_MSB 44
#define RARC_LSB 45
#define RSRC_LSB 46

#define RTD0_B1 47
#define RTD0_B2 48
#define RTD0_B3 49
#define RTD1_B1 50
#define RTD1_B2 51
#define RTD1_B3 52
#define RTD2_B1 53
#define RTD2_B2 54
#define RTD2_B3 55
#define RTD3_B1 56
#define RTD3_B2 57
#define RTD3_B3 58
#define RTD4_B1 59
#define RTD4_B2 60
#define RTD4_B3 61
#define RTD5_B1 62
#define RTD5_B2 63
#define RTD5_B3 64
#define RTD6_B1 65
#define RTD6_B2 66
#define RTD6_B3 67

#define eps_status 68

#define CRC 69




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

#define msp_temperature 10

/**** USCI ****/

#define A0 0
#define A1 1
#define A2 2
#define B0 3
#define B1 4
#define B2 5

/**** LED ****/

#define system_on_dir P5DIR
#define system_on_port P5OUT
#define system_on_pin BIT4

#define HWREG16(x)                                                             \
    (*((volatile uint16_t *)((uint16_t)x)))

/**** MPPT PWM ****/

#define clock 8000000
#define mppt_pwm_frequency 500000
#define mppt_pwm_duty_cycle_step (clock/mppt_pwm_frequency)*0.0625


#endif /* HAL_H_ */
