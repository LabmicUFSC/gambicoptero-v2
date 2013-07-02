#ifndef FLIGHT_CONTROL_H_
#define FLIGHT_CONTROL_H_

#include "accelerometer.h"
#include "gyroscope.h"
#include "motor.h"
#include "pid.h"
#include "kinematics.h"

class FlightControl
{

enum State {
	INIT, MEASURING_SENSORS, UPDATING_PID, SETTING_MOTORS, MOVING
};


	State state;
	bool moving;
	MovementDirection direction;
	double clock;

	Accelerometer * accelerometer;
	Gyroscope * gyroscope;
	Motor * motor;
	PID * pid;
	Kinematics * kinematics;

public:
	FlightControl(Accelerometer * accelerometer, Gyroscope * gyroscope, Motor * motor, PID * pid, Kinematics * kinematics) :
					accelerometer(accelerometer), gyroscope(gyroscope), motor(motor), pid(pid),
					kinematics(kinematics), state(INIT), clock(0.0f)
	{
		
	};
	virtual ~FlightControl();

	static void loop();
	static bool is100Hz(){
		return true;
	};
};
#endif