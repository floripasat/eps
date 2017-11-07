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
    config_ADS1248(6);
    config_DS2775();

#ifdef _VERBOSE_DEBUG
    uart_tx_debug("system boot complete\r\n");
#endif


#ifdef _DEBUG_WATCHDOG
    while(1);
#endif

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

#ifdef _VERBOSE_DEBUG
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

    P1DIR |= BIT6;
    P1OUT &= ~BIT6;		// disable 555

    OBDH_TTC_regulator_enable_dir |= OBDH_TTC_regulator_enable_pin;
    payload_regulator_enable_dir |= payload_regulator_enable_pin;

#if defined(_DEBUG) || defined(_VERBOSE_DEBUG)
    system_on_dir |= system_on_pin;
    system_on_port |= system_on_pin;
#endif
}


