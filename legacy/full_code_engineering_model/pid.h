#ifndef PID_H_
#define PID_H_

#include <limits.h>

enum {
	PID_MAX = 2000,
	MAX_I_TERM = PID_MAX/2
};

/*
 * PID driver instance data. The user is required to allocate a
 * variable of this type for every PID device in the system. A pointer
 * to a variable of this type is then passed to the driver API functions.
 */
typedef struct {
	float LastProcessValue;
	float SumError;
	float ScalingFactor;
	long PFactor;
	long IFactor;
	long DFactor;
	int MaxError;
	long MaxSumError;
} Pid;

void Pid_Init(Pid *pid, float PFactor, float IFactor, float DFactor,
		int ScalingFactor);
float Pid_Control(int SetPoint, float ProcessValue, Pid *pid);

#endif
