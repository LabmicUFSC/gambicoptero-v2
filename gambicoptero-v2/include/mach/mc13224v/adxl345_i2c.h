#ifndef __ADXL345I2C_h
#define __ADXL345I2C_h

//#include <system>///memory_map.h>
#include<system.h>
#include <mach/mc13224v/i2c.h>
#include <utility/math.h>

__BEGIN_SYS

class ADXL345_I2C
{
private:

    typedef MC13224V_I2C I2C;
    enum {
        BW_RATE     = 0x2C,
        POWER_CTL	= 0x2D,
        DATA_FORMAT	= 0x31,
        DATAX0		= 0x32,
        DATAX1		= 0x33,
        DATAY0		= 0x34,
        DATAY1		= 0x35,
        DATAZ0		= 0x36,
        DATAZ1		= 0x37,
        FIFO_CTL	= 0x38,
        DEVICE_ADDR = 0x53
    };
    enum {
        X = 0,
        Y = 1,
        Z = 2
    };

public:


    void measureAccel();
    ADXL345_I2C();

    ~ADXL345_I2C();

    bool initAccel();

    short sample_x() {
        return accelSample[X];
    }

    short sample_y() {
        return accelSample[Y];
    }

    short sample_z() {
        return accelSample[Z];
    }



private:
    
    I2C i2c;
    short accelSample[3];
};


__END_SYS

#endif
