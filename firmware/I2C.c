#include <msp430.h>
#include "I2C.h"
#include "misc.h"

volatile uint8_t EPS_data[69] = {0};

void I2C_tx(uint8_t tx_data){
	UCB2TXBUF = tx_data;
}

void I2C_config(void){
	P9SEL |= BIT5 + BIT6;                            // Assign P2.0 to UCB0SDA and...
	P9DIR |= BIT5 + BIT6;                            // P2.1 to UCB0SCL

	UCB2CTL1 |= UCSWRST;                      // Enable SW reset
	UCB2CTL0 = UCMODE_3 | UCSYNC;             // I2C Slave, synchronous mode
	UCB2I2COA = 0x48;                         // Own Address is 048h
	UCB2CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
	UCB2IE |= UCTXIE | UCSTPIE | UCSTTIE;     // Enable STT and STP interrupt
	// Enable TX interrupt
}


#pragma vector = USCI_B2_VECTOR
__interrupt void USCI_B2_ISR(void)
{

	volatile static uint8_t tx_data_counter = 0;
  switch(__even_in_range(UCB2IV,12))
  {
  case  0: break;                           // Vector  0: No interrupts
  case  2: break;                           // Vector  2: ALIFG
  case  4: break;                           // Vector  4: NACKIFG
  case  6:                                  // Vector  6: STTIFG
    UCB2IFG &= ~UCSTTIFG;                   // Clear start condition int flag
    break;
  case  8:                                  // Vector  8: STPIFG
    UCB2IFG &= ~UCSTPIFG;                   // Clear stop condition int flag
    break;
  case 10: break;                           // Vector 10: RXIFG
  case 12:                                  // Vector 12: TXIFG
    UCB2TXBUF = EPS_data[tx_data_counter];                 // Transmit data at address PTxData
    tx_data_counter++;
    break;
  default: break;
  }
}
