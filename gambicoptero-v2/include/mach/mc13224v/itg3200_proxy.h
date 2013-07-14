#ifndef ITG_3200_PROXY
#define ITG_3200_PROXY

#include <system.h>

__BEGIN_SYS

class ITG3200_Proxy
{ 
	short gyro[3];
public:
	ITG3200_Proxy(){};
	~ITG3200_Proxy(){};

	void measureGyro(){
		short accel;
    	Quadcopter_Network::get_data_coordinator(accel,accel, accel,
                                            gyro[0], gyro[1], gyro[2] );
	}

	short sample_x(){ return gyro[0]; }
	short sample_y(){ return gyro[1]; }
	short sample_z(){ return gyro[2]; }

	/* data */
};

__END_SYS


#endif