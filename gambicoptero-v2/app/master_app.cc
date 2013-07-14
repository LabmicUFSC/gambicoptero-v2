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

OStream cout;

int task_100hz() {

  for(;;) {
      G_Dt = (currentTime - hundredHZpreviousTime) / 1000000.0;
      hundredHZpreviousTime = currentTime;
      
      evaluateGyroRate();
      evaluateMetersPerSec();

      for (int axis = XAXIS; axis <= ZAXIS; axis++) {
        filteredAccel[axis] = computeFourthOrder(meterPerSecSec[axis], &fourthOrder[axis]);
      }
        
      calculateKinematics(gyroRate[XAXIS], gyroRate[YAXIS], gyroRate[ZAXIS], filteredAccel[XAXIS], filteredAccel[YAXIS], filteredAccel[ZAXIS], G_Dt);
      
      processFlightControl();

      Periodic_Thread::wait_next();
  }
}

int main() {
    Periodic_Thread thread(&task_100hz, 10000);
    thread.join();
    for(;;);

//    sensor(1);
}

