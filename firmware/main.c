#include <msp430.h> 
#include <stdlib.h>
#include "ADC.h"
#include "ADS1248.h"
#include "I2C.h"
#include "onewire.h"
#include "pid.h"
#include "timer.h"
#include "uart.h"
#include "SPI.h"
#include "clock.h"
#include "hal.h"
#include "watchdog.h"


void config_msp430(void);


void main(void){

 	WDTCTL = WDTPW + WDTHOLD;

	config_msp430();
	config_ADS1248(0);
	config_DS2775();

	__bis_SR_register(GIE);
	while(1);
}


/**********************************
 * Function name: config_msp430
 *
 * Arguments: void
 *
 * Returns: void
 *
 */


void config_msp430(void){

	clock_config();

	watchdog_config();

	uart_config();

	watchdog_reset_counter();

	#ifdef _DEBUG
		uart_tx_debug("system booting\r\n");
	#endif

	adc_config();		// call MSP ADC configuration function

	watchdog_reset_counter();

	spi_config();		// call SPI configuration function

	watchdog_reset_counter();

	I2C_config();

	watchdog_reset_counter();

	timer_config();		// call timer configuration function

	watchdog_reset_counter();

	__bis_SR_register(GIE);     // Enter LPM0, enable interrupts

	#ifdef _DEBUG
		uart_tx_debug("system boot complete\r\n");
	#endif

}


