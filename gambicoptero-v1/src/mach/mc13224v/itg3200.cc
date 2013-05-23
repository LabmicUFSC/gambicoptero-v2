#include "mach/mc13224v/itg3200.h"
#include <alarm.h>
#include<tsc.h>
#include<utility/AQMath.h>
#include<utility/math.h>

__USING_SYS

ITG3200::uint8 ITG3200::_acc_temp_msb[1] = {0x1B};
ITG3200::uint8 ITG3200::_acc_temp_lsb[1] = { 0x1C};
ITG3200::uint8 ITG3200::_acc_x_msb[1] = {0x1D};
ITG3200::uint8 ITG3200::_acc_x_lsb[1] = { 0x1E };
ITG3200::uint8 ITG3200::_acc_y_msb[1] = { 0x1F };
ITG3200::uint8 ITG3200::_acc_y_lsb[1] = { 0x20 };
ITG3200::uint8 ITG3200::_acc_z_msb[1] = { 0x21 };
ITG3200::uint8 ITG3200::_acc_z_lsb[1] = { 0x22 };

ITG3200::ITG3200(unsigned int unit)
{

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

    gyroRate[0] = 0.0; gyroRate[1] = 0.0; gyroRate[2] = 0.0;
    gyroZero[0] = 0;     gyroZero[1] = 0;     gyroZero[2] = 0;
    gyroSample[0] =0; gyroSample[1] =0; gyroSample[0] =0;
    gyroSmoothFactor = 1.0;
    gyroHeading = 0.0;
    gyroLastMesuredTime = 0;

    _gyroScaleFactor = 1.0 / DEGREE_SENSITIVITY_LSB_PER_SECOND;//Math::radians(1.0 / DEGREE_SENSITIVITY_LSB_PER_SECOND);

    while (!verifyDevice());// while(1);

    Alarm::delay(100000);
    reset();
    Alarm::delay(100000);
    I2c::I2cErr_t e;

    Alarm::delay(100000);
    e = configure(LOW_PASS_FILTER_ADDR, LOW_PASS_FILTER_VALUE);
   if (e){
        db<ITG3200>(ERR) << "Could not set configurations. Error = " << e << endl;
        while(1);
    }
    Alarm::delay(100000);

   // e = configure(SAMPLE_RATE_DIVIDER_ADDR, DIVIDER); //DE onde ISSO?
    e = configure(OSCILLATOR_ADDR, OSCILLATOR_VALUE);//RANGE
    if (e) {
        db<ITG3200>(ERR) << "Could not set oscilator VALUE. Error = " << e << endl;
        while(1); /* halt */
    }

  //readConfiguration

}


ITG3200::I2c::I2cErr_t ITG3200::configure(const uint8 address, const uint8 data){
    unsigned char value[2] = {address,data};
    return I2c::i2cWrite(value, 2, ADDR, true);
}

void ITG3200::calibrate() {

    short findZeroX[FINDZERO];
    short findZeroY[FINDZERO];
    short findZeroZ[FINDZERO];
    short x;
    short y;
    short z;


    for (int i=0; i<FINDZERO; i++) {
        readAxis();
        x = (buf[0] << 8) | buf[1];
        y = (buf[2] << 8) | buf[3];
        z = (buf[4] << 8) | buf[5];
        findZeroX[i] = x;
        findZeroY[i] = y;
        findZeroZ[i] = z;
        Alarm::delay(100);

        }
    int tempX =  Math::findMedianInt(findZeroX, FINDZERO);
    int tempY =  Math::findMedianInt(findZeroY, FINDZERO);
    int tempZ = Math::findMedianInt(findZeroZ, FINDZERO);
    db<ITG3200> (TRC) << "--- Gyro Zero --- calibration" << endl;
    gyroZero[0] = tempX;
    gyroZero[1] = tempY;
    gyroZero[2] = tempZ;
    for (int i = 0; i < 3; ++i) {
        db<ITG3200>(TRC) << "gyroZero[" << i << "]: " << gyroZero[i] << endl;
    }

}

void ITG3200::measureSpecificGyroADC(int *gyroADC){
    readAxis();
    short x = (buf[0] << 8) | buf[1];
    short y = (buf[2] << 8) | buf[3];
    short z = (buf[4] << 8) | buf[5];

    gyroADC[XAXIS] = x - gyroZero[XAXIS];
    gyroADC[YAXIS] = gyroZero[YAXIS] - y;
    gyroADC[ZAXIS] = gyroZero[ZAXIS] - z;

}



void ITG3200::measureSpecificGyroSum(){
    readAxis();
    short x = (buf[0] << 8) | buf[1];
    short y = (buf[2] << 8) | buf[3];
    short z = (buf[4] << 8) | buf[5];

    gyroSample[XAXIS] += x;
    gyroSample[YAXIS] += y;
    gyroSample[ZAXIS] += z;
}

