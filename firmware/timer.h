/**
 * \file
 *
 * \brief Timer driver header
 *
 * \author Bruno Valo Barbosa Eiterer <brunoeiterer@gmail.com> and Daniel Igiski Baron <->
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#define PWM_DUTY_CYCLE	0.5		// absolute pwm duty cycle
#define PWM_PERIOD	10000			// pwm period in us

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
