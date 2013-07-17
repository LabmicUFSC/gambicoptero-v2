#ifndef __ADXL345proxy_h
#define __ADXL345proxy_h

//#include <system>///memory_map.h>
#include <system.h>
#include <utility/math.h>

#include <mach/mc13224v/quadcopter_network.h>
#include <utility/predictor.h>
#include <AeroQuad/Libraries/AQ_Defines/GlobalDefined.h>

__BEGIN_SYS


class ADXL345_Proxy
{
	unsigned int last_timestamp;
    short accelSample[3];
    Predictor predictor[3];

public:


    void measureAccel()
    {
    	short gyro;
		if(Quadcopter_Network::last_coordinator_timestamp() > last_timestamp)
		{
			last_timestamp = Quadcopter_Network::get_data_coordinator(accelSample[0], accelSample[1], accelSample[2], gyro, gyro, gyro );
		    predictor[0].new_value(accelSample[0], micros());
            predictor[1].new_value(accelSample[1], micros());
            predictor[2].new_value(accelSample[2], micros());
        }
		else
		{
            accelSample[0] = predictor[0].get_prediction2(micros());
            accelSample[1] = predictor[1].get_prediction2(micros());
            accelSample[2] = predictor[2].get_prediction2(micros());
		}
    }
    ADXL345_Proxy()
    {
		last_timestamp = 0;
    }

    ~ADXL345_Proxy()
    {

    }

    short sample_x()
    {
    	return accelSample[0];
    }

    short sample_y()
    {
    	return accelSample[1];
    }

    short sample_z()
    {
    	return accelSample[2];
    }
};


__END_SYS

#endif
