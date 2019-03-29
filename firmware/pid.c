/**
 * \file
 *
 * \brief PID algorithm source
 *
 * \author Bruno Vale Barbosa Eiterer <brunoeiterer@gmail.com> and Leonardo Kessler Slongo <lkslongo@gmail.com>
 */

#include "pid.h"
#include <stdlib.h>

/**
 * \brief Resets the PID structure values to the default.
 *
 * Resets the error and process values to zero, applies the scaling factor the each parameter and assigns the error limits.
 *
 * \param pid is a structure containing the parameters to be initialized
 * \param PFactor is the proportional parameter of the PID algorithm
 * \param IFactor is the integral parameter of the PID algorithm
 * \param DFactor is the derivative parameter of the PID algorithm
 * \param ScalingFactor is used to scale the parameters of the PID algorithm
 *
 * \returns -
 *
 */

void Pid_Init(struct Pid pid, float PFactor, float IFactor, float DFactor,
		int ScalingFactor)
{
	/*
	 * Start values for PID controller.
	 */
	pid.SumError = 0;
	pid.LastProcessValue = 0;

	pid.ScalingFactor = ScalingFactor;

	/*
	 * Tuning constants for PID loop.
	 */
	pid.PFactor = (int)(PFactor*(float)pid.ScalingFactor);
	pid.IFactor = (int)(IFactor*(float)pid.ScalingFactor);
	pid.DFactor = (int)(DFactor*(float)pid.ScalingFactor);

	/*
	 *  Limits to avoid overflow.
	 */
	pid.MaxError = INT_MAX / (pid.PFactor + 1);
	pid.MaxSumError = PID_MAX / (pid.IFactor + 1);
}

/**
 * \brief Executes the PID algorithm
 *
 * Calculates the error from the setpoint and multiplies by the proportional parameter, adds the error to the error sum and multiplies by the integral parameter and calculates the
 * difference from the last iteration value with the current value and multiplies by the derivative parameter. Then scales the result by a ramp function.
 *
 * \param SetPoint as the desired value of the process
 * \param ProcessValue is the current value of the process
 * \param pid is a pointer to a struct containing the PID parameters
 *
 * \returns the control value to be used in the process
 */

float Pid_Control(int SetPoint, float ProcessValue, struct Pid *pid)
{
	volatile float PTerm, DTerm;
	volatile float ITerm;
	volatile float Temp;
	volatile float Ret,Error;

	Error = SetPoint - ProcessValue;

	/*
	 * Calculate Pterm and limit error overflow.
	 */
	if (Error > pid->MaxError) {
		PTerm = INT_MAX;
	} else if (Error < -pid->MaxError) {
		PTerm = -INT_MAX;
	} else {
		PTerm = pid->PFactor * Error;
	}

	/*
	 * Calculate ITerm and limit integral runaway.
	 */
	Temp = pid->SumError + Error;

	if (Temp > pid->MaxSumError) {
		ITerm = MAX_I_TERM;
		pid->SumError = pid->MaxSumError;
	} else if (Temp < -pid->MaxSumError) {
		ITerm = -MAX_I_TERM;
		pid->SumError = -pid->MaxSumError;
	} else {
		pid->SumError = Temp;
		ITerm = pid->IFactor * pid->SumError;
	}

	/*
	 * Calculate DTerm.
	 */
	DTerm = pid->DFactor * (pid->LastProcessValue - ProcessValue);

	pid->LastProcessValue = ProcessValue;

	Ret = (PTerm + ITerm + DTerm) / pid->ScalingFactor;

	Ret = 0.0001221*Ret + 0.5;

	if(Ret > 1) Ret = 1;
	if(Ret < 0) Ret = 0;


	return (Ret);
}
