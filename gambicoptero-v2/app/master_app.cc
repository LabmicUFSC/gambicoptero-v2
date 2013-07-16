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
#include <AeroQuad/Libraries/AQ_Gyroscope/Gyroscope_ITG3200.h>
#include <AeroQuad/Libraries/AQ_Accelerometer/Accelerometer_ADXL345.h>

#include <AeroQuad/Libraries/AQ_Defines/GlobalDefined.h>
#include <AeroQuad/Libraries/AQ_Defines/SensorsStatus.h>
#include <AeroQuad/Libraries/AQ_Math/FourtOrderFilter.h>
#include <AeroQuad/Libraries/AQ_Kinematics/Kinematics_ARG.h>
#include <AeroQuad/Libraries/AQ_Receiver/Receiver_RemotePC.h>

#include <mach/mc13224v/quadcopter_network.h>

OStream cout;
ADXL345_Proxy acceleroso;
ITG3200_Proxy gyroso;


void maca_handler(MACA_Transceiver::Event event)
{
    if(event == MACA_Transceiver::LOST_COMMUNICATION)
        cout<<"Lost communication! Abort!\n";
    else if(event == MACA_Transceiver::BEACON_READY)
		;
//        cout<<"Network received data from coordinator!\n";
    else if(event == MACA_Transceiver::DATA_READY)
		;
  //      cout<<"Network received feedback data!\n";
}

void _evaluateGyroRate()
{
  gyroso.measureGyro();
  gyroRate[XAXIS] = gyroso.sample_x();
  gyroRate[YAXIS] = gyroso.sample_y();
  gyroRate[ZAXIS] = gyroso.sample_z();
}

void _evaluateMetersPerSec()
{
  acceleroso.measureAccel();
  accelSample[XAXIS] = acceleroso.sample_x();
  accelSample[YAXIS] = acceleroso.sample_y();
  accelSample[ZAXIS] = acceleroso.sample_z();
}

int task_100hz() {

	int i = 0;
  for(;;) {
      G_Dt = (currentTime - hundredHZpreviousTime) / 1000000.0;
      hundredHZpreviousTime = currentTime;
      
      _evaluateGyroRate();
      _evaluateMetersPerSec();

	  i=(i+1)%100;
	  if(!i)
	      cout << "X: " << accelSample[0] << "    Y: " << accelSample[1] << "    Z: " << accelSample[2] << "\n";

      //for (int axis = XAXIS; axis <= ZAXIS; axis++) {
      //  filteredAccel[axis] = computeFourthOrder(accelSample[axis], &fourthOrder[axis]);
      //}
        
      //calculateKinematics(gyroRate[XAXIS], gyroRate[YAXIS], gyroRate[ZAXIS], filteredAccel[XAXIS], filteredAccel[YAXIS], filteredAccel[ZAXIS], G_Dt);
      
      //processFlightControl();

      Periodic_Thread::wait_next();
  }
}

int main() {
    Quadcopter_Network::init(&maca_handler);
	Quadcopter_Network::run();
    Periodic_Thread thread(&task_100hz, 10000);
    thread.join();
    for(;;);

//    sensor(1);
}

