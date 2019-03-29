/**
 * \file
 *
 * \brief MCU internal watchdog header
 *
 * \author: Mario Baldini
 */

#ifndef UTIL_WATCHDOG_H_
#define UTIL_WATCHDOG_H_

#include <msp430.h>

//mode
#define INTERVAL			0x10
#define WATCHDOG			0x00

//time to trigger
#define _1_2_SEC			0x04
#define _188_mSEC			0x05
#define _11_8_mSEC			0x06
#define _4_1_mSEC			0x00
#define _1_5_mSEC			0x07
#define _1_mSEC				0x01
#define _64_uSEC			0x02
#define _8_uSEC				0x03

#include <msp430.h>

/**
 * \brief Configures the MCU internal watchdog
 */
void watchdog_config();

/**
 * \brief Resets the watchdog counter
 */
void watchdog_reset_counter(void);

/**
 * \brief Stops the watchdog counter
 */
void wdt_hold_counter(void);

/**
 * \brief Uses the watchdog to reboot the MCU
 */
void reboot(void);



#endif /* UTIL_WATCHDOG_H_ */
