#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

class Accelerometer
{
public:
	Accelerometer() {};
	virtual ~Accelerometer(){};

	bool measure(double * values)
	{
		values[0] = 1.0f;
		values[1] = 1.0f;
		values[2] = 1.0f;
		return true;
	}
};

#endif