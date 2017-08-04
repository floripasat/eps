#include <msp430.h>
#include <string.h>
#include "I2C.h"
#include "misc.h"

volatile uint8_t EPS_data[71] = {0x7E, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 0, 0x01, 69, 70};
volatile uint8_t EPS_data2[71] = {0};

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
	UCB2IE |= UCTXIE | UCRXIE | UCSTPIE | UCSTTIE;     // Enable STT, STP, TX, RX interrupt
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
    memcpy(EPS_data2, EPS_data, 71);
    EPS_data2[CRC] = crc8(0x03, 0x92, EPS_data2+1, 69);
    break;
  case  8:                                  // Vector  8: STPIFG
    UCB2IFG &= ~UCSTPIFG;                   // Clear stop condition int flag
    break;
  case 10: 									// Vector 10: RXIFG
	  if(UCB2RXBUF == 0x0F){
		  tx_data_counter = 0;
	  }
	  break;
  case 12:                                  // Vector 12: TXIFG
    UCB2TXBUF = EPS_data2[tx_data_counter];                 // Transmit data at address PTxData
    tx_data_counter++;
    break;
  default: break;
  }
}
