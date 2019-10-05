/*
 * config.h
 *
 * Copyright (C) 2019, Universidade Federal de Santa Catarina.
 *
 * This file is part of FloripaSat-EPS.
 *
 * FloripaSat-EPS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FloripaSat-EPS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-EPS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief General configuration definitions.
 * 
 * \author Ramon de Araujo Borba <ramonborba97@gmail.com>
 * 
 * \date 27/07/2019
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define MAX_BATTERY_CHARGE 						    2450 //[mAh]
#define BAT_MONITOR_CHARGE_VALUE				    (uint16_t)(MAX_BATTERY_CHARGE/0.625) //0.625 is a conversion factor for the  battery monitor

#define RESET_BATTERY_ACCUMULATED_CURRENT		    0
#define FIRST_CHARGE_RESET_ROUTINE				    0

#define HEATER_ENABLED                              1
#define HEATER_TEMPERATURE_SETPOINT                 5   // Temperature in Celsius

#define BATTERY_MONITOR_AS_HEATER_REFERENCE         0
#define BATTERY_MONITOR_AS_HEATER_REF_DUTY_CYCLE    0.5 // 0.0 to 1.0

#endif // CONFIG_H_

//! \} End of config group
