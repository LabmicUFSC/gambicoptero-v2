#include<utility/Kinematics.h>

__USING_SYS

Kinematics::Kinematics():CF(0), KF(1), DCM(2),
    ARG(3), MARG(4), kinematicsType(0), accelCutoff(0.0){

    for (int i = 0; i < 3; i++){
        kinematicsAngle[i] = 0;
        correctedRateVector[i] = 0;
        earthAccel[i] = 0;
    }

    gyroAngle[0] = 0;
    gyroAngle[1] = 0;
}

void Kinematics::initializeBaseKinematicsParam() {
    db<Kinematics>(TRC) << "--- Initialize Base Kinematics Param ---" << endl;

    for (unsigned char axis = AeroQuad::XAXIS; axis <= AeroQuad::ZAXIS; axis++)
        kinematicsAngle[axis] = 0.0;
    gyroAngle[AeroQuad::XAXIS] = 0;
    gyroAngle[AeroQuad::YAXIS] = 0;
}
