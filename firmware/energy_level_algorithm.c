/*
 * energy_level_algorithm.c
 *
 *  Created on: 26/06/2017
 *      Author: Bruno Eiterer
 */


#include "energy_level_algorithm.h"

uint8_t energyLevelAlgorithm(uint16_t chargeLevel){

	if(chargeLevel >= 0.75*maxChargeLevel)
		return 0x10;
	else if(chargeLevel >= 0.5*maxChargeLevel)
		return 0x20;
	else if(chargeLevel >= 0.25*maxChargeLevel)
		return 0x30;
	else
		return 0x40;
}
