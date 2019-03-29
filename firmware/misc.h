#ifndef MISC_H_
#define MISC_H_

#include <stdint.h>
#include <stdio.h>
#include "watchdog.h"

#define TRUE 1
#define FALSE 0
#define RTD_VALUE_LOWER_LIMIT 0x00449DA9	//Equivalent to -30° Celcius
#define RTD_VALUE_UPPER_LIMIT 0x005C825D	//Equivalent to  50° Celcius

void sysled_enable(void);
void sysled_toggle(void);
void frame_to_string(unsigned char frame[], unsigned char string[], int size);
void int_to_char(int data, char string[], int size);
unsigned char hex_to_char(unsigned char byte);
uint32_t rtd_value_verification(uint32_t value1, uint32_t value2);
//uint32_t median_value(uint32_t *vector, uint8_t size);

#endif /* MISC_H_ */
