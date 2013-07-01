#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_


class Gyroscope
{
public:
	Gyroscope(){};
	virtual ~Gyroscope(){};

	
	bool measure(double * values)
	{
		values[0] = 0.0f;
		values[1] = 0.0f;
		values[2] = 0.0f;
		return true;
	}
};
#endif