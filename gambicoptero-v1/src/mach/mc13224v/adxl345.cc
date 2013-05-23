#include "mach/mc13224v/adxl345.h"
#include <alarm.h>
#include <utility/malloc.h>

__BEGIN_SYS


void MC13224V_ADXL345::evaluateMetersPerSec() {

  for (short axis = 0; axis <= 2; axis++) {
    meterPerSecSec[axis] = (accelSample[axis] / accelSampleCount) * accelScaleFactor[axis] + runTimeAccelBias[axis];
        accelSample[axis] = 0;
        db<MC13224V_ADXL345>(TRC) << "meterPerSecSec[" << axis << "] = " << meterPerSecSec[axis] << endl;
  }
  accelSampleCount = 0;
}


void MC13224V_ADXL345::measureAccel() {

   meterPerSecSec[0] = sample_x() * accelScaleFactor[0] + runTimeAccelBias[0];
   meterPerSecSec[1] = sample_y() * accelScaleFactor[1] + runTimeAccelBias[1];
   meterPerSecSec[2] = sample_z() * accelScaleFactor[2] + runTimeAccelBias[2];

}

void MC13224V_ADXL345::measureAccelSum() {

    accelSample[0] += sample_x();
    accelSample[1] += sample_y();
    accelSample[2] += sample_z();

    accelSampleCount++;
}

void MC13224V_ADXL345::computeAccelBias(){
    db<MC13224V_ADXL345>(TRC) << "--- computeAccelBias ---" << endl;


    for (int samples = 0; samples < SAMPLECOUNT; samples++) {
       measureAccelSum();
       Alarm::delay(2500); //é em microseconds ?
     }

     for (int axis = 0; axis < 3; axis++) {
       meterPerSecSec[axis] = (float(accelSample[axis])/SAMPLECOUNT) * accelScaleFactor[axis];
       accelSample[axis] = 0;
       db<MC13224V_ADXL345>(TRC) << "accelSample/SAMPLECOUNT["<<axis<<"] = " << accelSample[axis]/SAMPLECOUNT << endl;
       db<MC13224V_ADXL345>(TRC) << "accelScaleFactor["<<axis<<"] = " << accelScaleFactor[axis] << endl;
       db<MC13224V_ADXL345>(TRC) << "meterPerSecSec["<<axis<<"] = " << meterPerSecSec[axis] << endl;
       db<MC13224V_ADXL345>(TRC) << "runtTimeAccelBias["<<axis<<"] = " << runTimeAccelBias[axis] << endl;
     }


     accelSampleCount = 0;
     runTimeAccelBias[0] = -meterPerSecSec[0];
     runTimeAccelBias[1] = -meterPerSecSec[1];
     runTimeAccelBias[2] = -9.8065 - meterPerSecSec[2];
     db<MC13224V_ADXL345> (TRC) << " --- runTimeAccelBias after calibration --- " << endl;
     for (char axis = 0; axis < 3; axis++)
         db<MC13224V_ADXL345>(TRC) << "runtTimeAccelBias["<<axis<<"] = " << runTimeAccelBias[axis] << endl;

     accelOneG = Math::abs(meterPerSecSec[2] + runTimeAccelBias[2]);
     db<MC13224V_ADXL345>(TRC) << "accelOneG = " << accelOneG << endl;

}



MC13224V_ADXL345::MC13224V_ADXL345() : accelOneG(-9.80665),
    accelSampleCount(0), SAMPLECOUNT(400),
    _write_mask(0x00), _read_mask(0x80),
    _sample_x(0),
    _sample_y(0), _sample_z(0){

    //no codigo original esta no DataStorage a definicao
    //dos valores, porque ele guarda e le da eeprom
    filteredAccel[0] = 1.0;
    filteredAccel[1] = 1.0;
    filteredAccel[2] = 1.0;

    //NO DATA STORAGE  MUDA
    accelScaleFactor[0] = 1;//0.0035; //1
    accelScaleFactor[1] = 1;//0.0035;
    accelScaleFactor[2] = 1;//0.0035;

    runTimeAccelBias[0] = 0;
    runTimeAccelBias[1] = 0;
    runTimeAccelBias[2] = 0;

    meterPerSecSec[0] = 0;
    meterPerSecSec[1] = 0;
    meterPerSecSec[2] = 0;

    //#include <system/memory_map.h>
    accelSample[0] = 0;
    accelSample[1] = 0;
    accelSample[2] = 0;


    SPI::spiConfig_t config;
    SPI::spiDelay delay = SPI::delayOne;
    SPI::spiSdoInactive doInactive = SPI::Mosi_out_11;
    SPI::spiClockPol clkPol = SPI::polOne;
    SPI::spiClockPhase clkPha = SPI::phaOne;
    SPI::spiMisoPhase misoPhase = SPI::Same;
    SPI::spiClockFreq clkFreq = SPI::_1_5MHz;
    SPI::spi3Wire wire = SPI::Wire4;
    SPI::spiMode mode = SPI::Master;
    SPI::spiSlaveSelectSetup ssSetup = SPI::Auto_Active_Low;

    config.Setup.Bits.SsSetup = ssSetup; //table 15-10 automatic, low
    config.Setup.Bits.SsDelay = delay;
    config.Setup.Bits.SdoInactive = doInactive;
    config.Setup.Bits.ClockPol = clkPol; //ok CPOL
    config.Setup.Bits.ClockPhase = clkPha; //ok CPHA
    config.Setup.Bits.MisoPhase = misoPhase; //UPDATE ON RISE???
    config.Setup.Bits.ClockFreq = clkFreq; //24/n
    config.Setup.Bits.Mode = mode;  //low = masterMode
    config.Setup.Bits.S3Wire = wire; //4Wire
    config.ClkCtrl.Bits.DataCount =  16;//0b011000;//32-8;
    config.ClkCtrl.Bits.ClockCount = 16;//0b0011000;

    _spi = new SPI(config);
    enable_sensor();
    //range should be passed on constructor
    write(DATA_FORMAT, 0x01); //???

}

void MC13224V_ADXL345::write(unsigned int address, unsigned int value){

    address = address << (32 - 8);
    value = value << (32 - 16);
    unsigned int data = address | value;
    _spi->writeRegisterSync(data);//SPI::SPI_WriteSync(data);
}

int MC13224V_ADXL345::read(char registerAddress){
     SPI::spiErr_t e;
    unsigned int data[] = {0};
    unsigned int address = (0x80 | registerAddress) << (32 -8);
    e = SPI::SPI_WriteSync(address);
    e = SPI::SPI_ReadSync(data);
    return data[0];
}

int MC13224V_ADXL345::format(char add0, char add1){
    int value;
    value =   0x000000FF & ( read(add0) );//DATAX0);
    value |= (0xFFFFFFFF & ( read(add1) ) ) << 8; //0x00000003
    return value;
}

void MC13224V_ADXL345::initializeAccel(int range, int sample_rate){
        write(DATA_FORMAT, range); //FULL RANGE +/-4G
        write(0x2C, sample_rate);  //200hz sampling
    }


__END_SYS
