#include <msp430.h>
#include <string.h>
#include "I2C.h"
#include "misc.h"
#include "fsp.h"

volatile uint8_t EPS_data[69] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 0, 0x01, 0x01};
volatile uint8_t EPS_data2[69] = {0};
volatile FSPPacket obdh_packet_fsp_struct;
volatile uint8_t obdh_packet_fsp_array[77] = {0};

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
    static uint8_t packet_length;
    volatile static uint8_t tx_data_counter = 0;
    static uint8_t i = 0;
    static FSPPacket obdh_rx_packet;
    static uint8_t obdh_rx_buffer[8];
    uint8_t fsp_status;

    switch(__even_in_range(UCB2IV,12))
    {
    case  0: break;                           // Vector  0: No interrupts
    case  2: break;                           // Vector  2: ALIFG
    case  4: break;                           // Vector  4: NACKIFG
    case  6:                                  // Vector  6: STTIFG
        UCB2IFG &= ~UCSTTIFG;                   // Clear start condition int flag
        fsp_reset();
        i = 0;
        fsp_gen_data_pkt(EPS_data, sizeof(EPS_data), FSP_ADR_OBDH, FSP_PKT_WITHOUT_ACK, &obdh_packet_fsp_struct);
        fsp_encode(&obdh_packet_fsp_struct, obdh_packet_fsp_array, &packet_length);
        break;
    case  8:                                  // Vector  8: STPIFG
        UCB2IFG &= ~UCSTPIFG;                   // Clear stop condition int flag
        break;
    case 10: 									// Vector 10: RXIFG
        obdh_rx_buffer[i++] = UCB2RXBUF;
        if(i >= 8){
            i = 0;
            do {
                fsp_status = fsp_decode(obdh_rx_buffer[i++], &obdh_rx_packet);
            } while(fsp_status == FSP_PKT_NOT_READY);

            if(fsp_status == FSP_PKT_READY) {
                if(obdh_rx_packet.payload[0] == 0x02)
                    tx_data_counter = 0;
            }
        }
        break;
    case 12:                                  // Vector 12: TXIFG
        UCB2TXBUF = obdh_packet_fsp_array[tx_data_counter];                // Transmit data at address PTxData
        tx_data_counter++;
        break;
    default: break;
    }
}
