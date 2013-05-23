
#include<mach/mc13224v/itg3200.h>
#include<alarm.h>
#include<utility/ostream.h>
#include<mach/mc13224v/motor.h>
#include<utility/AQMath.h>
//#include<mach/mc13224v/i2c.h>


__USING_SYS

OStream cout;



int main() {
   MC13224V_I2c::i2c_init();

  //  Alarm::delay(2000000);

    ITG3200 gyro; //Já inicializa
    gyro.calibrate();
    int timer = ARM7_TSC::time_stamp();
    cout << "ITG3200 initialization done successfully." << endl;
  //  Alarm::delay(2000000);

    float factor = 14.375;
    while(1) {
      //  if ( (ARM7_TSC::time_stamp()) - timer > 1000) {

         Alarm::delay(1000000);
        gyro.measureGyroSum();
        gyro.evaluateGyroRate();

       /* cout << "x: " << (gyro.get_x()) / factor << endl;
        cout << "y: " << ( gyro.get_y() / factor)<< endl;
        cout << "z: " <<  (gyro.get_z()) / factor<< endl;

        cout << "x: " << degrees(gyro.gyroRate[0]) << endl;
        cout << "y: " <<  degrees(gyro.gyroRate[1])<< endl;
        cout << "z: " <<  degrees(gyro.gyroRate[2])<< endl;
*/
        cout << "x: " << (gyro.gyroRate[0]) << endl;
        cout << "y: " << (gyro.gyroRate[1])<< endl;
        cout << "z: " << (gyro.gyroRate[2])<< endl;

      //Alarm::delay(500000);
       // }
    }
}
