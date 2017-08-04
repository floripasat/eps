/*
 * energy_level_algorithm.c
 *
 *  Created on: 26/06/2017
 *      Author: Bruno Eiterer
 */


#include "energy_level_algorithm.h"

uint8_t energyLevelAlgorithm(uint8_t previous_level, uint16_t chargeLevel){

    switch(previous_level){
    case level1:
        if(chargeLevel > 0.8*maxChargeLevel)
            return level1;
        else{
            previous_level = level2;
            return level2;
        }
        break;

    case level2:
        if(chargeLevel > 0.85*maxChargeLevel){
            previous_level = level1;
            return level1;
        }else if(chargeLevel < 0.85*maxChargeLevel && chargeLevel > 0.6*maxChargeLevel){
            return level2;
        }else{
            previous_level = level3;
            return level3;
        }
        break;
    case level3:
        if(chargeLevel > 0.65*maxChargeLevel){
            previous_level = level2;
            return level2;
        }else if(chargeLevel < 0.65*maxChargeLevel && chargeLevel > 0.4*maxChargeLevel){
            return level3;
        }else{
            previous_level = level4;
            return level4;
        }
        break;
    case level4:
        if(chargeLevel > 0.45*maxChargeLevel){
            previous_level = level3;
            return level3;
        }else if(chargeLevel < 0.45*maxChargeLevel && chargeLevel > 0.2*maxChargeLevel){
            return level4;
        }else{
            previous_level = level5;
            OBDH_TTC_regulator_enable_port &= ~OBDH_TTC_regulator_enable_pin;
            return level5;
        }
        break;
    case level5:
        if(chargeLevel > 0.25*maxChargeLevel){
            previous_level = level4;
            OBDH_TTC_regulator_enable_port |= OBDH_TTC_regulator_enable_pin;
            return level4;
        }else{
            return level5;
        }
        break;
    }

    return 0;
}
