#ifndef KINE_H_
#define KINE_H_

class Kinematics
{
public:
	Kinematics(float initPos, float initVelo, float initAccel);
	virtual ~Kinematics();

	float getPos();
	void setAccel(float accel);
	void updateTime(float amount);
	/* data */

	float position, velocity, acceleration;
};
#endif
