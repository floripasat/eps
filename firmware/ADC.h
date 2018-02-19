/**
 * \file
 *
 * \brief ADC driver header
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com>
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

/**
 * \fn adc_config
 *
 * \brief Configures the ADC peripheral
 */

void adc_config(void);

/**
 * \fn adc_read
 *
 * \brief Reads an ADC channel
 */

int16_t adc_read(uint8_t channel);


#endif /* ADC_H_ */
