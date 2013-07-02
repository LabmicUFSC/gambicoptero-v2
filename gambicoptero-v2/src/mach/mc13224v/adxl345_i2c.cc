#include "mach/mc13224v/adxl345_i2c.h"
#include <alarm.h>
#include <utility/malloc.h>

__BEGIN_SYS

ADXL345_I2C::ADXL345_I2C() {
    i2c.enable();
}
ADXL345_I2C::~ADXL345_I2C() {
    i2c.disable();
}

bool ADXL345_I2C::initAccel() {
    if(!i2c.write_register(DEVICE_ADDR, POWER_CTL, 1<<3))
        return false;
    if(!i2c.write_register(DEVICE_ADDR, DATA_FORMAT, 0x01))
        return false;
    if(!i2c.write_register(DEVICE_ADDR, BW_RATE, 8+2+1))
        return false;
    return true;
}

void ADXL345_I2C::measureAccel() {
    unsigned char values[6];
    if(!i2c.read_register(0x53, 0x32, 6, values))
        return;
    accelSample[X] = (values[1]<<8) + values[0];
    accelSample[Y] = (values[3]<<8) + values[2];
    accelSample[Z] = (values[5]<<8) + values[4];
}


__END_SYS
