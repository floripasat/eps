 /**
  * \brief I<sup>2</sup>C protocol driver source
  *
  * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
  */

#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include "I2C.h"
#include "misc.h"
#include "fsp.h"
#include "avoid_infinit_loops.h"
#include "flash.h"

volatile uint8_t EPS_data[69] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 0, 0x01, 0x02};
volatile uint8_t EPS_data2[69] = {0};
volatile fsp_packet_t obdh_packet_fsp_struct;
volatile uint8_t obdh_packet_fsp_array[77] = {0};

/**
 * \brief Sends data via I<sup>2</sup>C protocol
 *
 * Writes the data to be sent in the UCB2TXBUF register.
 *
 * \param tx_data is the data to be sent
 *
 * \returns -
 */

void I2C_tx(uint8_t tx_data){
    UCB2TXBUF = tx_data;
}


/**
 * \brief I<sup>2</sup>C peripheral configuration
 *
 * Configures port 9 bit 5 as I<sup>2</sup>C SDA and port 9 bit 6 as I<sup>2</sup>C SCL. Then set the UCSWRST bit in the UCB2CTL1 register to enable configuration of the peripheral.
 * Then configures the peripheral as I<sup>2</sup>C slave and sets the UCSYNC to signal a synchronous protocol in the UCB2CTL0 register. Then selects own address as 0x48 in the
 * UCB2I2COA register. Then it clears the UCSWRST bit in the UCB2CTL1 register. Finally it enables the TX, RX, STP and STT interrupts in the UCB2IE register.
 *
 * \param -
 *
 * \returns -
 */
void I2C_config(void){
    P9SEL |= BIT5 + BIT6;                            // Assign P2.0 to UCB0SDA and...
    P9DIR |= BIT5 + BIT6;                            // P2.1 to UCB0SCL

    UCB2CTL1 |= UCSWRST;                      // Enable SW reset
    UCB2CTL0 = UCMODE_3 | UCSYNC;             // I2C Slave, synchronous mode
    UCB2I2COA = 0x48;                         // Own Address is 048h
    UCB2CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
    UCB2IE |= UCTXIE | UCRXIE | UCSTPIE | UCSTTIE;     // Enable STT, STP, TX, RX interrupt
}

/**
 * \brief I<sup>2</sup>C peripheral interrupt handler
 *
 * Uses a switch structure to the determine the interrupt source, then proceeds as follows: <br>
 * <br>
 * <b> Start Condition Interrupt </b> <br>
 * Clears the interrupt flag, resets the fsp protocol, clears the data reception counter, generates the fsp packet with the data to be sent to OBDH and encodes it in an array. <br>
 * <br>
 * <b> Stop Condition Interrupt</b> <br>
 * Clears the interrupt flag. <br>
 * <br>
 * <b> RX Interrupt </b> <br>
 * Stores the data from the UCB2RXBUF register in an array and increments the reception counter. If the counter reaches 8, decodes the fsp packet received. If the packet payload
 * is 0x02, indicating a data request command, resets the TX counter. Else if the packet payload is 0xC1, indicating a reset battery charge command, sets the reset battery
 * charge flag on the flash memory. <br>
 * <br>
 * <b> TX Interrupt </b> <br>
 * Puts data from the encoded array position indicated by the tx counter in the UCB2TXBUF and increments the tx counter.
 *
 * \param -
 *
 * \returns -
 */


#pragma vector = USCI_B2_VECTOR
__interrupt void USCI_B2_ISR(void)
{
    static uint8_t packet_length;
    volatile static uint8_t tx_data_counter = 0;
    static uint8_t i = 0;
    static fsp_packet_t obdh_rx_packet;
    static uint8_t obdh_rx_buffer[8];
    uint8_t fsp_status;

    interruption_occurring = 1;     // Variavel para debug (APAGAR DEPOIS DE DEBUGAR)

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
    case 10:                                    // Vector 10: RXIFG
        obdh_rx_buffer[i++] = UCB2RXBUF;
        if(i >= 8){
            i = 0;
            config_avoid_infinit_loops(1000);  // Maximum time on the loop: (TA2CCR0/clock): 1000/250000: 4ms
            do {
                fsp_status = fsp_decode(obdh_rx_buffer[i++], &obdh_rx_packet);
            } while((fsp_status == FSP_PKT_NOT_READY) && !avoid_infinit_loops());

            if(fsp_status == FSP_PKT_READY){
                if(obdh_rx_packet.payload[0] == DATA_REQUEST_COMMAND){
                    tx_data_counter = 0;
                }
                else if(obdh_rx_packet.payload[0] == RESET_BATTERY_CHARGE_COMMAND){             // enter if a reset battery charge command is received from OBDH
                    flash_erase(RESET_BATTERY_CHARGE_ADDR_FLASH);
                    flash_write_single(RESET_BATTERY_CHARGE_COMMAND, RESET_BATTERY_CHARGE_ADDR_FLASH);

                    flash_erase(FLASH_COUNTER_ADDR_FLASH);                                     // reset the counter stored on the flash memory
                    flash_write_long(0x00, FLASH_COUNTER_ADDR_FLASH);

                    tx_data_counter = 0;
                }
            }
        }
        break;
    case 12:                                  // Vector 12: TXIFG
        UCB2TXBUF = obdh_packet_fsp_array[tx_data_counter];             // Transmit data at address PTxData
        tx_data_counter++;
        break;
    default: break;
    }
}
