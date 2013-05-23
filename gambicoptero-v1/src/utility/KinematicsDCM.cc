
#include<utility/Kinematics_DCM.h>
#include<utility/AQMath.h>
__USING_SYS



void KinematicsDCM::calculateKinematics(float rollRate,            float pitchRate,      float yawRate,
                         float longitudinalAccel,   float lateralAccel,   float verticalAccel,
                         float oneG,                float magX,           float magY,
                         float G_Dt) {

    matrixUpdate(rollRate, pitchRate, yawRate, G_Dt);
    normalize();
    driftCorrection(longitudinalAccel, lateralAccel, verticalAccel, oneG, magX, magY);
    eulerAngles();
    earthAxisAccels(longitudinalAccel, lateralAccel, verticalAccel, oneG);
}


void KinematicsDCM::initializeKinematics(float hdgX, float hdgY)
{
    initializeBaseKinematicsParam(); //(hdgX,hdgY);

    db<KinematicsDCM> (TRC) << "--- initializeKinematics --- " << endl;
    for (unsigned char i=0; i<3; i++) {
        omegaP[i] = 0;
        omegaI[i] = 0;
    }
    dcmMatrix[0] =  hdgX;
    dcmMatrix[1] = -hdgY;
    dcmMatrix[2] =  0;
    dcmMatrix[3] =  hdgY;
    dcmMatrix[4] =  hdgX;
    dcmMatrix[5] =  0;
    dcmMatrix[6] =  0;
    dcmMatrix[7] =  0;
    dcmMatrix[8] =  1;

    kpRollPitch = 0.1;        // alternate 0.05;
    kiRollPitch = 0.0002;     // alternate 0.0001;

    kpYaw = -0.1;             // alternate -0.05;
    kiYaw = -0.0002;          // alternate -0.0001;

}


KinematicsDCM::KinematicsDCM():errorCourse(0),
    kpRollPitch(0.0), kpYaw(0.0), kiYaw(0.0){
    for (int i = 0 ; i < 9; i++)
        dcmMatrix[i] = 0;

    for (int i = 0; i < 3; i++){
        omegaP[i] = 0;
        omegaI[i] = 0;
        omega[i]  = 0;
    }

}

void KinematicsDCM::matrixUpdate(float p, float q, float r, float G_Dt){

    float rateGyroVector[3];
    float temporaryMatrix[9];
    float updateMatrix[9];

    rateGyroVector[AeroQuad::XAXIS]  = p;
    rateGyroVector[AeroQuad::YAXIS] = q;
    rateGyroVector[AeroQuad::ZAXIS]   = r;

    AQMath::vectorSubtract(3, &omega[AeroQuad::XAXIS],
                   &rateGyroVector[AeroQuad::XAXIS],
                   &omegaI[AeroQuad::XAXIS]);

    AQMath::vectorSubtract(3, &correctedRateVector[AeroQuad::XAXIS],
                   &omega[AeroQuad::XAXIS],
                   &omegaP[AeroQuad::XAXIS]);

    //Accel_adjust();//adjusting centrifugal acceleration. // Not used for quadcopter

    updateMatrix[0] =  0;
    updateMatrix[1] = -G_Dt * correctedRateVector[AeroQuad::ZAXIS];    // -r
    updateMatrix[2] =  G_Dt * correctedRateVector[AeroQuad::YAXIS];  //  q
    updateMatrix[3] =  G_Dt * correctedRateVector[AeroQuad::ZAXIS];    //  r
    updateMatrix[4] =  0;
    updateMatrix[5] = -G_Dt * correctedRateVector[AeroQuad::XAXIS];   // -p
    updateMatrix[6] = -G_Dt * correctedRateVector[AeroQuad::YAXIS];  // -q
    updateMatrix[7] =  G_Dt * correctedRateVector[AeroQuad::XAXIS];   //  p
    updateMatrix[8] =  0;

    AQMath::matrixMultiply(3, 3, 3, temporaryMatrix, dcmMatrix, updateMatrix);
    AQMath::matrixAdd(3, 3, dcmMatrix, dcmMatrix, temporaryMatrix);
}


