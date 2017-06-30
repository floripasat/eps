/*
 * watchdog.c
 *
 *  Created on: 11 de mai de 2016
 *      Author: mario
 */

#include "watchdog.h"


void watchdog_config(void) {
	WDTCTL = WDTPW + WDTSSEL_1 + WDTIS_4;
}

void watchdog_reset_counter(void){
	WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTCNTCL;
	WDTCTL = ((WDTCTL & 0x00FF) + WDTPW) & ~WDTCNTCL;

}

void wdt_hold_counter(void){
	WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTHOLD;
}

void wdt_release_counter(void){
	WDTCTL = ((WDTCTL & 0x00FF) + WDTPW) & ~WDTHOLD;
}

void reboot(void){
	WDTCTL = 0x00;
}
