#ifndef MOTOR_H_
#define MOTOR_H_

class Motor
{


	double value;

public:
	Motor();
	virtual ~Motor();

	void setThrust(double value) { this->value=value; }
};

#endif