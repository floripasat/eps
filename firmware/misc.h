#ifndef MISC_H_
#define MISC_H_

#include <stdint.h>
#include <stdio.h>
#include "watchdog.h"


#define TRUE 1
#define FALSE 0

void sysled_enable(void);
void sysled_toggle(void);
void frame_to_string(unsigned char frame[], unsigned char string[], int size);
void int_to_char(int data, char string[], int size);
unsigned char hex_to_char(unsigned char byte);
uint16_t msp_temperature_convert(uint16_t temperature_raw);


#endif /* MISC_H_ */
