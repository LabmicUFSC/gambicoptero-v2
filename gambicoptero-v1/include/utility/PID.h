
#ifndef PID_H
#define PID_H

#include<utility/AQMath.h>
#include<utility/AeroQuad.h>
#include<machine.h>

__BEGIN_SYS

class PIDclass {

    public:

    static const int RATE_XAXIS_PID_IDX       =   0;
    static const int RATE_YAXIS_PID_IDX       =   1;
    static const int ZAXIS_PID_IDX            =   2;
    static const int ATTITUDE_XAXIS_PID_IDX   =   3;
    static const int ATTITUDE_YAXIS_PID_IDX   =   4;
    static const int HEADING_HOLD_PID_IDX     =   5;
    static const int ATTITUDE_GYRO_XAXIS_PID_IDX= 6;
    static const int ATTITUDE_GYRO_YAXIS_PID_IDX= 7;
    static const int ALTITUDE_HOLD_PID_IDX      = 8;
    static const int ZDAMPENING_PID_IDX         = 9;

    // PID Variables

    struct PIDdata {
        float P, I, D;
        float lastPosition;
        // AKA experiments with PID
        float previousPIDTime;
        float integratedError;
        float windupGuard; // Thinking about having individual wind up guards for each PID
    } PID[10];
    // This struct above declares the variable PID[] to hold each of the PID values for various functions
    // The following constants are declared in AeroQuad.h
    // ROLL = 0, PITCH = 1, YAW = 2 (used for Arcobatic Mode, gyros only)
    // ROLLLEVEL = 3, PITCHLEVEL = 4, LEVELGYROROLL = 6, LEVELGYROPITCH = 7 (used for Stable Mode, accels + gyros)
    // HEADING = 5 (used for heading hold)
    // ALTITUDE = 8 (used for altitude hold)
    // ZDAMPENING = 9 (used in altitude hold to dampen vertical accelerations)
    float windupGuard; // Read in from EEPROM

    // Modified from http://www.arduino.cc/playground/Main/BarebonesPIDForEspresso
    float updatePID(float targetPosition, float currentPosition,
                    int pidIDX);// {//struct PIDdata *PIDparameters) {

    int printPid(const PIDdata * pidData);
    //void zeroIntegralError() __attribute__ ((noinline));
    void zeroIntegralError();

};

__END_SYS
#endif // PID_H
