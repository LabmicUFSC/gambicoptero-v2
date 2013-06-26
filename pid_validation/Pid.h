#ifndef PID_H_
#define PID_H_

#include "AeroQuad.h"

class Pid
{
	struct PIDdata {
        float P, I, D;
        float lastPosition;
        // AKA experiments with PID
        float previousPIDTime;
        float integratedError;
        float windupGuard; // Thinking about having individual wind up guards for each PID

        PIDdata(const float P = 0.0f, const float I = 0.0f, const float D = 0.0f):P(P),I(I),D(D), lastPosition(0.0f), previousPIDTime(0.0f), integratedError(0.0f), windupGuard(0.375f){};
    } PID[10];

public:
	Pid(const float P, const float I, const float D);
	virtual ~Pid();

	float updatePid(float targetPosition, float currentPosition, int pidIDX);

	/* data */
};
#endif