void ITG3200::evaluateSpecificGyroRate(int *gyroADC){
    gyroADC[XAXIS] = (gyroSample[XAXIS] / gyroSampleCount) - gyroZero[XAXIS];
    gyroADC[YAXIS] = gyroZero[YAXIS] - (gyroSample[YAXIS] / gyroSampleCount);
    gyroADC[ZAXIS] = gyroZero[ZAXIS] - (gyroSample[ZAXIS] / gyroSampleCount);
}


void ITG3200::readAxis(){
    MC13224V_I2c::I2cErr_t e;
    unsigned char regAddress[1] = {MEMORY_ADDR};//| 0x01};
    MC13224V_I2c::i2c_sendData(ADDR,
                                         regAddress,//&data[0],
                                         1,
                                          MC13224V_I2c::gI2cMstrReleaseBus_c);

    Alarm::delay(5);
   // uint8_t data[8] = {0x0, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    e = MC13224V_I2c::i2c_receiveData(ADDR,  buf, BUFFER_SIZE, MC13224V_I2c::gI2cMstrHoldBus_c);
    if (e) db<ITG3200> (ERR) << "Error read: " << e << endl;

}


//initialize == constructor
void ITG3200::measure(){

    //I2c::i2cWrite(ADDR, 1, MEMORY_ADDR, );
    int gyroADC[3];
    measureSpecificGyroADC(gyroADC);

    for (short axis = 0; axis <= ZAXIS; axis++){
       gyroRateDegrees[axis] = AQMath::filterSmooth(gyroADC[axis] * _gyroScaleFactor, gyroRateDegrees[axis]
                                        ,gyroSmoothFactor);
    }

    for (int i = XAXIS; i < ZAXIS; ++i) gyroRate[i] = Math::radians(gyroRateDegrees[i]);
    long int currentTime = ARM7_TSC::time_stamp();

    //aqui temos o time_stamp em microseconds
    if (gyroRate[ZAXIS] > (Math::radians(1.0)) || gyroRate[ZAXIS] < Math::radians(-1.0)) {
       gyroHeading += gyroRate[ZAXIS] * ((currentTime - gyroLastMesuredTime) / 1000000.0);
     }

    gyroLastMesuredTime = currentTime;

}

void ITG3200::measureGyroSum(){

    measureSpecificGyroSum();
    gyroSampleCount++;
}

void ITG3200::evaluateGyroRate(){

    int gyroADC[3];
    evaluateSpecificGyroRate(gyroADC);
    gyroSample[XAXIS] = 0;
    gyroSample[YAXIS] = 0;
    gyroSample[ZAXIS] = 0;
    gyroSampleCount = 0;
    for (short axis = 0; axis <= ZAXIS; axis++){
        gyroRateDegrees[axis] =  Math::filterSmooth((gyroADC[axis] * _gyroScaleFactor),
                                      gyroRateDegrees[axis], gyroSmoothFactor);
     }
     //Measure gyroheading

    for (int i = XAXIS; i < ZAXIS; ++i) gyroRate[i] = Math::radians(gyroRateDegrees[i]);
    long int currentTime = ARM7_TSC::time_stamp();
    if (gyroRate[ZAXIS] > Math::radians(1.0) || gyroRate[ZAXIS] < Math::radians(-1.0)) {
        gyroHeading += gyroRate[ZAXIS] * ((currentTime - gyroLastMesuredTime) / 1000000.0);
     }
      gyroLastMesuredTime = currentTime;

}

bool ITG3200::verifyDevice() {
    unsigned char data[1] = { ID_ADDR };
    I2c::i2cWrite(data, 1, ADDR, false);

    Alarm::delay(100000);
    unsigned char id = I2c::i2cReadByte(ADDR, true);
    if (id != ID){
        db<ITG3200> (ERR) <<"Error: Device not found." << endl;
        return false;
    }
    return true;
}

void ITG3200::reset(){
    configure(RESET_ADDR, RESET_VALUE);

}


 int ITG3200::get_axis(unsigned char * axis_msb){
    Alarm::delay(10);
     I2c::i2cWrite(axis_msb,1, ADDR, false);
    Alarm::delay(10);
    unsigned char msb = I2c::i2cReadByte(ADDR, false);
    Alarm::delay(10);
    unsigned char lsb = I2c::i2cReadByte(ADDR, false);
    int value = (int) ((msb <<8) | lsb);
    return value;
}

int ITG3200::get_x(){ return  get_axis(_acc_x_msb); }

int ITG3200::get_y(){ return get_axis(_acc_y_msb); }

int ITG3200::get_z(){ return get_axis(_acc_z_msb);}

