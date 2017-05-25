#include <stdint.h>
#include "mppt.h"
#include "hal.h"


void mppt_algorithm(uint16_t current, uint16_t voltage, uint16_t address){

	volatile static float previous_power = 0;
	volatile static float previous_voltage = 0;
	volatile float power;

	power = (current*solar_panel_current_unit)*(voltage*solar_panel_voltage_unit);

	if(previous_power > power && previous_voltage < voltage*solar_panel_voltage_unit){		// determine if previous load current action increased the input power
		TBCCR0 = TBCCR0 + mppt_pwm_duty_cycle_step;
	}
	else if(previous_power > power && previous_voltage > voltage*solar_panel_voltage_unit){
		TBCCR0 -= mppt_pwm_duty_cycle_step;
	}
	else if(previous_power < power && previous_voltage < voltage*solar_panel_voltage_unit){
		TBCCR0 -= mppt_pwm_duty_cycle_step;
	}
	else if(previous_power < power && previous_voltage > voltage*solar_panel_voltage_unit){
		TBCCR0 = TBCCR0 + mppt_pwm_duty_cycle_step;
	}
	previous_power = power;						// saves current input power for next iteration
	previous_voltage = voltage*solar_panel_voltage_unit;					// saves current voltage for next iteration
}

