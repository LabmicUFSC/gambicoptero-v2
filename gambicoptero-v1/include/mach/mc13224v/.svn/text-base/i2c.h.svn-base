#ifndef _mc13224v_i2c_H
#define _mc13224v_i2c_H

#include<machine.h>
__BEGIN_SYS

class MC13224V_I2c {
private:
    static const unsigned int I2C_CANCEL_TRANSFER_ADDRESS = 0x00004b5d;
    static const unsigned int I2C_DISABLE_ADDRESS =         0x000049f9;
    static const unsigned int I2C_ENABLE_ADDRESS =          0x000049c5;
    static const unsigned int I2C_INIT_ADDRESS =            0x0000497d;
    static const unsigned int I2C_GETSTATUS_ADDRESS =       0x00004b45;
    static const unsigned int I2C_ISR_ADDRESS =            0x00004e29;
    static const unsigned int I2C_RECEIVEDATA_ADDRESS =     0x00004ced;
    static const unsigned int I2C_RECOVERBUS_ADDRESS =      0x00004a41;
    static const unsigned int I2C_SENDDATA_ADDRESS =        0x00004bdd;
    static const unsigned int I2C_SETCALLBACKFUNCTION_ADDRESS= 0x00004aa9;
    static const unsigned int I2C_SETCONFIG_ADDRESS =       0x00004ac1;

    MC13224V_I2c() {
    }
    ~MC13224V_I2c() {
    }

public:
    enum I2cErr_t {
        gI2cErrNoError_c = 0, //0
        gI2cErrModuleIsEn_c,//1
        gI2cErrModuleIsDis_c,//2
        gI2cErrClkIsEn_c,//3
        gI2cErrClkIsDis_c,//4
        gI2cErrNullPointer_c,//5
        gI2cErrNoCallback_c,//6
        gI2cErrInvalidOp_c,//7
        gI2cErrBusBBusy_c,//8
        gI2cErrNoDevResp_c,//9
        gI2cErrNoAckResp_c,//10
        gI2cErrTransferInProgress_c,//11
        gI2cErrModuleBusy_c,//12
        gI2cErrBusError_c//13
    };

    enum I2cMode_t {
        gI2cSlaveMode_c = 0,
        gI2cMasterMode_c,s
    };

    enum I2cTransferMode_t {
        gI2cSlvTransfer_c = 0,
        gI2cMstrReleaseBus_c,
        gI2cMstrHoldBus_c,
        gI2cTransferTypeMax_c
    };

    enum I2cResponse_t {
        gI2cAckResponse_c = 0,
        gI2cNoAckResponse_c
    };

    enum I2cOperation_t {
        gI2cWrite_c = 0,
        gI2cRead_c
    };

    enum I2cBusStatus_t {
        gI2cBusIdle_c = 0,
        gI2cBusBusy_c
    };

    enum I2cTransferType_t {
        gI2cTransmitData_c = 0,
        gI2cReceiveData_c
    };

    struct I2Config_t {
        void (*I2cCallbackFunction_func)(unsigned char dataLossLength, unsigned short int status);
        bool i2cInteruptEn;
        bool i2cBroadcastEn;
        unsigned char slaveAddress;
        unsigned char freqDivider;
        unsigned char saplingRate;

    } ;

    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef void (*I2cCallbackFunction_t)(unsigned char dataLossLength,
                                          unsigned short int status);
    //static I2cCallbackFunction_func callbackFunc;

    typedef void (*I2c_Init_func)();
private:
    static I2c_Init_func i2c_fun_init;
public:

    static void i2c_init();

    static I2cErr_t i2cWrite(uint8_t data[], int data_size,
                                    uint8_t addr,
                                    bool releaseBus);

    static uint8_t i2cReadByte(MC13224V_I2c::uint8_t addr,
                               bool releaseBus);

    static short readShort(uint8_t addr, bool releaseBus);

    typedef I2cErr_t (*I2c_Enable_func)();
    static I2c_Enable_func i2c_enable;

    typedef I2cErr_t (*I2c_Disable_func)();
    static I2c_Disable_func i2c_disable;

    typedef I2cErr_t (*I2c_SetConfig_func)(I2Config_t *pI2Config);
    static I2c_SetConfig_func i2c_setConfig;

    typedef I2cErr_t (*I2c_RecoverBus_func)();
    static I2c_RecoverBus_func i2c_recoverBus;

    typedef I2cErr_t  (*I2c_SetCallbackFunction_func)(I2cCallbackFunction_t
                                                   pI2cCallback);
    static I2c_SetCallbackFunction_func i2c_setCallbackFunction;

    typedef I2cErr_t (*I2c_SendData_func)(uint8_t slaveDestAddr,
                                          uint8_t *i2cBuffData,
                                          uint16_t dataLength,
                                          I2cTransferMode_t transferMode);
    static I2c_SendData_func i2c_sendData;

    typedef I2cErr_t (*I2c_ReceiveData_func)(uint8_t slaveDestAddr,
                                             uint8_t *i2cBuffData,
                                             uint16_t dataLength,
                                             I2cTransferMode_t transferMode);
    static I2c_ReceiveData_func i2c_receiveData;

    typedef I2cErr_t (*I2c_GetStatus_func)(uint8_t *status);
    static I2c_GetStatus_func i2c_getStatus;

    typedef I2cErr_t (*I2c_CancelTransfer_func)(I2cTransferType_t transferType);
    static I2c_CancelTransfer_func i2c_cancelTransfer;

    typedef void (*I2c_Isr_func)();
    static I2c_Isr_func i2c_isr;

};
__END_SYS

#endif // I2C_H

