#ifndef KINEMATICS_DCM_H
#define KINEMATICS_DCM_H

#include<utility/AeroQuad.h>
#include<utility/Kinematics.h>
#include <utility/math.h>

__BEGIN_SYS

class KinematicsDCM : public Kinematics {

    float dcmMatrix[9];
    float omegaP[3];// = {0.0,0.0,0.0};
    float omegaI[3];// = {0.0,0.0,0.0};
    float omega[3] ;//= {0.0,0.0,0.0};
    float errorCourse;// = 0.0;
    float kpRollPitch;// = 0.0;
    float kiRollPitch;// = 0.0;
    float kpYaw;// = 0.0;
    float kiYaw;// = 0.0;

    ////////////////////////////////////////////////////////////////////////////////
    // Matrix Update
    ////////////////////////////////////////////////////////////////////////////////
public:
    KinematicsDCM();

    void matrixUpdate(float p, float q, float r, float G_Dt);

    ////////////////////////////////////////////////////////////////////////////////
    // Normalize
    ////////////////////////////////////////////////////////////////////////////////
    void normalize();

    ////////////////////////////////////////////////////////////////////////////////
    // Drift Correction
    ////////////////////////////////////////////////////////////////////////////////

    void driftCorrection(float ax, float ay, float az, float oneG, float magX, float magY);

    ////////////////////////////////////////////////////////////////////////////////
    // Accel Adjust
    ////////////////////////////////////////////////////////////////////////////////

    /*void Accel_adjust(void) {
  // ADC : Voltage reference 3.0V / 10bits(1024 steps) => 2.93mV/ADC step
  // ADXL335 Sensitivity(from datasheet) => 330mV/g, 2.93mV/ADC step => 330/0.8 = 102
  #define GRAVITY 102 //this equivalent to 1G in the raw data coming from the accelerometer
  #define Accel_Scale(x) x*(GRAVITY/9.81)//Scaling the raw data of the accel to actual acceleration in meters for seconds square

  accelVector[1] += Accel_Scale(speed_3d*omega[2]);  // Centrifugal force on Acc_y = GPS_speed*GyroZ
  accelVector[2] -= Accel_Scale(speed_3d*omega[1]);  // Centrifugal force on Acc_z = GPS_speed*GyroY
}*/

    ////////////////////////////////////////////////////////////////////////////////
    // Euler Angles
    ////////////////////////////////////////////////////////////////////////////////

    void eulerAngles(void)
    {
        kinematicsAngle[AeroQuad::XAXIS]  =  Math::atan2(dcmMatrix[7], dcmMatrix[8]);
        kinematicsAngle[AeroQuad::YAXIS] =  -Math::asin(dcmMatrix[6]);
        kinematicsAngle[AeroQuad::ZAXIS]   =  Math::atan2(dcmMatrix[3], dcmMatrix[0]);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Earth Axis Accels
    ////////////////////////////////////////////////////////////////////////////////

    void earthAxisAccels(float ax, float ay, float az, float oneG);



    ////////////////////////////////////////////////////////////////////////////////
    // Initialize DCM
    ////////////////////////////////////////////////////////////////////////////////

    virtual void initializeKinematics(float hdgX, float hdgY);

    ////////////////////////////////////////////////////////////////////////////////
    // Calculate DCM
    ////////////////////////////////////////////////////////////////////////////////

    virtual void calculateKinematics(float rollRate,            float pitchRate,      float yawRate,
                             float longitudinalAccel,   float lateralAccel,   float verticalAccel,
                             float oneG,                float magX,           float magY,
                             float G_Dt);
    void calibrateKinematics() {};

};

__END_SYS
#endif // KINEMATICS_DCM_H
