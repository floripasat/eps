/*
 * eps_ADC.h
 *
 *  Created on: 16/11/2016
 *      Author: Bruno Eiterer
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void config_adc(void);

int16_t read_adc(uint8_t channel);


#endif /* ADC_H_ */
