/**
 * \file
 *
 * \brief Clock configuration source
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
 */

#include <msp430.h>
#include <stdint.h>
#include "clock.h"
#include "avoid_infinit_loops.h"

/**
 * \brief Configures the MCU clocks
 *
 * Configures all of the MCU clocks frequency and source, based on the following
 * table:
 *
 * | Clock | Source | Divider |Frequency |
 * | ----- | ------ | ------- |--------- |
 * | MCLK  | XT2    | 4       |8 MHz     |
 * | SMCLK | XT2    | 1       |32 MHz    |
 * | ACLK  | XT2    | 2       |16 MHz    |
 *
 * To do so, it configures P7.2 and P7.3 as XT2, enables XT2 in UCSCTL6, enables
 * the internal load capacitor in UCSCTL6, chooses the respective dividers in UCSCTL5 and
 * finay chooses XT2 as the source in UCSCTL4. Then it loops until all the fault
 * flags are cleared, indicating that the external crystal is working properly.
 *
 * \param -
 * \return -
 *
*/

void clock_config(void){

    config_avoid_infinit_loops(1000);         // Maximum time on the loop: (TA2CCR0/clock): 1000/250000: 4ms
    while((BAKCTL & LOCKBAK) && !avoid_infinit_loops())                   // Unlock XT1 pins for operation
        BAKCTL &= ~(LOCKBAK);

    P7SEL |= BIT2 + BIT3; //XT2
    UCSCTL6 &= ~XT2OFF;            // Enable XT1 & XT2
    UCSCTL6 |= XCAP_3;                        // Internal load cap

    UCSCTL5 |= DIVM__4 + DIVS__1 + DIVA__2;
    UCSCTL4 |= SELA__XT2CLK + SELS__XT2CLK + SELM__XT2CLK;  // SMCLK = MCLK = ACLK = XT2

    config_avoid_infinit_loops(250);      // Maximum time on the loop: (TA2CCR0/clock): 250/250000: 1ms
    do {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | XT1HFOFFG | DCOFFG);  // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
    } while ((SFRIFG1 & OFIFG) && !avoid_infinit_loops());
}
