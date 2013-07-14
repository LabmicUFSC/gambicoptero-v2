__USING_SYS

#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <mach/mc13224v/adxl345_i2c.h>
#include <periodic_thread.h>
#include <utility/math.h>
#include <AeroQuad/AeroQuad/AeroQuad.h>
#include <AeroQuad/AeroQuad/FlightControlProcessor.h>
#include <AeroQuad/Libraries/AQ_Gyroscope/Gyroscope_ITG3200.h>
#include <AeroQuad/Libraries/AQ_Accelerometer/Accelerometer_ADXL345.h>

#include <AeroQuad/Libraries/AQ_Defines/GlobalDefined.h>
#include <AeroQuad/Libraries/AQ_Defines/SensorsStatus.h>
#include <AeroQuad/Libraries/AQ_Math/FourtOrderFilter.h>
#include <AeroQuad/Libraries/AQ_Kinematics/Kinematics_ARG.h>
#include <AeroQuad/Libraries/AQ_Receiver/Receiver_RemotePC.h>

// INCLUIR QUADCOPTER NETWORK

OStream cout;


int task_100hz() {

  for(;;) {
      evaluateGyroRate();
      evaluateMetersPerSec();

      // envia pela rede
      //set_data_coordinator( short accx, short accy, short accz, short gyrx, short gyry, short gyrz );
      // precisa de um conversor de precisão float -> short
      Quadcopter_Network::set_data_coordinator(meterPerSecSec[XAXIS], meterPerSecSec[YAXIS], meterPerSecSec[ZAXIS], gyroRate[XAXIS], gyroRate[YAXIS], gyroRate[ZAXIS]);

      Periodic_Thread::wait_next();
  }
}

int main() {

    Periodic_Thread thread(&task_100hz, 10000);
    thread.join();
    for(;;);

//    sensor(1);
}