void KinematicsDCM::normalize(){
    float error=0;
    float temporary[9];
    float renorm=0;

    error= -AQMath::vectorDotProduct(3, &dcmMatrix[0], &dcmMatrix[3]) * 0.5;         // eq.18

    AQMath::vectorScale(3, &temporary[0], &dcmMatrix[3], error);                     // eq.19
    AQMath::vectorScale(3, &temporary[3], &dcmMatrix[0], error);                     // eq.19

    AQMath::vectorAdd(6, &temporary[0], &temporary[0], &dcmMatrix[0]);               // eq.19

    AQMath::vectorCrossProduct(&temporary[6],&temporary[0],&temporary[3]);           // eq.20

    for(unsigned char v=0; v<9; v+=3) {
        renorm = 0.5 *(3 - AQMath::vectorDotProduct(3, &temporary[v],&temporary[v]));  // eq.21
        AQMath::vectorScale(3, &dcmMatrix[v], &temporary[v], renorm);
    }
}

void KinematicsDCM::driftCorrection(float ax, float ay,
                                    float az, float oneG,
                                    float magX, float magY){
    //  Compensation of the Roll, Pitch and Yaw drift.
    float accelMagnitude;
    float accelVector[3];
    float accelWeight;
    float errorRollPitch[3];
#ifdef HeadingMagHold
    float errorCourse;
    float errorYaw[3];
    float scaledOmegaP[3];
#endif
    float scaledOmegaI[3];

    //  Roll and Pitch Compensation
    accelVector[AeroQuad::XAXIS] = ax;
    accelVector[AeroQuad::YAXIS] = ay;
    accelVector[AeroQuad::ZAXIS] = az;

    // Calculate the magnitude of the accelerometer vector
    accelMagnitude = (Math::sqrt(accelVector[AeroQuad::XAXIS] * accelVector[AeroQuad::XAXIS] +
                                 accelVector[AeroQuad::YAXIS] * accelVector[AeroQuad::YAXIS] +
                                 accelVector[AeroQuad::ZAXIS] * accelVector[AeroQuad::ZAXIS])) / oneG;

    // Weight for accelerometer info (<0.75G = 0.0, 1G = 1.0 , >1.25G = 0.0)
    // accelWeight = constrain(1 - 4*abs(1 - accelMagnitude),0,1);

    // Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
    accelWeight = AQMath::constrain(1 - 2 * Math::abs(1 - accelMagnitude), 0, 1);

    AQMath::vectorCrossProduct(&errorRollPitch[0], &accelVector[0], &dcmMatrix[6]);
    AQMath::vectorScale(3, &omegaP[0], &errorRollPitch[0], kpRollPitch * accelWeight);

    AQMath::vectorScale(3, &scaledOmegaI[0], &errorRollPitch[0], kiRollPitch * accelWeight);
    AQMath::vectorAdd(3, omegaI, omegaI, scaledOmegaI);

    //  Yaw Compensation
#ifdef HeadingMagHold
    errorCourse = (dcmMatrix[0] * magY) - (dcmMatrix[3] * magX);
    AQMath::vectorScale(3, errorYaw, &dcmMatrix[6], errorCourse);

    AQMath::vectorScale(3, &scaledOmegaP[0], &errorYaw[0], kpYaw);
    AQMath::vectorAdd(3, omegaP, omegaP, scaledOmegaP);

    AQMath::vectorScale(3, &scaledOmegaI[0] ,&errorYaw[0], kiYaw);
    AQMath::vectorAdd(3, omegaI, omegaI, scaledOmegaI);
#else
    omegaP[AeroQuad::ZAXIS] = 0.0;
    omegaI[AeroQuad::ZAXIS] = 0.0;
#endif
}

void KinematicsDCM::earthAxisAccels(float ax, float ay, float az, float oneG){
    float accelVector[3];

    accelVector[AeroQuad::XAXIS] = ax;
    accelVector[AeroQuad::YAXIS] = ay;
    accelVector[AeroQuad::ZAXIS] = az;

    earthAccel[AeroQuad::XAXIS] = AQMath::vectorDotProduct(3, &dcmMatrix[0], &accelVector[0]);
    earthAccel[AeroQuad::YAXIS] = AQMath::vectorDotProduct(3, &dcmMatrix[3], &accelVector[0]);
    earthAccel[AeroQuad::ZAXIS] = AQMath::vectorDotProduct(3, &dcmMatrix[6], &accelVector[0]) + oneG;
}
