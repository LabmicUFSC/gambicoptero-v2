#ifndef ITG_3200_PROXY
#define ITG_3200_PROXY

#include <system.h>


#include <mach/mc13224v/quadcopter_network.h>

__BEGIN_SYS

class ITG3200_Proxy
{ 
	short gyro[3];
	private:
		unsigned int last_timestamp;
public:
	ITG3200_Proxy(){ last_timestamp = 0;};
	~ITG3200_Proxy(){};

	void measureGyro(){
		short accel;
		if(Quadcopter_Network::last_coordinator_timestamp() > last_timestamp)
		{
			last_timestamp = Quadcopter_Network::get_data_coordinator(accel,accel, accel, gyro[0], gyro[1], gyro[2] );
		}
		else
		{
			//Call predictor
		}
	}

	short sample_x(){ return gyro[0]; }
	short sample_y(){ return gyro[1]; }
	short sample_z(){ return gyro[2]; }

	/* data */
};

__END_SYS


#endif
