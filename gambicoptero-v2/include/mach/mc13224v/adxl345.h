// EPOS  MC13224V_ADXL345 Accelerometer Mediator Declarations MC13244

#ifndef __ADXL345_h
#define __ADXL345_h

//#include <system>///memory_map.h>
#include<system.h>
#include <mach/mc13224v/spi.h>
#include <utility/math.h>

__BEGIN_SYS

class MC13224V_ADXL345
{
private:

    typedef MC13224V_SPI SPI;
    enum {
        POWER_CTL	= 0x2D,
        DATA_FORMAT	= 0x31,
        DATAX0		= 0x32,
        DATAX1		= 0x33,
        DATAY0		= 0x34,
        DATAY1		= 0x35,
        DATAZ0		= 0x36,
        DATAZ1		= 0x37,
        FIFO_CTL	= 0x38
    };

public:
static const int FULLRANGE = 0x09; //FULLRANGE -/+4g
static const int SAMPLER_RATE_200HZ = 8+2+1;  //200hz sampling

float filteredAccel[3];
float accelScaleFactor[3];
float runTimeAccelBias[3];

float meterPerSecSec[3];
long accelSample[3];
float accelOneG;
char accelSampleCount;
const int SAMPLECOUNT;

    //esses valores sao todos globais no original
    //que beleza hein?
    //parece que o EEPROM 'e usado para agilizar a calibragem


    void initializeAccel(int range, int sample_rate);

    void computeAccelBias();
    void evaluateMetersPerSec();
    void measureAccel();

    void measureAccelSum();

    MC13224V_ADXL345();
    MC13224V_ADXL345(int);

    ~ MC13224V_ADXL345() {
        disable_sensor();
        delete (_spi);
    }


    int getId() {

        unsigned int address = 0x80 << (32 - 8);
        _spi->writeRegisterSync(address);
        unsigned int stream  = _spi->readRegisterSync();
        char id = stream;

        return id;
    }

    int getPowerControl() {
        int val = read(POWER_CTL);
        char power = val;
        return power;
    }



    int sample_x() {
        _sample_x = format(DATAX0, DATAX1);
        return _sample_x;
    }

    int get_x() { return 0; }
    bool enable_x() { return true; }
    void disable_x() {}
    bool data_ready_x() { return true; }

    int sample_y() {
        _sample_y = format(DATAY0, DATAY1);
        return _sample_y;
    }

    int get_y() { return 0; }
    bool enable_y() { return true; }
    void disable_y() {}
    bool data_ready_y() { return true; }

    int get_z() { return 0; }
    bool enable_z() { return true; }
    void disable_z() {}
    bool data_ready_z() { return true; }
    
    int sample_z() {
        _sample_z = format(DATAZ0, DATAZ1);
        return _sample_z;
    }


private:
     //What is Reg8? changed to char
    void write(unsigned int address, unsigned int value);
    int read(char registerAddress);
    int format(char add0, char add1);

    void enable_sensor() {
        write(POWER_CTL, 0x08);
    }

    void disable_sensor() {
        write(POWER_CTL, 0x00);
    }

    SPI * _spi;


    char _write_mask;
    char _read_mask;
    short _sample_x;
    short _sample_y;
    short _sample_z;

};


__END_SYS

#endif
