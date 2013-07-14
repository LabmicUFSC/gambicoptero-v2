#include "mach/mc13224v/itg3200_i2c.h"
#include <alarm.h>
#include <utility/malloc.h>

__BEGIN_SYS

const float ITG3200_I2C::SCALE_TO_RADIANS = 823.626831;

ITG3200_I2C::ITG3200_I2C() {
    i2c.enable();
}
ITG3200_I2C::~ITG3200_I2C() {
    i2c.disable();
}

bool ITG3200_I2C::initGyro() {

    if(!i2c.write_register(DEVICE_ADDR, RESET_ADDRESS, RESET_VALUE))// send a reset to the device
        return false;
    if(!i2c.write_register(DEVICE_ADDR, LOW_PASS_FILTER_ADDR, LOW_PASS_FILTER_VALUE))// 10Hz low pass filter
        return false;
    if(!i2c.write_register(DEVICE_ADDR, RESET_ADDRESS, OSCILLATOR_VALUE))// use internal oscillator
        return false;
    
    gyroScaleFactor = Math::radians(1.0 / 14.375);  //  ITG3200 14.375 LSBs per °/sec
    return true;
}

void ITG3200_I2C::measureGyro() {
    unsigned char values[6] = {0,0,0,0,0,0};
    i2c.set_timeout(40000);
    if(!i2c.read_register(DEVICE_ADDR, MEMORY_ADDRESS, 6, values))
        return;
    gyroSample[X] = (values[0]<<8) + values[1];
    gyroSample[Y] = (values[2]<<8) + values[3];
    gyroSample[Z] = (values[4]<<8) + values[5];
}


__END_SYS
