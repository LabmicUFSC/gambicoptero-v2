#include <machine.h>
#include <mach/mc13224v/i2c.h>

__BEGIN_SYS

MC13224V_I2c::I2c_Init_func MC13224V_I2c::i2c_fun_init =
        reinterpret_cast<MC13224V_I2c::I2c_Init_func>(
            reinterpret_cast<void *>(MC13224V_I2c::I2C_INIT_ADDRESS));


void MC13224V_I2c::i2c_init(){

    i2c_fun_init();
    CPU::out32(
        Machine::IO::GPIO_FUNC_SEL0,
        CPU::in32(Machine::IO::GPIO_FUNC_SEL0)
        |  (1 << 24) /* set GPIO_12 to I2C_SCL */
        |  (1 << 26) /* set GPIO_13 to I2C_SDA */
    );




}

MC13224V_I2c::I2cErr_t MC13224V_I2c::i2cWrite(MC13224V_I2c::uint8_t data[], int data_size,
                                MC13224V_I2c::uint8_t addr,
                                bool releaseBus) {
    return MC13224V_I2c::i2c_sendData(addr,
                                      data,//&data[0],
                                      data_size,
                                      (releaseBus) ?
                                        MC13224V_I2c::gI2cMstrReleaseBus_c :
                                        MC13224V_I2c::gI2cMstrHoldBus_c
    );
}



MC13224V_I2c::uint8_t MC13224V_I2c::i2cReadByte(MC13224V_I2c::uint8_t addr, bool releaseBus) {
    MC13224V_I2c::uint8_t data[1] = { 0x0 };
    MC13224V_I2c::i2c_receiveData(addr ,//| 0x01,
                                  &data[0],
                                  1,
                                  (releaseBus) ?
                                   MC13224V_I2c::gI2cMstrReleaseBus_c :
                                   MC13224V_I2c::gI2cMstrHoldBus_c
    );

    return data[0];
}

short MC13224V_I2c::readShort(uint8_t addr, bool releaseBus){
    unsigned char data[2] = {0x00, 0x00};//nao precisa
    return 0;


}



MC13224V_I2c::I2c_Enable_func MC13224V_I2c::i2c_enable =
        reinterpret_cast<MC13224V_I2c::I2c_Enable_func>(
            reinterpret_cast<void *>(MC13224V_I2c::I2C_ENABLE_ADDRESS));

MC13224V_I2c::I2c_Disable_func MC13224V_I2c::i2c_disable =
        reinterpret_cast<MC13224V_I2c::I2c_Disable_func>(
            reinterpret_cast<void *>(MC13224V_I2c::I2C_DISABLE_ADDRESS));


MC13224V_I2c::I2c_SetConfig_func MC13224V_I2c::i2c_setConfig =
        reinterpret_cast<MC13224V_I2c::I2c_SetConfig_func>(
             reinterpret_cast<void *>(MC13224V_I2c::I2C_SETCONFIG_ADDRESS));

MC13224V_I2c::I2c_RecoverBus_func MC13224V_I2c::i2c_recoverBus =
        reinterpret_cast<MC13224V_I2c::I2c_RecoverBus_func>(
            reinterpret_cast<void *>(MC13224V_I2c::I2C_RECOVERBUS_ADDRESS));

MC13224V_I2c::I2c_SetCallbackFunction_func MC13224V_I2c::i2c_setCallbackFunction=
        reinterpret_cast<MC13224V_I2c::I2c_SetCallbackFunction_func>(
            reinterpret_cast<void *>(MC13224V_I2c::I2C_SETCALLBACKFUNCTION_ADDRESS));


MC13224V_I2c::I2c_SendData_func MC13224V_I2c::i2c_sendData=
        reinterpret_cast<MC13224V_I2c::I2c_SendData_func>(
            reinterpret_cast<void *>(MC13224V_I2c::I2C_SENDDATA_ADDRESS));

MC13224V_I2c::I2c_ReceiveData_func MC13224V_I2c::i2c_receiveData=
        reinterpret_cast<MC13224V_I2c::I2c_ReceiveData_func>(
            reinterpret_cast<void *>(MC13224V_I2c::I2C_RECEIVEDATA_ADDRESS));

MC13224V_I2c::I2c_GetStatus_func MC13224V_I2c::i2c_getStatus=
        reinterpret_cast<MC13224V_I2c::I2c_GetStatus_func>(
            reinterpret_cast<void *>(MC13224V_I2c::I2C_GETSTATUS_ADDRESS));

MC13224V_I2c::I2c_CancelTransfer_func MC13224V_I2c::i2c_cancelTransfer=
        reinterpret_cast<MC13224V_I2c::I2c_CancelTransfer_func>(
            reinterpret_cast<void *>(MC13224V_I2c::I2C_CANCEL_TRANSFER_ADDRESS));

MC13224V_I2c::I2c_Isr_func MC13224V_I2c::i2c_isr=
        reinterpret_cast<MC13224V_I2c::I2c_Isr_func>(
            reinterpret_cast<void *>(MC13224V_I2c::I2C_ISR_ADDRESS));


__END_SYS
