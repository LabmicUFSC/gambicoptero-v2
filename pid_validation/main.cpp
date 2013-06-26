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
	const float startTime = 2.5f;
	const float duration = 30.0f;
	const float finalTime = startTime + duration;
	AeroQuad::_currentTime = startTime;

	Pid pid(5.0f, 1.0f, -10.0f);
	Kinematics sensor(0.0f, 0.0f, 0.0f);

	const float targetPosition = 3.0f;
	float current = 0.0f;
	float pidValue;
	float epsilon = 0.01;

	int k = 0;
	int i = 0;

	fstream out;
	out.open("pid.dat", fstream::out);
	// while( fabs(current - targetPosition) > epsilon )
	out << AeroQuad::_currentTime << "\t0.00\t0.00" << endl;
	while( AeroQuad::_currentTime < finalTime )
	{	
		AeroQuad::_currentTime += 0.010;
		current = sensor.getPos();	
		out << AeroQuad::_currentTime << "\t" << current << "\t" <<targetPosition <<  endl;
		pidValue = pid.updatePid(targetPosition, current, 0);
		// cout << "inst " << AeroQuad::_currentTime << " pid = " << pidValue << endl;
		sensor.setAccel(pidValue);
		sensor.updateTime(0.010f);
	}
	out.close();


	return 0;
}
