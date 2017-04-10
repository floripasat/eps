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

	uart_config();

	#ifdef _DEBUG
		uart_tx_debug("system booting\r\n");
	#endif

	timer_config();		// call timer configuration function

	adc_config();		// call MSP ADC configuration function

	spi_config();		// call SPI configuration function

	I2C_config();

	__bis_SR_register(GIE);     // Enter LPM0, enable interrupts

	#ifdef _DEBUG
		uart_tx_debug("system boot complete\r\n");
	#endif
}


