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
	const float duration = 60.0f;
	const float finalTime = startTime + duration;
	AeroQuad::_currentTime = startTime;

	Pid pid[3] = {
		Pid(2.5f, 1.0f, -10.0f),
		Pid(5.0f, 2.0f, -20.0f),
		Pid(10.0f, 4.0f, -40.0f),
	};
	Kinematics sensor[3] = {
		Kinematics(0.0f, 0.0f, 0.0f),
		Kinematics(0.0f, 0.0f, 0.0f),
		Kinematics(0.0f, 0.0f, 0.0f)
	};

	const float targetPosition = 3.0f;

	float current = 0.0f;
	float pidValue;
	float epsilon = 0.01;

	int k = 0;
	int i = 0;

	fstream out;
	out.open("pid.dat", fstream::out);
	// while( fabs(current - targetPosition) > epsilon )
	out << AeroQuad::_currentTime << "\t0.00\t0.00\t0.00\t0.0" << endl;
	while( AeroQuad::_currentTime < finalTime )
	{	
		AeroQuad::_currentTime += 0.010;
		out << AeroQuad::_currentTime;
		for(int i = 0; i < 3; i++)
		{
			current = sensor[i].getPos();	

			pidValue = pid[i].updatePid(targetPosition, current, 0);
			// cout << "inst " << AeroQuad::_currentTime << " pid = " << pidValue << endl;
			sensor[i].setAccel(pidValue);
			sensor[i].updateTime(0.010f);
			out << "\t" << current;
		}
		out << "\t" << targetPosition << endl;
	}
	out.close();


	return 0;
}
