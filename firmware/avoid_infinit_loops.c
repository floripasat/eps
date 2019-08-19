#include <msp430.h>
#include <stdint.h>

uint8_t avoid_infinit_loops(void)
{
    return (TA2CCTL0 & CCIFG);
}

void config_avoid_infinit_loops(uint16_t i)
{
    TA2CCR0 = i;
    TA2CCTL0 &= ~CCIFG;
    TA2CTL |= TACLR;
}
