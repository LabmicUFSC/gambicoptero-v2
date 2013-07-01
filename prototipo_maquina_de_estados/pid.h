#ifndef PID_H_
#define PID_H_

enum MovementDirection {
	UP, DOWN, FRONT, BACK, LEFT, RIGHT
};


class PID
{
public:
	PID(){};
	virtual ~PID(){};

	void movement(MovementDirection direction, double * accelerometerResult, double * gyroscopeResult, double * pidResult);
	void stable(double * accelerometerResult, double * gyroscopeResult, double * pidResult);
};

#endif