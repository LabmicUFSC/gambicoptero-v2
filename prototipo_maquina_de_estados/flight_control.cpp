#include "flight_control.h"

FlightControl::~FlightControl()
{

}

static void FlightControl::loop()
{

	double accelerometerResult[3];
	double gyroscopeResult[3];
	double pidResult[6];
	unsigned char motor[4];
	while(1)
	{
		switch(state)
		{
			case INIT:
				state = MEASURING_SENSORS;

			case MEASURING_SENSORS:
				if(gyro->measure(gyroscopeResult) && accelerometerResult->measure(accelerometerResult))
					state = UPDATING_PID;
				else
					state = PANIC;

			case UPDATING_PID:
				if(moving)
					pid->movement(direction, accelerometerResult, gyroscopeResult, pidResult);
				else
					pid->stable(accelerometerResult, gyroscopeResult, pidResult);

			case SETTING_MOTORS:
				kinematics->calculateMotorThrust(pidResult, motor);
				// manda o array motor pra rede
				// Quadcopter_Network::set_data_feedback(motor[0],motor[1], motor[2], motor[3]);
				motor->setMotor(motor[0]);

			case PANIC:

			default:

		}	
	}
}