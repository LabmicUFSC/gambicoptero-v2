#ifndef BMA180_H
#define BMA180_H
#include <system.h>
#include <mach/mc13224v/i2c.h>
__BEGIN_SYS

class BMA180
{

public:
 //   BMA180(uint8 address = 0x00): ADDR(address){
    BMA180(unsigned int unit = 0);
     ~BMA180(){}
     int sample_x() {
         return 0;
     }

     int get_x();
     int get_y();
     int get_z();
     void calibrate();

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

     MC13224V_I2c::I2cErr_t config(unsigned char address,
                                   unsigned char data);

private:
    typedef unsigned char uint8;
    static const uint8 ADDR = 0x80;
    static const uint8 ID_ADDR = 0x00;
    static const uint8 ID = 0x03;
    static const uint8 RESET_REGISTER = 0x10;
    static const uint8 TRIGER_RESET_VALUE = 0x10;
    static const uint8 ENABLE_WRITE_CONTROL_REGIST = 0x0D;
    static const uint8 CONTROL_REGISTER = 0x10;
    static const uint8 BW_TCS =  0x20;
    static const uint8 LOW_PASS_FILTER_REGISTER =  0x20;
    static const uint8 _10HZ_LOW_PASS_FILTER_VALUE =  0x0F;
    static const uint8 _1200HZ_LOW_PASS_FILTER_VALUE = 0X7F;
    static const uint8 OFFSET_REGISTER = 0x35;
    static const uint8 READ_ROLL_ADDRESS = 0x02;
    static const uint8 READ_PITCH_ADDRESS = 0x04;
    static const uint8 READ_YAW_ADDRESS = 0x06;
    static const uint8 BUFFER_SIZE =  6;

    static uint8 _acc_x_lsb[1];
    static uint8 _acc_x_msb[1];
    static uint8 _acc_y_lsb[1];
    static uint8 _acc_y_msb[1];
    static uint8 _acc_z_lsb[1];
    static uint8 _acc_z_msb[1];
    int get_axis(uint8 axis_lsb[], uint8 axis_msb[]);
    bool verifyDevice();
    typedef MC13224V_I2c I2c;

};

//BMA180::uint8 BMA180::_acc_x_lsb[0] = {0x02};

#endif // BMA180_H
__END_SYS
