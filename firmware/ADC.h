/*
 * eps_ADC.h
 *
 *  Created on: 16/11/2016
 *      Author: Bruno Eiterer
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void adc_config(void);

int16_t adc_read(uint8_t channel);


#endif /* ADC_H_ */
