/*
 * clock.c
 *
 *  Created on: 06/04/2017
 *      Author: Bruno Eiterer
 */

#include <msp430.h>
#include "clock.h"

void clock_config(void){

    while(BAKCTL & LOCKBAK)                   // Unlock XT1 pins for operation
        BAKCTL &= ~(LOCKBAK);

    P7SEL |= BIT2 + BIT3; //XT2
    UCSCTL6 &= ~(XT1OFF | XT2OFF);            // Enable XT1 & XT2
    UCSCTL6 |= XCAP_3;                        // Internal load cap

    UCSCTL5 |= DIVM__4 + DIVS__1 + DIVA__2;
    UCSCTL4 |= SELA__XT2CLK + SELS__XT2CLK + SELM__XT2CLK;  // SMCLK = MCLK = ACLK = XT2

    do {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | XT1HFOFFG | DCOFFG);  // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
    } while (SFRIFG1 & OFIFG);
}
