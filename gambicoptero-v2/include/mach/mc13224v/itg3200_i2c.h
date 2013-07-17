#ifndef __ITG3200I2C_h
#define __ITG3200I2C_h

//#include <system>///memory_map.h>
#include<system.h>
#include <mach/mc13224v/i2c.h>
#include <utility/math.h>

__BEGIN_SYS

class ITG3200_I2C
{
private:

    typedef MC13224V_I2C I2C;
    enum {
        IDENTITY               = 0x68,
        IDENTITY_MASK          = 0x7E,
        MEMORY_ADDRESS         = 0x1D,
        RESET_ADDRESS          = 0x3E,
        RESET_VALUE            = 0x80,
        LOW_PASS_FILTER_ADDR   = 0x16,
        LOW_PASS_FILTER_VALUE  = 0x1D,    // 10Hz low pass filter
        OSCILLATOR_ADDR        = 0x3E,
        OSCILLATOR_VALUE       = 0x01,    // use X gyro oscillator
        TEMPERATURE_ADDRESS    = 0x1B, 
        DEVICE_ADDR            = 0x68
    };
    enum {
        X = 0,
        Y = 1,
        Z = 2
    };

    static const float SCALE_TO_RADIANS; // 14.375 LSBs per °/sec, / Pi / 180

public:


    bool measureGyro();
    ITG3200_I2C();

    ~ITG3200_I2C();

    bool initGyro();

    short sample_x() {
        return gyroSample[X];
    }

    short sample_y() {
        return gyroSample[Y];
    }

    short sample_z() {
        return gyroSample[Z];
    }



private:
    
    I2C i2c;
    short gyroSample[3];


    float gyroRate[3];
    int   gyroZero[3];
    float gyroScaleFactor;
    float gyroHeading;
    unsigned long gyroLastMesuredTime;
    unsigned char gyroSampleCount;
};


__END_SYS

#endif
