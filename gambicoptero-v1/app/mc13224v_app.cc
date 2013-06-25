#include <machine.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <alarm.h>
#include <sensor.h>
#include <mach/mc13224v/i2c.h>

__USING_SYS

const unsigned char SINK_ID  = 0x01;
const unsigned int DATA_SIZE = 32;

char msg[DATA_SIZE];

NIC * nic;
OStream cout;

void i2c_init() {
    /* Initialize i2c module */
    MC13224V_I2c::i2c_init();

    CPU::out32(
        Machine::IO::GPIO_FUNC_SEL0,
        CPU::in32(Machine::IO::GPIO_FUNC_SEL0)
        |  (1 << 24) /* set GPIO_12 to I2C_SCL */
        |  (1 << 26) /* set GPIO_13 to I2C_SDA */
    );

    /* Configure i2c module */
    MC13224V_I2c::I2Config_t config;
    config.i2cInteruptEn = false;
    config.freqDivider = 0x20;
    config.i2cBroadcastEn = false;
    config.saplingRate = 0x10; /* default */
    config.slaveAddress = 0x00;
    config.I2cCallbackFunction_func = 0;
    MC13224V_I2c::i2c_setConfig(&config);

    /* Enable i2c module */
    MC13224V_I2c::i2c_enable(); 
}

MC13224V_I2c::I2cErr_t i2cWrite(uint8_t data[], int data_size, uint8_t addr, bool releaseBus) {
    return MC13224V_I2c::i2c_sendData(addr,
                                      &data[0],
                                      data_size,
                                      (releaseBus) ?
                                        MC13224V_I2c::gI2cMstrReleaseBus_c :
                                        MC13224V_I2c::gI2cMstrHoldBus_c
    ); 
}

uint8_t i2cReadByte(uint8_t addr, bool releaseBus) {
    uint8_t data[1] = { 0x00 };
    MC13224V_I2c::i2c_receiveData(addr | 0x01,
                                  &data[0],
                                  1,
                                  (releaseBus) ?
                                   MC13224V_I2c::gI2cMstrReleaseBus_c :
                                   MC13224V_I2c::gI2cMstrHoldBus_c
    ); 

    return data[0];
}

int main() {
	MC13224V_I2c::i2c_init();
	

}

