#ifndef ITG_3200_PROXY
#define ITG_3200_PROXY

#include <system.h>


#include <mach/mc13224v/quadcopter_network.h>
#include <utility/predictor.h>
#include <AeroQuad/Libraries/AQ_Defines/GlobalDefined.h>


__BEGIN_SYS


class ITG3200_Proxy
{ 
	short gyro[3];
	unsigned int last_timestamp;
	Predictor predictor[3];

public:
	ITG3200_Proxy(){ last_timestamp = 0;};
	~ITG3200_Proxy(){};

	void measureGyro(){
		short accel;
		if(Quadcopter_Network::last_coordinator_timestamp() > last_timestamp)
		{
			last_timestamp = Quadcopter_Network::get_data_coordinator(accel,accel, accel, gyro[0], gyro[1], gyro[2] );
			predictor[0].new_value(gyro[0], micros());
            predictor[1].new_value(gyro[1], micros());
            predictor[2].new_value(gyro[2], micros());
		}
		else
		{
            gyro[0] = predictor[0].get_prediction2(micros());
            gyro[1] = predictor[1].get_prediction2(micros());
            gyro[2] = predictor[2].get_prediction2(micros());
		}
	}

	short sample_x(){ return gyro[0]; }
	short sample_y(){ return gyro[1]; }
	short sample_z(){ return gyro[2]; }

	/* data */
};

__END_SYS


#endif
