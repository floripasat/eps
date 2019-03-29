/*
 * flash.h
 *
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
 *
 * This file is part of FloripaSat-OBDH.
 *
 * FloripaSat-OBDH is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FloripaSat-OBDH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /**
 * \file flash.h
 *
 * \brief Functions to use the MSP430's flash memory
 *
 * \author Matheus Frata
 *
 */

#include <msp430.h>
#include <stdint.h>

#ifndef UTIL_FLASH_H_
#define UTIL_FLASH_H_

#define FULL     1
#define NOT_FULL 0

#define TRUE  1
#define FALSE 0

/*
 * FLASH MEMORY ADRESSES
 */

#define BANK0  0
#define BANK1  1
#define BANK2  2
#define BANK3  3

//128 KB banks
#define BANK0_ADDR      0x008000
#define BANK1_ADDR      0x028000
#define BANK2_ADDR      0x048000
#define BANK3_ADDR      0x068000
//128 B info segments
#define SEGD_ADDR       0x001800
#define SEGC_ADDR       0x001880
#define SEGB_ADDR       0x001900
#define SEGA_ADDR       0x001980
//512 B bootstrap segments
#define BSL0_ADDR       0x001600
#define BSL1_ADDR       0x001400
#define BSL2_ADDR       0x001200
#define BSL3_ADDR       0x001000
#define MASS_ERASE      0XFFFFFF

#define FLASH_COUNTER_ADDR_FLASH            (uint32_t *) SEGD_ADDR      // flash memory address to store a counter used in battery charge reset procedures
#define RESET_BATTERY_CHARGE_ADDR_FLASH     (uint8_t *) SEGC_ADDR       // flash memory address to store the battery charge reset mode flag
#define FIRST_CHARGE_RESET_ADDR_FLASH       (uint8_t *) SEGB_ADDR       // flash memory address to store the flag for the first charge reset (in deployment)

char *flash_ptr;                         	// Initialize Flash pointer
long *current_flash_ptr;					//place holder to save flash pointer to boot

void flash_write(uint8_t* data, uint16_t bytes);
void flash_write_single(uint8_t data, uint8_t *addr);
void flash_write_long(uint32_t data, uint32_t *addr);
void flash_erase(uint32_t*);
uint8_t flash_read_single(uint8_t *addr);
uint32_t flash_read_long(uint32_t *addr);

#endif /* UTIL_FLASH_H_ */
