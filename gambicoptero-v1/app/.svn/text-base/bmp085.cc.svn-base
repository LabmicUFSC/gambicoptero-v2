#include<mach/mc13224v/i2c.h>
#include<alarm.h>
#include<utility/ostream.h>

__USING_SYS

/*
    This code is based on the arduino code from sparkfun:
    http://www.sparkfun.com/tutorial/Barometric/BMP085_Example_Code.pde
    Its just an example. Please make sure you connected everything
    correctly before using.
*/

// Calibration values
// WARNING: arduino(ATMega) integer == ARM short!!!!!
short ac1;
short ac2; 
short ac3; 
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1; 
short b2;
short mb;
short mc;
short md;
int b5;

OStream cout;

int bmp085ReadInt(unsigned char address) {
  MC13224V_I2c::uint8_t buffAddress[1] = { address };
  MC13224V_I2c::i2c_sendData((MC13224V_I2c::uint8_t)0xEE,
                             &buffAddress[0],
                             1,
                             MC13224V_I2c::gI2cMstrReleaseBus_c
  );
  
  /* Wait a bit (REQUIRED!) */
  Alarm::delay(2000);
 
  MC13224V_I2c::uint8_t buffData[2] = { 0x00, 0x00 };
  MC13224V_I2c::i2c_receiveData((MC13224V_I2c::uint8_t)0xEF,
                                &buffData[0],
                                2,
                                MC13224V_I2c::gI2cMstrReleaseBus_c
  );
         
  return (int) (buffData[0]<<8 | buffData[1]);
}

void getCalibrationValues() {
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  cout << "ac5 = " << ac5 << endl;

  ac6 = bmp085ReadInt(0xB4);
  cout << "ac6 = " << ac6 << endl;
  
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  cout << "mc = " << mc << endl;
  md = bmp085ReadInt(0xBE);
  cout << "md = " << md << endl;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT() {
  MC13224V_I2c::uint8_t buffer[2] = { 0xF4, 0x2E };
  MC13224V_I2c::i2c_sendData((MC13224V_I2c::uint8_t)0xEE,
                             &buffer[0],
                             2,
                             MC13224V_I2c::gI2cMstrReleaseBus_c
  );
 
  // Wait at least 4.5ms
  Alarm::delay(5000);
  
  // Read two bytes from registers 0xF6 and 0xF7
  unsigned int ut = bmp085ReadInt(0xF6);
  return ut;
}

/* returns Temp (* 0.1 oC) */
short bmp085GetTemperature(unsigned int ut) {
  long long x1, x2;
  
  x1 = (((long long)ut - (long long)ac6)*(long long)ac5) >> 15;

    
  x2 = ((long long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);  
}

int main() {
    /* wait 5sec. (not required) */
    Alarm::delay(4000000);

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
    config.freqDivider = 0x01;
    config.i2cBroadcastEn = false;
    config.saplingRate = 0x10; /* default */
    config.slaveAddress = 0x00;
    MC13224V_I2c::i2c_setConfig(&config);

    /* Enable i2c module */
    MC13224V_I2c::i2c_enable();
 
    //CPU::out8(Machine::IO::I2C_FREQ_DIV, 0x20);
    getCalibrationValues();

    while(true){     
        /* Try to send some data */
       cout << (short)bmp085GetTemperature(bmp085ReadUT()) << "*0.1C" << endl;
        
        /* Just wait a bit (not required) */
        Alarm::delay(2000000);
    }
}
