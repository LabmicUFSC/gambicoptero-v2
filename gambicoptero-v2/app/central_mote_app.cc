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
#include <mach/mc13224v/quadcopter_network.h>

OStream cout;
ITG3200_I2C gyroso;

volatile bool network_ok;
void maca_handler(MACA_Transceiver::Event event)
{
    if(event == MACA_Transceiver::LOST_COMMUNICATION)
	{
        cout<<"Lost communication! Abort!\n";
		network_ok = false;
	}
    else if(event == MACA_Transceiver::BEACON_READY)
		;
        //cout<<"Network received data from coordinator!\n";
    else if(event == MACA_Transceiver::DATA_READY)
		;
        //cout<<"Network received feedback data!\n";
}

int task_100hz() {
	int i = 0;
  

  while(network_ok) {
      measureAccel();
      //measureGyro();
      //if(!gyroso.measureGyro())
      //  for(;;)cout<<"TRETA\n";
      gyroRate[XAXIS] = gyroso.sample_x();
      gyroRate[YAXIS] = gyroso.sample_y();
      gyroRate[ZAXIS] = gyroso.sample_z();
      //evaluateMetersPerSec();
      // envia pela rede
      //set_data_coordinator( short accx, short accy, short accz, short gyrx, short gyry, short gyrz );
      // precisa de um conversor de precisão float -> short
      
      Quadcopter_Network::set_data_coordinator(accelSample[XAXIS], accelSample[YAXIS], accelSample[ZAXIS], gyroRate[XAXIS], gyroRate[YAXIS], gyroRate[ZAXIS]);

	  i=(i+1)%100;
	  if(!i)
	  {
		  cout << "accelerometer: (" << accelSample[XAXIS] << "     " << accelSample[YAXIS] << "     " << accelSample[ZAXIS] << ")    ~~    ";
		  cout << "gyroscope: ("<< gyroRate[XAXIS] << ",     " << gyroRate[YAXIS] << ",     " << gyroRate[ZAXIS] << ")\n";
	  }

      Periodic_Thread::wait_next();
  }
  cout << "Lost the network! Land blindingly!\n";
}

int main() {
	network_ok = true;;
      Quadcopter_Network::init(&maca_handler);

    initializeAccel();
    if(!gyroso.initGyro())
     for(;;) cout << "TRETA2\n";
    //if(!initializeGyro()) {
    // for(;;)
    //  cout << "CAGOU\n";
    //}
    //computeAccelBias();

	Quadcopter_Network::run();
    Periodic_Thread thread(&task_100hz, 10000);
    thread.join();
    for(;;);

//    sensor(1);
}

