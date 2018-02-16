/**
 * \file
 *
 * \brief MCU internal watchdog source
 *
 * \author: Mario Baldini
 */

#include "watchdog.h"

/**
 * \brief Configures the MCU internal watchdog
 *
 * Writes the WDTCTL register with the WDTPW value to prevent a reboot, selects
 * the clock source as the VLO (10 kHz) and the timer internal to 10000/2^13 = 819.2 ms
 *
 * \param -
 *
 * \returns -
 */

void watchdog_config(void) {
	WDTCTL = WDTPW + WDTSSEL__VLO + WDTIS__8192;
}

/**
 * \brief Resets the watchdog counter
 *
 * Clears the MCU internal watchdog counter by setting the WDTCNTCL bit in the WDTCTL
 * register. As always, the WDTPW must be written to avoid a reboot.
 *
 * \param -
 *
 * \returns -
 */

void watchdog_reset_counter(void){
	WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTCNTCL;
	WDTCTL = ((WDTCTL & 0x00FF) + WDTPW) & ~WDTCNTCL;

}

/**
 * \brief Stops the watchdog counter
 *
 * Stops the MCU internal watchdog counter by setting the WDTHOLD bit in the WDTCNTCL
 * register. As always, the WDTPW value must be written to avoid a reboot.
 *
 * \param -
 *
 * \returns -
 */

void wdt_hold_counter(void){
	WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTHOLD;
}

/**
 * After a Hold has been issued to the counter, this function can be used to resume
 * counting. It achieves its objective by clearing the WDTHOLD bit in the WDTCTL
 * register. As always, the WDTPW value has to be written to avoid a reboot.
 *
 * \param -
 *
 * \returns -
 */

void wdt_release_counter(void){
	WDTCTL = ((WDTCTL & 0x00FF) + WDTPW) & ~WDTHOLD;
}

/**
 * \brief Uses the watchdog to reboot the MCU
 *
 * Writes a value to WDTCTL without the WDTPW, causing a reboot.
 *
 * \param -
 *
 * \returns -
 */

void reboot(void){
	WDTCTL = 0x00;
}
