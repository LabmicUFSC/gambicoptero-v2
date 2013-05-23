#ifndef ITG3200_H
#define ITG3200_H

#include <mach/mc13224v/i2c.h>


__BEGIN_SYS

class ITG3200{

public:
    void readAxis();
 //   BMA180(uint8 address = 0x00): ADDR(address){
    ITG3200(unsigned int unit = 0);
     ~ITG3200(){}
     int sample_x() {
         return 0;
     }
     int get_x();
     int get_y();
     int get_z();
     void calibrate();
     void measure();
     void measureGyroSum();
     void evaluateGyroRate();

     void measureSpecificGyroADC(int *gyroADC); //private ??
     void measureSpecificGyroSum();
     void evaluateSpecificGyroRate(int *gyroADC);


     virtual bool enable_x() {
         return 0;
     }

     virtual void disable_x() {

     }

     virtual bool data_ready_x() {
         return 0;
     }

     virtual int sample_y() {
         return 0;
     }


     virtual bool enable_y() {
        return 0;
     }

     virtual void disable_y() {

     }

     virtual bool data_ready_y() {
         return 0;
     }

     //set configurations, sample_rate ou pg24


     float gyroRate[3];
     int   gyroZero[3];
     long  gyroSample[3];
     float gyroSmoothFactor;
    // float gyroScaleFactor;
     float gyroHeading;
     unsigned long gyroLastMesuredTime;
     float gyroRateDegrees[3];

 private:

    typedef MC13224V_I2c I2c;
    typedef unsigned char uint8;

    static const uint8 MEMORY_ADDR = 0x1D;
    static const uint8 ADDR = 0xD2;
    static const uint8 ID_ADDR = 0x00;
    static const uint8 ID = 0x69;
    static const uint8 BUFFER_SIZE = 6;
    static const uint8 RESET_ADDR = 0x3E;
    static const uint8 RESET_VALUE =0x80;
    static const uint8 LOW_PASS_FILTER_ADDR = 0x16;
    static const uint8 LOW_PASS_FILTER_VALUE = 0x1D;
    static const uint8 OSCILLATOR_ADDR = 0x3E;
    static const uint8 OSCILLATOR_VALUE = 0x01;
    static const float SCALE_TO_RADIANS = 	823.626831; // 14.375 LSBs per °/sec, / Pi / 180
    static const uint8 SAMPLE_RATE_DIVIDER_ADDR = 0x15;
    static const uint8 DIVIDER = 0x00;
    static const float DEGREE_SENSITIVITY_LSB_PER_SECOND = 14.375;
    float _gyroScaleFactor;
    uint8 buf[BUFFER_SIZE]; //aux to get data from device with i2c

    static uint8 _acc_temp_msb[1];
    static uint8 _acc_temp_lsb[1];
    static uint8 _acc_x_lsb[1];
    static uint8 _acc_x_msb[1];
    static uint8 _acc_y_lsb[1];
    static uint8 _acc_y_msb[1];
    static uint8 _acc_z_lsb[1];
    static uint8 _acc_z_msb[1];
    static const short XAXIS = 0;
    static const short YAXIS = 1;
    static const short ZAXIS = 2;

    int get_axis(uint8 axis_lsb[]);
    bool verifyDevice();
    void reset();
    short gyroSampleCount;
    static const int FINDZERO = 49;
    MC13224V_I2c::I2cErr_t configure(const uint8 address, const uint8 data);

};

__END_SYS
#endif // ITG3200_H

