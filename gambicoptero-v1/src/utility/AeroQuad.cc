#include<utility/AeroQuad.h>

__USING_SYS

short AeroQuad::_headingHoldConfig   = 1;
float AeroQuad::_headingHold         = 0; // calculated adjustment for quad to go to heading (PID output)
float AeroQuad::_heading             = 0; // measured heading from yaw gyro (process variable)
float AeroQuad::_relativeHeading     = 0; // current heading the quad is set to (set point)
short AeroQuad::_headingHoldState   = OFF;

short AeroQuad::calibrateESC = 0;
int AeroQuad::testCommand = 1000;

short AeroQuad::RATE_FLIGHT_MODE      = 0;
short AeroQuad::ATTITUDE_FLIGHT_MODE  = 1;


const int AeroQuad::ALTPANIC  = 2;
const int AeroQuad::ON =  1;
const int AeroQuad::OFF = 0;
const int AeroQuad::TASK_100HZ = 1;
const int AeroQuad::TASK_50HZ = 2;
const int AeroQuad::TASK_10HZ = 10;
const int AeroQuad::THROTTLE_ADJUST_TASK_SPEED = TASK_50HZ;

short AeroQuad::_flightMode = 1;//ATTITUDE_FLIGHT_MODE;

unsigned long AeroQuad::frameCounter = 0; // main loop executive frame counter

float AeroQuad::_G_Dt = 0.002;
int AeroQuad::_throttle = 1000;
//unsigned char AeroQuad::_motorArmed = false;
unsigned char AeroQuad::_safetyCheck = false;
unsigned char AeroQuad::_maxLimit = false;
unsigned char AeroQuad::_minLimit = false;
float AeroQuad::_filteredAccel[3] = {0.0,0.0,0.0};

unsigned long AeroQuad::_previousTime = 0;
unsigned long AeroQuad::_currentTime = 0;
unsigned long AeroQuad::_deltaTime = 0;
// sub loop time variable
unsigned long AeroQuad::tenHZpreviousTime = 0;
unsigned long AeroQuad::fiftyHZpreviousTime = 0;
unsigned long AeroQuad::hundredHZpreviousTime = 0;

const int AeroQuad::minThrottleAdjust = -50;
const int AeroQuad::maxThrottleAdjust = 50;
const int AeroQuad::minArmedThrottle = 1150;


const short AeroQuad::XAXIS = 0;
const short AeroQuad::YAXIS = 1;
const short AeroQuad::ZAXIS = 2;
