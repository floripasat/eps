/**
 * \file
 *
 * \brief Timer driver header
 *
 * \author Bruno Valo Barbosa Eiterer <brunoeiterer@gmail.com> and Daniel Igiski Baron <danieligiskibaron@hotmail.com>
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#define PWM_DUTY_CYCLE	0.5		// absolute pwm duty cycle
#define PWM_PERIOD	10000			// pwm period in us

#define COUNTER_VALUE_12_HOURS      432000
#define COUNTER_VALUE_1_SECOND      10
#define COUNTER_VALUE_10_SECONDS    100
#define COUNTER_VALUE_10_MINUTES    6000
#define FLASH_COUNTER_VALUE_1_DAY   144

/**
 * \brief Configures the timer peripheral
 */

void timer_config();

extern volatile int avc_msb;
extern volatile int avc_lsb;
extern volatile int vr_msb1;
extern volatile int vr_lsb1;
extern volatile int vr_msb2;
extern volatile int vr_lsb2;
extern volatile int tr_msb;
extern volatile int tr_lsb;
extern volatile int cr_msb;
extern volatile int cr_lsb;
extern volatile int acr_msb;
extern volatile int acr_lsb;
extern volatile unsigned int RG_Protection;
extern volatile uint8_t EPSData[23];
extern volatile long temp[3];

#endif /* TIMER_H_ */
