//#include "mach/mc13224v/motor.h"
#include <sensor.h>
#include <mach/mc13224v/adxl345.h>
#include <mach/mc13224v/i2c.h>
#include <utility/PID.h>
#include<utility/Kinematics_DCM.h>
#include<utility/ostream.h>
#include<utility/math.h>
#include<utility/motors.h>
__USING_SYS

OStream cout;


void setup(MC13224V_ADXL345 * ac) {

  Alarm::delay(3000000);

  cout << "Accelerometer library test (ADXL345_TEST)" << endl;
  ac->initializeAccel(MC13224V_ADXL345::FULLRANGE, MC13224V_ADXL345::SAMPLER_RATE_200HZ);
  ac->computeAccelBias();
// Motors::startMotors();
}

void loop(MC13224V_ADXL345 * ac) {

    const int microseconds = 1000000;
  //if((millis() - timer) > 10) // 100Hz
  //Alarm::delay(2000000);
  //  timer = millis();
   ac->measureAccelSum();
   ac->evaluateMetersPerSec();

    int axmeter = ac->meterPerSecSec[0];
    int aymeter = ac->meterPerSecSec[1];
    int azmeter = ac->meterPerSecSec[2];
   /* cout << "Roll: ";
    cout << ac->meterPerSecSec[0];
    cout << " Pitch: ";
    cout << ac->meterPerSecSec[1];
    cout << " Yaw: ";
    cout << ac->meterPerSecSec[2] << endl;
    //cout << endl << endl << endl;
*/
    int ax = ac->sample_x();
    int ay = ac->sample_y();
    int az = ac->sample_z();
    float aux = (180.0/Math::PI);
    int ax2 = ax* ax;
    int ay2 = ay*ay;
    int az2 = az*az;

    float sqrt_ay2az2 = Math::sqrt(ay2 + az2);
    float sqrt_aymazm = Math::sqrt(aymeter *aymeter+ azmeter*azmeter);

    float atan_sqrt_ay2az2 = Math::atan(sqrt_ay2az2);
    float atan_sqrt_aym2azm2 = Math::atan(sqrt_aymazm);
    float alfa = atan_sqrt_ay2az2 * aux;
    float alfa2 = atan_sqrt_aym2azm2 * aux;
    alfa = Math::atan(ax/(Math::sqrt((ay*ay) + (az * az)) )) * (180.0/Math::PI);
    float beta = Math::atan(ay/(Math::sqrt(ax * ax + az * az) ) ) * (180.0/Math::PI);
    float gama = Math::atan(az/(Math::sqrt(ay * ay + ax * ax))/az ) * (180.0/Math::PI);
    cout << "alfa: " << alfa << "  beta: " << beta << " gama: " << gama << endl;
  //  cout << endl << endl << endl;

}


int main () {

    MC13224V_ADXL345 * ac = new MC13224V_ADXL345;
    setup(ac);

    for (int var = 0; var < 500; ++var) {
        loop(ac);
    }
    Motors::shutDown();
    cout << " END" << endl;



}

