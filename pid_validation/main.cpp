#include <iostream>
using std::cout;
using std::endl;

#include <cmath>
using std::fabs;

#include <fstream>
using std::fstream;

#include "Pid.h"
#include "AeroQuad.h"
#include "Kinematics.h"


int main(int argc, char const *argv[])
{

	cout << "Hello World" << endl;
	AeroQuad::_currentTime = 0.0f;

	Pid pid(1.0f, 0.1f, -0.4f);
	Kinematics sensor(0.0f, 0.0f, 0.0f);

	const float targetPosition = 10.0f;
	float current = 0.0f;
	float pidValue;
	float epsilon = 0.01;

	int k = 0;
	int i = 0;

	fstream out;
	out.open("pid.dat", fstream::out);
	// while( fabs(current - targetPosition) > epsilon )
	while( AeroQuad::_currentTime < 100.0f )
	{	
		AeroQuad::_currentTime += 0.010;
		current = sensor.getPos();
		cout << current << endl;	
		out << AeroQuad::_currentTime << "\t" << current << endl;
		pidValue = pid.updatePid(targetPosition, current, 0);
		cout << "inst " << AeroQuad::_currentTime << " pid = " << pidValue << endl;
		sensor.setAccel(pidValue);
		sensor.updateTime(0.010f);
	}
	out.close();


	return 0;
}
