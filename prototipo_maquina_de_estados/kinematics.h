#ifndef KINEMATICS_H_
#define KINEMATICS_H_

class Kinematics
{
public:
	Kinematics(){};
	virtual ~Kinematics(){};

	void calculateMotorThrust(double * pidResult, unsigned char * motor);
};

#endif