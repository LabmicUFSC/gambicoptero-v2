#include<mach/mc13224v/i2c.h>
#include<alarm.h>
#include<utility/ostream.h>
#include<mach/mc13224v/motor.h>

__USING_SYS

typedef unsigned char uint8_t;
typedef char int8_t; 
typedef short int16_t;

const uint8_t ITG3200_ADDR = 0xD2;
     
enum Axis {
    X = 0,
    Y,
    Z
};

OStream cout;

/* I2C functions ----------------------------------------------------------- */
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
/* I2C functions ----------------------------------------------------------- */


/* ITG3200 functions ------------------------------------------------------- */
void initITG3200() {
    uint8_t data[1] = { 0x00 };
    i2cWrite(data, 1, ITG3200_ADDR, false);
    Alarm::delay(10000);
    uint8_t id = i2cReadByte(ITG3200_ADDR, true);
    if(id != 0x69) {
        cout << "Error: Device not found." << endl;
        while(1); /* halt */
    }
    Alarm::delay(10000);

    // Set Sample Rate Divider to 0 (see manual pg23)
    MC13224V_I2c::I2cErr_t e;
    uint8_t data_sample_rate[2] = { 0x15, 0x00 };
    e = i2cWrite(data_sample_rate, 2, ITG3200_ADDR, true);
    if(e) {
        cout << "Could not set sample rate divider. Error = " << e << endl;
        while(1); /* halt */
    }
    Alarm::delay(10000);

    // Set some configurations (see manual pg24)
    uint8_t data_conf[2] = { 0x16, 0b00011100 };
    e = i2cWrite(data_conf, 2, ITG3200_ADDR, true);
    if(e) {
        cout << "Could not set configurations[1]. Error = " << e << endl;
        while(1); /* halt */
    }
    Alarm::delay(10000);
} 

int16_t measureITG3200(Axis axis) {
    uint8_t data[1] = { 0x1D + (axis << 1) };
    i2cWrite(data, 1, ITG3200_ADDR, false);
    uint8_t msb = i2cReadByte(ITG3200_ADDR, false);
    uint8_t lsb = i2cReadByte(ITG3200_ADDR, true);

    return (msb << 8) | lsb;
}
/* ITG32000 functions ------------------------------------------------------ */

 
/* Main program ------------------------------------------------------------ */
int main() {
    /* wait 2sec. (not required) */
    Alarm::delay(2000000);

    i2c_init();
    initITG3200();
    cout << "ITG3200 initialization done successfully." << endl;
    Alarm::delay(2000000);

    while(1) {
        cout << measureITG3200(X) << endl;
        cout << measureITG3200(Y) << endl;
        cout << measureITG3200(Z) << endl;
        Alarm::delay(500000);
    }
}
/* Main program ------------------------------------------------------------ */
