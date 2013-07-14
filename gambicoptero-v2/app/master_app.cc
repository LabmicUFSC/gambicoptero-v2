__USING_SYS

#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <mach/mc13224v/adxl345_proxy.h>
#include <mach/mc13224v/itg3200_proxy.h>
#include <periodic_thread.h>
#include <utility/math.h>
#include <AeroQuad/AeroQuad/AeroQuad.h>
#include <AeroQuad/AeroQuad/FlightControlProcessor.h>
//#include <AeroQuad/Libraries/AQ_Gyroscope/Gyroscope_ITG3200.h>
//#include <AeroQuad/Libraries/AQ_Accelerometer/Accelerometer_ADXL345.h>

#include <AeroQuad/Libraries/AQ_Defines/GlobalDefined.h>
#include <AeroQuad/Libraries/AQ_Defines/SensorsStatus.h>
#include <AeroQuad/Libraries/AQ_Math/FourtOrderFilter.h>
#include <AeroQuad/Libraries/AQ_Kinematics/Kinematics_ARG.h>
#include <AeroQuad/Libraries/AQ_Receiver/Receiver_RemotePC.h>

OStream cout;
ADXL345_Proxy accel;
ITG3200_Proxy gyro;


void maca_handler(MACA_Transceiver::Event event)
{
    if(event == MACA_Transceiver::LOST_COMMUNICATION)
        cout<<"Lost communication! Abort!\n";
    else if(event == MACA_Transceiver::COORDINATOR_DATA_READY)
        cout<<"Network received data from coordinator!\n";
    else if(event == MACA_Transceiver::FEEDBACK_DATA_READY)
        cout<<"Network received feedback data!\n";
}

void _evaluateGyroRate()
{
  gyro.measureAccel();
  gyroRate[XAXIS] = gyro.sample_x();
  gyroRate[YAXIS] = gyro.sample_y();
  gyroRate[ZAXIS] = gyro.sample_z();

}

void _evaluateMetersPerSec()
{
  accel.measureAccel();
  meterPerSecSec[XAXIS] = accel.sample_x();
  meterPerSecSec[YAXIS] = accel.sample_y();
  meterPerSecSec[ZAXIS] = accel.sample_z();
}

int task_100hz() {

  init(maca_handler);
  for(;;) {
      G_Dt = (currentTime - hundredHZpreviousTime) / 1000000.0;
      hundredHZpreviousTime = currentTime;
      
      _evaluateGyroRate();
      _evaluateMetersPerSec();

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

