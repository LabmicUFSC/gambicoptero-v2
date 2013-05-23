#include<mach/mc13224v/i2c.h>
#include<alarm.h>
#include<utility/ostream.h>

__USING_SYS

typedef unsigned char uint8_t;
typedef char int8_t; 
typedef short int16_t;

const uint8_t ITG3200_ADDR = 0xD2;
const uint8_t ID = 0x69;

enum Axis {
    X = 0,
    Y,
    Z
};

OStream cout;

void initI2c(){
    MC13224V_I2c::I2Config_t config;
    config.i2cInteruptEn = false;
    config.freqDivider = 0x10;
    config.i2cBroadcastEn = false;
    config.saplingRate = 0x10; /* default */
    config.slaveAddress = 0x00;
    config.I2cCallbackFunction_func = 0;

    //  MC13224V_I2c::i2c_setConfig(&config);
    /* Enable i2c module */
    MC13224V_I2c::i2c_enable();


}

/* ITG3200 functions ------------------------------------------------------- */
void initITG3200() {
    uint8_t data[1] = { 0x00 };
    MC13224V_I2c::i2cWrite(data, 1, ITG3200_ADDR, false);
    Alarm::delay(10000);
    uint8_t id =    MC13224V_I2c::i2cReadByte(ITG3200_ADDR, true);
    if(id != ID) {
       cout << "Error: Device not found. ->" << id << endl;
        while(1); /* halt */
    }
    Alarm::delay(10000);

    // Set Sample Rate Divider to 0 (see manual pg23)
    MC13224V_I2c::I2cErr_t e;
    uint8_t data_sample_rate[2] = { 0x15, 0x00 };
    e =    MC13224V_I2c::i2cWrite(data_sample_rate, 2, ITG3200_ADDR, true);
    if(e) {
        cout << "Could not set sample rate divider. Error = " << e << endl;
        while(1); /* halt */
    }
    Alarm::delay(10000);

    // Set some configurations (see manual pg24)
    uint8_t data_conf[2] = { 0x16 , 0b00011101 };
    e =    MC13224V_I2c::i2cWrite(data_conf, 2, ITG3200_ADDR, true);
    if(e) {
        cout << "Could not set configurations[1]. Error = " << e << endl;
        while(1); /* halt */
    }
    Alarm::delay(10000);

    uint8_t data2_conf[2] = { 0x3E , 0b00000001 };
    e =    MC13224V_I2c::i2cWrite(data2_conf, 2, ITG3200_ADDR, true);
    if(e) {
        cout << "Could not set configurations[1]. Error = " << e << endl;
        while(1); /* halt */
    }
    Alarm::delay(10000);
} 

char whoAmI(){
    MC13224V_I2c::I2cErr_t e;
    uint8_t data[1] = {0x00};
    e = MC13224V_I2c::i2cWrite(data, 1, ITG3200_ADDR, false);
    if (e) cout << "Error readDLPF: " << e << endl;
    Alarm::delay(100000);
    uint8_t lsb = MC13224V_I2c::i2cReadByte(ITG3200_ADDR, false);
    return lsb;
}

char readDLPF(){
    MC13224V_I2c::I2cErr_t e;
    uint8_t data[1] = {0x16};
    e = MC13224V_I2c::i2cWrite(data, 1, ITG3200_ADDR, false);
    if (e) cout << "Error readDLPF: " << e << endl;
    Alarm::delay(100000);
    uint8_t lsb = MC13224V_I2c::i2cReadByte(ITG3200_ADDR, false);
    return lsb;
}

/*short readRegister(char address){
    MC13224V_I2c::I2cErr_t e;
    uint8_t data[1] = {address};
    e = MC13224V_I2c::i2cWrite(data, 1, ITG3200_ADDR, false);
    if (e) cout << "Error read: " << e << endl;
    //Alarm::delay(100000);
    uint8_t lsb = MC13224V_I2c::i2cReadByte(ITG3200_ADDR, false);
    return lsb;
}*/

short readRegister(){
    MC13224V_I2c::I2cErr_t e;
    uint8_t regAddress[1] = {0x1B };//| 0x01};
    MC13224V_I2c::i2c_sendData(ITG3200_ADDR,
                                         regAddress,//&data[0],
                                         1,
                                          MC13224V_I2c::gI2cMstrReleaseBus_c);

    Alarm::delay(5);
    uint8_t data[8] = {0x0, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    e = MC13224V_I2c::i2c_receiveData(ITG3200_ADDR,    data, 8, MC13224V_I2c::gI2cMstrHoldBus_c);
    if (e){

        cout << "Error read: " << e << endl;

        Alarm::delay(5000000);}
    for (int i = 0; i < 4; ++i){
        short dataN = (data[i*2] << 8 ) | data[i*2+1];
        cout << hex << "data["<<i <<"] = " << dataN << " -> " << dec << (float)(dataN / 14.375) << endl;
    }
     //uint8_t lsb = MC13224V_I2c::i2cReadByte(ITG3200_ADDR, false);
     //cout << "lsb: " << hex << (int)lsb << endl;
    return 0;
}

int16_t measureITG3200(Axis axis) {
    uint8_t data[1] = { 0x1D + (axis << 1) };
       MC13224V_I2c::i2cWrite(data, 1, ITG3200_ADDR, false);
    uint8_t msb =    MC13224V_I2c::i2cReadByte(ITG3200_ADDR, false);
    uint8_t lsb =    MC13224V_I2c::i2cReadByte(ITG3200_ADDR, false);

    return (msb << 8) | lsb;
}
/* ITG32000 functions ------------------------------------------------------ */

 
/* Main program ------------------------------------------------------------ */
int main() {
    /* wait 2sec. (not required) */
  //  OStream coutizinho;
    Alarm::delay(2000000);
    MC13224V_I2c::i2c_init();
    initI2c();
    MC13224V_I2c::i2c_enable;
    initITG3200();
    cout << "ITG3200 initialization done successfully." << endl;
    Alarm::delay(2000000);

    while(1) {
        cout << "---  READ ---" << endl;
        readRegister();
        cout << endl << endl << endl << endl << endl;
        Alarm::delay(500000);
       /* coutizinho << "Reading: 0x0 " << bin << readRegister(0x0) << endl;
        Alarm::delay(1000000);
        coutizinho << "Reading: 0x15 " << bin << readRegister(0x15) << endl;
        Alarm::delay(1000000);
        coutizinho << "Reading: 0x16 " <<bin << readRegister(0x16) << endl;
        Alarm::delay(1000000);
        coutizinho << "Reading: 0x17 " << bin << readRegister(0x17) << endl;
        Alarm::delay(1000000);
        coutizinho << "Reading: 0x1A " << bin << readRegister(0x1A) << endl;
        Alarm::delay(1000000);
        coutizinho << "Reading: 0x3E " << bin << readRegister(0x3E) << endl;
      */ // Alarm::delay(1000000);

       // for (char i = 27; i <=  34; i++)
           // coutizinho << "Reading: " << hex << (short) i << " -> "  << hex << readRegister(i |0x01) << endl;

      /*  coutizinho << hex << measureITG3200(Y) << hex << endl;
        Alarm::delay(1000000);
        coutizinho << hex << measureITG3200(Z) << endl;
        Alarm::delay(1000000);
        coutizinho << "ReadDLPF: " << hex << (short) readDLPF() << hex << endl;
        Alarm::delay(500000);
        coutizinho << "ReadDLPF: " << hex << (short) whoAmI() << hex << endl;
        Alarm::delay(500000);
    */}
}
/* Main program ------------------------------------------------------------ */
