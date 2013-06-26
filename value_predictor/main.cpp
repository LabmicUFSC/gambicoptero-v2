#include <iostream>
using std::cout;
using std::endl;

#include <cmath>
using std::fabs;

#include <fstream>
using std::fstream;

#include "Predictor.h"


int main(int argc, char const *argv[])
{
	Predictor pred;

	fstream out;
	out.open("pred.dat", fstream::out);
	pred.new_value(3, 1);
	out << 1 << "\t" << 3 << endl;
	pred.new_value(2, 2);
	out << 2 << "\t" << 2 << endl;
	pred.new_value(1, 3);
	out << 3 << "\t" << 1 << endl;
	
	out << 4 << "\t" << pred.get_prediction3(4) << endl;
		

	return 0;
}
