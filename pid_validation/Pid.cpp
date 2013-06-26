#include "Pid.h"
#include <stdio.h>

Pid::Pid(const float P, const float I, const float D)
{
	for(unsigned i = 0; i < 10; i++)
	{
		PID[i] = PIDdata(P, I, D);
	}
}

Pid::~Pid()
{

}


float Pid::updatePid(float targetPosition, float currentPosition, int pidIDX)
{
    PIDdata * PIDparameters = &PID[pidIDX];
	const float deltaPIDTime = (AeroQuad::_currentTime - PIDparameters->previousPIDTime) /*/ 1000000.0*/;
	
	PIDparameters->previousPIDTime = AeroQuad::_currentTime; // AKA PID experiments
	float error = targetPosition - currentPosition;

	PIDparameters->integratedError += error * deltaPIDTime;
	// PIDparameters->integratedError = AQMath::constrain(PIDparameters->integratedError, -PIDparameters->windupGuard, PIDparameters->windupGuard);
	float dTerm = PIDparameters->D * (currentPosition - PIDparameters->lastPosition)  / (deltaPIDTime /** 100*/); // dT fix from Honk
	PIDparameters->lastPosition = currentPosition;
	float pid = (PIDparameters->P * error) + (PIDparameters->I * (PIDparameters->integratedError)) + dTerm;
	// return AQMath::constrain(pid, -20.0f, 20.0f);
	return pid;
}
