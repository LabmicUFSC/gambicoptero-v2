#include<mach/mc13224v/i2c.h>
#include<alarm.h>
#include<utility/ostream.h>
#include<machine.h>
#include<sensor.h>

__USING_SYS

typedef unsigned char uint8_t;
typedef char int8_t;
OStream cout;

/* I2C functions ----------------------------------------------------------- */
void i2cInit() {
    MC13224V_I2c::i2c_init();
    /* Initialize i2c module */
  /*  MC13224V_I2c::i2c_init();
    CPU::out32(
        Machine::IO::GPIO_FUNC_SEL0,
        CPU::in32(Machine::IO::GPIO_FUNC_SEL0)
        |  (1 << 24) /* set GPIO_12 to I2C_SCL */
//        |  (1 << 26) /* set GPIO_13 to I2C_SDA */
 //   );

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
   MC13224V_I2c::I2cErr_t e;
   e= MC13224V_I2c::i2c_enable();
    cout << "Erro: " << e << endl;

}
/*
MC13224V_I2c::I2cErr_t i2cWrite(uint8_t data[], int data_size, uint8_t addr, bool releaseBus) {
    return MC13224V_I2c::i2c_sendData(addr,
                                      &data[0],
                                      data_size,
                                      (releaseBus) ?
                                        MC13224V_I2c::gI2cMstrReleaseBus_c :
                                        MC13224V_I2c::gI2cMstrHoldBus_c
    ); 
}
*/
/*uint8_t i2cReadByte(uint8_t addr, bool releaseBus) {
    uint8_t data[1] = { 0x00 };
    MC13224V_I2c::i2c_receiveData(addr | 0x01,
                                  &data[0],
                                  1,
                                  (releaseBus) ?
                                   MC13224V_I2c::gI2cMstrReleaseBus_c :
                                   MC13224V_I2c::gI2cMstrHoldBus_c
    ); 

    return data[0];
} */
/* I2C functions ----------------------------------------------------------- */


/* BMA180 functions -------------------------------------------------------- */
/*void initBMA180() {
    uint8_t data[1] = { 0x00 };
    i2cWrite(data, 1, BMA180_ADDR, false);
    Alarm::delay(10000);
    uint8_t id = i2cReadByte(BMA180_ADDR, true);
    if(id != 0x03) {
        cout << "Error: Device not found." << endl;
        while(1); /* halt */
   // } */
   // Alarm::delay(10000);




    // Connect to the ctrl_reg1 register and set the ee_w bit to enable writing.
/*    MC13224V_I2c::I2cErr_t e;
    uint8_t data_ee_w[2] = { 0x0D, 0x10 };
    e = i2cWrite(data_ee_w, 2, BMA180_ADDR, true);
    if(e) {
        cout << "Error: Could not set ee_w bit." << endl;
        cout << "Error code: " << e << endl;
        while(1); /* halt */
  //  }
   // Alarm::delay(10000);

    // Connect to the bw_tcs register and set the filtering level to 10hz.
   /* uint8_t data_bw_tcs[2] = { 0x20, 0x08 };
    e = i2cWrite(data_bw_tcs, 2, BMA180_ADDR, true);
    if(e) {
        cout << "Error: Could not set filtering level." << endl;
        cout << "Error code: " << e << endl;
        while(1); /* halt */
   // }
   // Alarm::delay(10000);

    // Connect to the offset_lsb1 register and set the range to +- 2.
    // This is 0.25mg/LSB
   /* uint8_t data_offset_lsb1[2] = { 0x35, 0x04 };
    e = i2cWrite(data_offset_lsb1, 2, BMA180_ADDR, true);
    if(e) {
        cout << "Error: Could not set range." << endl;
        cout << "Error code: " << e << endl;
        while(1); /* halt */
   // }
   // Alarm::delay(10000);
//}

/*
int8_t readAccel(Axis axis) {
    MC13224V_I2c::I2cErr_t e;

    /* Wait for new data */
  /*  bool new_data = false;
    uint8_t lsb;
    while(!new_data) {
        uint8_t data[1] = { 0x02 + (axis << 1) };
        e = i2cWrite(data, 1, BMA180_ADDR, false);
        if(e) {
            cout << "I2C error[1]: " << e << endl;
        }
        Alarm::delay(10000);
        lsb = i2cReadByte(BMA180_ADDR, true);
        new_data = lsb & 0x01;
        Alarm::delay(10000);
    }

    uint8_t data2[1] = { 0x03 + (axis << 1) };
    e = i2cWrite(data2, 1, BMA180_ADDR, false);
    if(e) {
        cout << "I2C error[2]: " << e << endl;
    }
    uint8_t msb = i2cReadByte(BMA180_ADDR, true);    

    return msb; 

    //float result = (msb << 8 | lsb) * 0.25;
    //return result;
}*/
/* BMA180 functions -------------------------------------------------------- */

 
/* Main program ------------------------------------------------------------ */
int main() {
    i2cInit();
    const int one_second = 1000000;
    OStream cout;
    Alarm::delay(1000000);
    cout << "Inicializando" << endl;
  //  Accelerometer * accelerometer = new Accelerometer;
    Accelerometer * giroscopio = new Accelerometer(1);

    Alarm::delay(1000000);
    while(true){
        cout << "LOOP!!" << endl;
        Alarm::delay(one_second);
 /*       cout <<  "Accel: " << endl << " X: " <<  accelerometer->get_x() << endl;
     cout <<   "Y: " << accelerometer->get_y() <<endl;
     cout <<   "Z: " << accelerometer->get_z() <<endl;;
   */  cout << "Giroscopio: " << endl << "X: " << giroscopio->get_x()<<endl;;
     cout << "Y: " <<   giroscopio->get_y()<<endl;;
     cout <<  "Z: " <<   giroscopio->get_z()<<endl;;
    }
    /* wait 2sec. (not required) */
  /*  Alarm::delay(2000000);

    i2cInit();
    initBMA180();
    cout << "BMA180 initialization done successfully." << endl;
    Alarm::delay(2000000);

    while(1) {
        /* Read and convert to int, because we cant cout float values */
     /*   short x = (short)readAccel(X);
        short y = (short)readAccel(Y);
        short z = (short)readAccel(Z);

        cout << "X = " << x;
        cout << ", Y = " << y;
        cout << ", Z = " << z << endl;
        Alarm::delay(500000); */
    //}
}
/* Main program ------------------------------------------------------------ */
