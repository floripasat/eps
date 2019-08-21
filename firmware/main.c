/**
 * \file
 *
 * \brief Main MCU and peripherals configuration
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
*/

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
#include "fsp.h"
#include "flash.h"
#include "config.h"



void config_msp430(void);

/**
 * \brief Main Function
 *
 * Configures the MCU, the ADS1248 external ADC and the DS2775 batteries monitor.
*/

void main(void){

    WDTCTL = WDTPW + WDTHOLD;

    config_msp430();

    config_ADS1248(6);
    config_DS2775();

#if FIRST_CHARGE_RESET_ROUTINE == 1
    volatile uint8_t first_charge_reset_flag = flash_read_single(FIRST_CHARGE_RESET_ADDR_FLASH);
    if( (first_charge_reset_flag != FIRST_CHARGE_RESET_ACTIVE) && (first_charge_reset_flag != FIRST_CHARGE_RESET_DONE) ){
        flash_erase(FIRST_CHARGE_RESET_ADDR_FLASH);
        flash_write_single(FIRST_CHARGE_RESET_ACTIVE, FIRST_CHARGE_RESET_ADDR_FLASH);

        flash_erase(FLASH_COUNTER_ADDR_FLASH);
        flash_write_long(0x00, FLASH_COUNTER_ADDR_FLASH);
    }
#endif //FIRST_CHARGE_RESET_ROUTINE

#ifdef _VERBOSE_DEBUG
    uart_tx_debug("system boot complete\r\n");
#endif


#ifdef _DEBUG_WATCHDOG
    while(1);
#endif

    __bis_SR_register(GIE);
    while(1);
}


/**
 * \brief Configures the MCU peripherals
 *
 * Configures the MCU clock, watchdog, UART, ADC, SPI, I2C, timers,
 * the regulators and 555 enable pins, starts the fsp protocol in software and prints
 * messages and turn leds on in debug mode.
 *
 * \param -
 *
 * \return -
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

    fsp_init(FSP_ADR_EPS);

#if defined(_DEBUG) || defined(_VERBOSE_DEBUG)
    system_on_dir |= system_on_pin;
    system_on_port |= system_on_pin;
#endif
}
