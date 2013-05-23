#include "mach/mc13224v/bma180.h"
#include <mach/mc13224v/i2c.h>
#include<alarm.h>
__USING_SYS


BMA180::uint8 BMA180::_acc_x_lsb[1] = {0x02};
BMA180::uint8 BMA180::_acc_x_msb[1] = { 0x03 };
BMA180::uint8 BMA180::_acc_y_lsb[1] = { 0x04 };
BMA180::uint8 BMA180::_acc_y_msb[1] = { 0x05 };
BMA180::uint8 BMA180::_acc_z_lsb[1] = { 0x06 };
BMA180::uint8 BMA180::_acc_z_msb[1] = { 0x07 };

void BMA180::calibrate() {}


BMA180::BMA180(unsigned int unit) {

 MC13224V_I2c::I2cErr_t e;
    if(!verifyDevice()) {
        db<BMA180> (ERR) << "Device not foundASDASS" << endl;
        while(1); /* halt */
}
    // Connect to the ctrl_reg1 register and set the ee_w bit to enable writing.
    Alarm::delay(100000);
    config(RESET_REGISTER, TRIGER_RESET_VALUE);
    Alarm::delay(1000000);

    db<BMA180> (TRC) << "Connect to the ctrl_reg1 register and set the ee_w bit" <<
            "to enable writing." << endl;

    e = config(ENABLE_WRITE_CONTROL_REGIST, CONTROL_REGISTER);
    if(e) {
         db<BMA180>(ERR) << "Error: Could not set ee_w bit." << endl;
         db<BMA180> (ERR) << "Error code: " << e << endl;
        while(1); /* halt */
    }
//    Alarm::delay(100000);

    uint8  bw_address[1] = {BW_TCS};
    I2c::i2cWrite(bw_address, 1, ADDR, true);
    unsigned char data = I2c::i2cReadByte(ADDR, true);
    db<BMA180> (TRC) << "VAlor dos dados no BW_TCS: " << hex << data << hex << endl;
    // set low pass filter to 1.2kHz (value = 0000xxxx)
    //MUITO CUIDADO AQUI, COMO SETAR A FREQUENCIA???

    config(LOW_PASS_FILTER_REGISTER, data & _1200HZ_LOW_PASS_FILTER_VALUE);
      // From page 27 of BMA180 Datasheet
      //  1.0g = 0.13 mg/LSB
      //  1.5g = 0.19 mg/LSB
      //  2.0g = 0.25 mg/LSB
      //  3.0g = 0.38 mg/LSB
      //  4.0g = 0.50 mg/LSB
      //  8.0g = 0.99 mg/LSB
      // 16.0g = 1.98 mg/LSB

    uint8 off_set_reg_addr[1] = {OFFSET_REGISTER};
    I2c::i2cWrite(off_set_reg_addr, 1, ADDR, true);
    data = I2c::i2cReadByte(ADDR,true);

    data &= 0xF1;
  //data |= 0x04; // Set range select bits for +/-2g
    data |= 0x08; // set range select bits for +/-4g

    e = config(OFFSET_REGISTER, data);
// Connect to the bw_tcs register and set the filtering level to 10hz.
//    MC13224V_I2c::uint8_t data_bw_tcs[2] = { 0x20, 0x08 };
 //   e = MC13224V_I2c::i2cWrite(data_bw_tcs, 2, ADDR, true);

    if(e) {
        db<BMA180> (ERR) << "Error: Could not set filtering level." << endl;
        db<BMA180> (ERR) << "Error code: " << e << endl;
        while(1); /* halt */
    }

      Alarm::delay(100000);
    // Connect to the offset_lsb1 register and set the range to +- 2.
    // This is 0.25mg/LSB
/*     unsigned char data_offset_lsb1[2] = { 0x35, 0x04 };
    e = MC13224V_I2c::i2cWrite(data_offset_lsb1, 2, ADDR, true);
    if(e) {
        db<BMA180> (ERR) << "Error: Could not set range." << endl;
        db<BMA180> (ERR) << "Error code: " << e << endl;
        while(1); /* halt */
   // }
    Alarm::delay(100000);
}

MC13224V_I2c::I2cErr_t BMA180::config(unsigned char address,
                                      unsigned char data) {
 unsigned char value[2] = {address, data};
 return I2c::i2cWrite(value, 2, ADDR, true);

}

bool BMA180::verifyDevice() {
    MC13224V_I2c::I2cErr_t e;
    db<BMA180> (TRC) << "Inicializando o BMA180" << endl;
    //assert(i2c is init)
    MC13224V_I2c::uint8_t data[1] = {ID_ADDR };
    e = MC13224V_I2c::i2cWrite(data, 1, ADDR, false);
    Alarm::delay(100000);
    db<BMA180> (TRC) << "Escreveu o endereco, retornou: " << e << endl;
    MC13224V_I2c::uint8_t id = MC13224V_I2c::i2cReadByte(ADDR, true);
    db<BMA180> (TRC) << "Leu o endereço" << endl;
    return id == ID;
}

//pagina 52-53,
//aparentemente nao precisa esperar nada, dependendo do tempo de espera
//para chamar a função

int BMA180::get_x(){

    return get_axis(_acc_x_lsb, _acc_x_msb);
}

int BMA180::get_y(){
    return get_axis(_acc_y_lsb, _acc_y_msb);
}

int BMA180::get_z() {
    return get_axis(_acc_z_lsb, _acc_z_msb);
}

int BMA180::get_axis(BMA180::uint8 axis_lsb[], BMA180::uint8 axis_msb[]){
    int lsb;
    int msb;
    typedef MC13224V_I2c I2c;
    I2c::I2cErr_t e;
    bool new_data = false;
    db<BMA180> (TRC) << "Reading accel: " << endl;


    e = I2c::i2cWrite(axis_lsb, 1, ADDR, false);
    db<BMA180> (ERR) << "I2C error[1]: " << e << endl;

    //Nao precisa esperar o dado ficar pronto, ele está pronto,
    lsb = I2c::i2cReadByte(ADDR, true);
    I2c::i2cWrite(axis_msb, 1, ADDR, false);
    db<BMA180> (ERR) << "I2C error[2]: " << e << endl;
    msb = I2c::i2cReadByte(ADDR, true);
    return msb;

}

