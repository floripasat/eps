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

#define COUNTER_EQUIVALENT_TO_12H       431999
#define COUNTER_EQUIVALENT_TO_1S        9
#define COUNTER_EQUIVALENT_TO_10S       9
#define COUNTER_EQUIVALENT_TO_10MIN     5999
#define COUNTER_EQUIVALENT_TO_1H        6
#define COUNTER_EQUIVALENT_TO_1D        144

#define FIRST_CHARGE_RESET_ACTIVE       0xC5
#define FIRST_CHARGE_RESET_DONE     0x4F

// Calibration constant for ADC 1.5-V Reference, Temp. Sensor 30°C
#define CALADC12_15V_30C        (*((unsigned int *)0x1A1A))
// Calibration constant for ADC 1.5-V Reference, Temp. Sensor 85°C
#define CALADC12_15V_85C        (*((unsigned int *)0x1A1C))
// Calibration constant for ADC 2.5-V Reference, Temp. Sensor 30°C
#define CALADC12_25V_30C        (*((unsigned int *)0x1A22))
// Calibration constant for ADC 2.5-V Reference, Temp. Sensor 85°C
#define CALADC12_25V_85C        (*((unsigned int *)0x1A24))

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
