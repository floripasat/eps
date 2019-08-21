/**
 * \file
 *
 * \brief Energy level algorithm source
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
 */

#include "energy_level_algorithm.h"

/**
 * \brief Determines the operation level of the satellite based on the remaining charge of the batteries.
 *
 * Compares the current charge with the maximum charge level and changes the operation level. The levels are defined by 80 %, 60 %, 40 % and 20 % when decreasing the charge and
 * 85 %, 65 %, 45 % and 25 % when increasing the charge. This is done to avoid ringing in charge levels near the transitions. Also enables/disables the OBDH and the Payload regulators
 * based on the energy level
 *
 * \param previous_level is the operation level prior to the execution of the algorithm
 * \param chargeLevel is the current charge level of the batteries
 *
 * \returns the energy level
 */

uint8_t energyLevelAlgorithm(uint8_t previous_level, uint16_t chargeLevel){

    switch(previous_level){
    case LEVEL_1:
        if(chargeLevel > 0.875*MAX_BATTERY_CHARGE){
        	OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
        	payload_regulator_enable_port |= payload_regulator_enable_pin;
            return LEVEL_1;
        }
        else{
            previous_level = LEVEL_2;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return LEVEL_2;
        }
        break;

    case LEVEL_2:
        if(chargeLevel > 0.9*MAX_BATTERY_CHARGE){
            previous_level = LEVEL_1;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port |= payload_regulator_enable_pin;
            return LEVEL_1;
        }else if(chargeLevel <= 0.9*MAX_BATTERY_CHARGE && chargeLevel > 0.775*MAX_BATTERY_CHARGE){
        	OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
        	payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return LEVEL_2;
        }else{
            previous_level = LEVEL_3;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return LEVEL_3;
        }
        break;
    case LEVEL_3:
        if(chargeLevel > 0.8*MAX_BATTERY_CHARGE){
            previous_level = LEVEL_2;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return LEVEL_2;
        }else if(chargeLevel <= 0.8*MAX_BATTERY_CHARGE && chargeLevel > 0.675*MAX_BATTERY_CHARGE){
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return LEVEL_3;
        }else{
            previous_level = LEVEL_4;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return LEVEL_4;
        }
        break;
    case LEVEL_4:
        if(chargeLevel > 0.7*MAX_BATTERY_CHARGE){
            previous_level = LEVEL_3;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return LEVEL_3;
        }else if(chargeLevel <= 0.7*MAX_BATTERY_CHARGE && chargeLevel > 0.575*MAX_BATTERY_CHARGE){
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return LEVEL_4;
        }else{
            previous_level = LEVEL_5;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return LEVEL_5;
        }
        break;
    case LEVEL_5:
        if(chargeLevel > 0.6*MAX_BATTERY_CHARGE){
            previous_level = LEVEL_4;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return LEVEL_4;
        }else{
        	OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
        	payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return LEVEL_5;
        }
        break;
    default:
        OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
        payload_regulator_enable_port &= ~payload_regulator_enable_pin;
        return LEVEL_4;
        break;
    }

    return 0;
}
