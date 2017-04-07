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

    UCSCTL6 &= ~(XT1OFF);            // Enable XT1
    UCSCTL6 |= XCAP_3;                        // Internal load cap

    P7SEL |= BIT2 + BIT3; //XT2

    UCSCTL6 |= XT1OFF;
    UCSCTL6 &= ~XT2OFF;            // Enable XT2
    UCSCTL6 |= XCAP_3;                        // Internal load cap
    UCSCTL3 |= SELREF_2;                      // FLLref = REFO
                                                // Since LFXT1 is not used,
                                                // sourcing FLL with LFXT1 can cause
                                                // XT1OFFG flag to set

    P7SEL |= BIT2 + BIT3; //XT2
    UCSCTL6 &= ~(XT1OFF | XT2OFF);            // Enable XT1 & XT2
    UCSCTL6 |= XCAP_3;                        // Internal load cap

    UCSCTL5 |= DIVM_2 + DIVS_2;
    UCSCTL4 |= SELA_0 + SELS_5 + SELM_5;        // SMCLK = MCLK = XT2 , ACLK = XT1

    do {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | XT1HFOFFG | DCOFFG);  // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
    } while (SFRIFG1 & OFIFG);
}
