/*
 * crc.c
 *
 *  Created on: 30/06/2017
 *      Author: Bruno Eiterer
 */

#include "crc.h"

uint8_t crc8(uint8_t initial_value, uint8_t polynomial, uint8_t *data, uint8_t len){
    uint8_t crc = initial_value;
    while(len--)
    {
        crc ^= *data++;
        uint8_t j = 0;
        for (j=0; j<8; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80)? polynomial: 0);
        }
        crc &= 0xFF;
    }

    return crc;
}
