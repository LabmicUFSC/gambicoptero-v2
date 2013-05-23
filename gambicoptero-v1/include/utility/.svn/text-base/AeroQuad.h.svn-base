#ifndef AEROQUAD_H
#define AEROQUAD_H

#include<machine.h>

__BEGIN_SYS

class AeroQuad {
public:
    // Analog Reference Value
    // This value provided from Configurator
    // Use a DMM to measure the voltage between AREF and GND
    // Enter the measured voltage below to define your value for aref
    // If you don't have a DMM use the following:
    // AeroQuad Shield v1.7, aref = 3.0
    // AeroQuad Shield v1.6 or below, aref = 2.8
    //float aref;
    /**
     * Heading and heading hold global declaration section
     */
    static short _headingHoldConfig;
    static float _headingHold; // calculated adjustment for quad to go to heading (PID output)
    static float _heading; // measured heading from yaw gyro (process variable)
    static float _relativeHeading; // current heading the quad is set to (set point)
    static short  _headingHoldState;

    //////////////////////////////////////////////////////

    /**
     * ESC calibration process global declaration
     */
    static short calibrateESC;
    static int testCommand;

    /**
     * Flight control global declaration
     */
    static short RATE_FLIGHT_MODE;
    static short ATTITUDE_FLIGHT_MODE;

    static const int TASK_100HZ;
    static const int TASK_50HZ;
    static const int TASK_10HZ;
    static const int THROTTLE_ADJUST_TASK_SPEED;

    static short _flightMode;

    static unsigned long frameCounter; // main loop executive frame counter
    static const int minArmedThrottle; // initial value configured by user

    static float _G_Dt;
    static int _throttle;
   // static unsigned char _motorArmed ;
    static unsigned char _safetyCheck;
    static unsigned char _maxLimit ;
    static unsigned char _minLimit;
    static float _filteredAccel[3];

    // main loop time variable
    static unsigned long _previousTime;
    static unsigned long _currentTime;
    static unsigned long _deltaTime;
    // sub loop time variable
    static unsigned long tenHZpreviousTime;
    static unsigned long fiftyHZpreviousTime;
    static unsigned long hundredHZpreviousTime ;

    static const int minThrottleAdjust;
    static const int maxThrottleAdjust;

    static const int ON;
    static const int OFF;
    static const int ALTPANIC;
    static  const int  INVALID_ALTITUDE = -1000.0;

    static const short XAXIS;
    static const short YAXIS;
    static const short ZAXIS;
};

__END_SYS
#endif // AEROQUAD_H
