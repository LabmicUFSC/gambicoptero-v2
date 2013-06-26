#include <iostream>
using std::cout;
using std::endl;

#include <cmath>
using std::fabs;

#include <fstream>
using std::fstream;

#include "Pid.h"
#include "AeroQuad.h"


int main(int argc, char const *argv[])
{

	cout << "Hello World" << endl;
	AeroQuad::_currentTime = 0.0f;

	Pid pid(30.0f, 0.0f, 0.0f);

	const float targetPosition = 100.0f;
	float current = 0.0f;
	float pidValue;
	float epsilon = 0.01;

	int k = 0;
	int i = 0;

	fstream out;
	out.open("pid.dat", fstream::out);
	// while( fabs(current - targetPosition) > epsilon )
	while( AeroQuad::_currentTime < 2000.0f )
	{	
		AeroQuad::_currentTime += 0.1;
		if(i % 100 == 0)
		{
			cout << "(time, position) = (" << AeroQuad::_currentTime << ", " << current << ")" << endl;
			out << AeroQuad::_currentTime << "\t" << current << endl;
			pidValue = pid.updatePid(targetPosition, current, 0);
			current += pidValue / 500;
			k++;
		}
		i++;
	}
	out.close();


	return 0;
}