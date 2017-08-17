#include <stdint.h>
#include "mppt.h"
#include "hal.h"


void mppt_algorithm(uint16_t current, uint16_t voltage, uint16_t address, mppt_parameters_t* mppt_parameters){

	volatile float previous_power = mppt_parameters->previous_power;
	volatile uint16_t previous_voltage = mppt_parameters->previous_voltage;
	volatile float power;
	volatile uint8_t previous_step = mppt_parameters->previous_step;

	power = (current*solar_panel_current_unit)*(voltage*solar_panel_voltage_unit);

	if(HWREG16(address) > 70)
		HWREG16(address) = 10;

	if(previous_power > power && previous_step == 0){		// determine if previous load current action increased the input power
		if(HWREG16(address) < 70)
		HWREG16(address) += 1;
		mppt_parameters->previous_step = 1;
	}
	else if(previous_power > power && previous_step == 1){
		if(HWREG16(address) > 0)
		HWREG16(address) -= 1;
		mppt_parameters->previous_step = 0;

	}
	else if(previous_power < power && previous_step == 0){
		if(HWREG16(address) > 0)
		HWREG16(address) -= 1;
		mppt_parameters->previous_step = 0;
	}
	else if(previous_power < power && previous_step == 1){
		if(HWREG16(address) < 70)
		HWREG16(address) += 1;
		mppt_parameters->previous_step = 1;
	}
	mppt_parameters->previous_power = power;						// saves current input power for next iteration

#ifdef _DEBUG_MPPT

	uart_tx_debug("Input Power: ");
	float_send(power);
	uart_tx_debug("\r\n");

#endif
	mppt_parameters->previous_voltage = voltage;					// saves current voltage for next iteration

}

