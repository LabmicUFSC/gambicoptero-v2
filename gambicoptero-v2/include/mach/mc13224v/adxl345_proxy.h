#ifndef __ADXL345proxy_h
#define __ADXL345proxy_h

//#include <system>///memory_map.h>
#include<system.h>
#include <utility/math.h>

__BEGIN_SYS

class ADXL345_Proxy
{
public:


    void measureAccel()
    {
    	short gyro;
    	Quadcopter_Network::get_data_coordinator(accelSample[0], accelSample[1], accelSample[2],
                                            gyro, gyro, gyro );
    }
    ADXL345_Proxy()
    {

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



private:
    short accelSample[3];
};


__END_SYS

#endif