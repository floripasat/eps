#include <stdint.h>
#include "mppt.h"
#include "hal.h"


void mppt_algorithm(uint16_t current, uint16_t voltage, uint16_t address){

	volatile static float previous_power = 0;
	volatile static uint16_t previous_voltage = 0;
	volatile float power;
	volatile uint8_t previous_step = 1;

	HWREG16(address) += 1;


	power = (current*solar_panel_current_unit)*(voltage*solar_panel_voltage_unit);

	/*

	if(previous_power > power && previous_step == 0){		// determine if previous load current action increased the input power
		HWREG16(address) += 1;
		previous_step = 1;
	}
	else if(previous_power > power && previous_step == 1){
		HWREG16(address) -= 1;
		previous_step = 0;

	}
	else if(previous_power < power && previous_step == 0){
		HWREG16(address) -= 1;
		previous_step = 0;
	}
	else if(previous_power < power && previous_step == 1){
		HWREG16(address) += 1;
		previous_step = 1;
	}
	previous_power = power;						// saves current input power for next iteration
//	previous_voltage = voltage;					// saves current voltage for next iteration
 *
 *
 */

	__delay_cycles(2000);
}

