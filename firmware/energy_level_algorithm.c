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

    chargeLevel = 0x600; //always on level 4
    switch(previous_level){
    case level1:
        if(chargeLevel > 0.8*maxChargeLevel){
        	OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
        	payload_regulator_enable_port |= payload_regulator_enable_pin;
            return level1;
        }
        else{
            previous_level = level2;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return level2;
        }
        break;

    case level2:
        if(chargeLevel > 0.85*maxChargeLevel){
            previous_level = level1;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port |= payload_regulator_enable_pin;
            return level1;
        }else if(chargeLevel < 0.85*maxChargeLevel && chargeLevel > 0.6*maxChargeLevel){
        	OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
        	payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return level2;
        }else{
            previous_level = level3;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return level3;
        }
        break;
    case level3:
        if(chargeLevel > 0.65*maxChargeLevel){
            previous_level = level2;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return level2;
        }else if(chargeLevel < 0.65*maxChargeLevel && chargeLevel > 0.4*maxChargeLevel){
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return level3;
        }else{
            previous_level = level4;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return level4;
        }
        break;
    case level4:
        if(chargeLevel > 0.45*maxChargeLevel){
            previous_level = level3;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return level3;
        }else if(chargeLevel < 0.45*maxChargeLevel && chargeLevel > 0.2*maxChargeLevel){
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return level4;
        }else{
            previous_level = level5;
            OBDH_TTC_regulator_enable_port &= ~OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return level5;
        }
        break;
    case level5:
        if(chargeLevel > 0.25*maxChargeLevel){
            previous_level = level4;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return level4;
        }else{
        	OBDH_TTC_regulator_enable_port &= ~OBDH_TTC_regulator_enable_pin;
        	payload_regulator_enable_port &= ~payload_regulator_enable_pin;
            return level5;
        }
        break;
    }

    return 0;
}
