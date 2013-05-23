#ifndef KINEMATICS_H
#define KINEMATICS_H

#include<utility/math.h>
#include "AeroQuad.h"
// This class is responsible for calculating vehicle attitud

__BEGIN_SYS

class Kinematics{

public:
    const int CF;
    const int KF;
    const int DCM ;
    const int ARG ;
    const int MARG ;
    unsigned char kinematicsType;
    float accelCutoff ;
    float kinematicsAngle[3];
    float gyroAngle[2];
    float correctedRateVector[3];
    float earthAccel[3];

    Kinematics();
    void initializeBaseKinematicsParam();
    virtual void initializeKinematics(float hdgX, float hdgY) = 0;

    virtual void calculateKinematics(float rollRate,           float pitchRate,     float yawRate,
                             float longitudinalAccel,  float lateralAccel,  float verticalAccel,
                             float oneG,               float magX,          float magY,
                             float G_Dt) = 0;

    float getGyroUnbias(unsigned axis);
    void calibrateKinematics();

    // returns the kinematicsAngle of a specific axis in SI units (radians)
    //  const float getData(byte axis) {
    //    return kinematicsAngle[axis];
    //  }
    // return heading as +PI/-PI
    //  const float getHeading(byte axis) {
    //    return(kinematicsAngle[axis]);
    //  }

    // This really needs to be in Radians to be consistent
    // I'll fix later - AKA
    // returns heading in degrees as 0-360
    const float kinematicsGetDegreesHeading(unsigned char axis) {
        float tDegrees;

        tDegrees = Math::degrees(kinematicsAngle[axis]);
        if (tDegrees < 0.0)
            return (tDegrees + 360.0);
        else
            return (tDegrees);
    }

};
__END_SYS
#endif // KINEMATICS_H
