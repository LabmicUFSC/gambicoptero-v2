// EPOS Math Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __math_h
#define __math_h

#include <system/config.h>

__BEGIN_SYS

namespace Math {

static const float E = 2.71828183;
static const float PI =  3.14159265;
static const float PIO2 = 1.570796326;
static const float nan = (0.0/0.0);


int findMedianInt(short *data, int arraySize);
float filterSmooth(float currentData, float previousData, float smoothFactor);

static const double sq2p1 = 2.414213562373095048802e0;
   static const double sq2m1  = .414213562373095048802e0;


   static const double p4  = .161536412982230228262e2;
       static const double p3  = .26842548195503973794141e3;
       static const double p2  = .11530293515404850115428136e4;
       static const double p1  = .178040631643319697105464587e4;
       static const double p0  = .89678597403663861959987488e3;
       static const double q4  = .5895697050844462222791e2;
       static const double q3  = .536265374031215315104235e3;
       static const double q2  = .16667838148816337184521798e4;
       static const double q1  = .207933497444540981287275926e4;
       static const double q0  = .89678597403663861962481162e3;

float logf(float num, float base = E , float epsilon = 1e-12) ;
float radians(float);
float degrees(float radians);

double mxatan(double arg);

double msatan(double arg);

float abs(float x) ;

float sqrt(float x);

double atan(double arg);

float pow(float num, float expo);

float factorial(int num);

float cosineTaylor(float num, float precision = 10);

double atan2(double arg1, double arg2) ;

double asin(double arg);


}


__END_SYS

#endif
