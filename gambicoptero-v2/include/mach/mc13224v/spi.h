#ifndef _mc13224v_spi_H
#define _mc13224v_spi_H

__BEGIN_SYS

class MC13224V_SPI {
private:
    static const unsigned int SPI_Abort_ADDR               = 0x0000cd61;
    static const unsigned int SPI_Close_ADDR               = 0x0000ca7d;
    static const unsigned int SPI_GetConfig_ADDR           = 0x0000cb29;
    static const unsigned int SPI_GetRxAsync_ADDR          = 0x0000ccfd;
    static const unsigned int SPI_GetStatus_ADDR           = 0x0000cdc9;
    static const unsigned int SPI_ISR_ADDR                 = 0x0000cdd5;
    static const unsigned int SPI_Open_ADDR                = 0x0000ca49;
    static const unsigned int SPI_ReadSync_ADDR            = 0x0000cc09;
    static const unsigned int SPI_SetCallback_ADDR         = 0x0000cda5;
    static const unsigned int SPI_SetConfig_ADDR           = 0x0000caa1;
    static const unsigned int SPI_SetTxAsync_ADDR          = 0x0000cd35;
    static const unsigned int SPI_StartAsync_ADDR          = 0x0000cca1;
    static const unsigned int SPI_WriteSync_ADDR           = 0x0000cb81;


public:

        typedef struct {
          union {
            unsigned int  Word;
            struct {
              unsigned int  DataCount:7;   // max value is 32
              unsigned int  reserved0:1;   // should be zero
              unsigned int  ClockCount:8;  // Master Mode only, max value is 32
              unsigned int  reserved1:16;  // should be zero
            } Bits;
          } ClkCtrl;

          union {
            unsigned int  Word;
            struct {
              unsigned int  SsSetup:2;	//WRONG	/* Slave Select Setup*/
              unsigned int  SsDelay:2;     // Master Mode only
              unsigned int  SdoInactive:2;
              unsigned int  reserved0:2;   // should be zero
              unsigned int  ClockPol:1;
              unsigned int  ClockPhase:1;
              unsigned int  MisoPhase:1;   // Master Mode only
              unsigned int  reserved1:1;   // should be zero
              unsigned int  ClockFreq:3;   // Master Mode only
              unsigned int  reserved2:1;   // should be zero
              unsigned int  Mode:1;
              unsigned int  S3Wire:1;
              unsigned int  reserved3:14;  // should be zero
            } Bits;
          } Setup;
        } spiConfig_t;

private:
    spiConfig_t _config;


public:

    enum spiErr_t {
        gSpiErrNoError_c = 0,			//0
        gSpiErrAlreadyOpen_c,			//1
        gSpiErrPortClosed_c,			//2
        gSpiErrAbortError_c,			//3
        gSpiErrAsyncOperationPending_c,	//4
        gSpiErrWrongConfig_c,			//5
        gSpiErrWrongCallbackFunc_c,		//6
        gSpiErrNullPointer_c,			//7
        gSpiErrMax_c					//8
    };

    enum spiStatus_t {
        gSpiStatusClosed_c = 0,
        gSpiStatusIdle_c,
        gSpiStatusSyncOperationPending_c,
        gSpiStatusAsyncOperationPending_c,
        gSpiStatusError_c,
        gSpiStatusMax_c
    };



    enum spiDelay {
        delayZero = 0,
        delayOne,
        delayTwo,
        Three
    };

    enum spiSdoInactive {
        Mosi_out_00 = 0,
        Mosi_out_10,
        Mosi_out_01,
        Mosi_out_11
    };

    enum spiClockPol {
        polZero = 0,
        polOne,
    };

    enum spiClockPhase {
        phaZero = 0,
        phaOne
    };

    enum spiMisoPhase {
        Oposite = 0,
        Same
    };

    enum spiClockFreq {
        _12MHz = 0,
        _6MHz,
        _3MHz,
        _1_5MHz,
        _750KHz,
        _375KHz,
        _185_5KHz,
        _93_75KHz
    };

    enum spi3Wire {
        Wire4 = 0,
        Wire3
    };

    enum spiMode {
        Master = 0,
        Slave
    };

    enum spiSlaveSelectSetup{
        Auto_Active_High = 0,
        Auto_Active_Low,
        Low,
        High
    };

    MC13224V_SPI(spiConfig_t config); //qspiConfig_t config);
    ~MC13224V_SPI() {
        SPI_Close();
    }

    typedef void (*spiCallback_t)(unsigned int Data);

    typedef spiErr_t (*SPI_Open_func)();
    static SPI_Open_func SPI_Open;

    typedef spiErr_t (*SPI_Close_func)();
    static SPI_Close_func SPI_Close;

    typedef spiErr_t (*SPI_SetConfig_func)(spiConfig_t* pConfig);
    static SPI_SetConfig_func SPI_SetConfig;

    typedef spiErr_t (*SPI_GetConfig_func)(spiConfig_t* pConfig);
    static SPI_GetConfig_func SPI_GetConfig;

    typedef spiErr_t (*SPI_WriteSync_func)(unsigned int Data);
    static SPI_WriteSync_func SPI_WriteSync;

    //Important the register address MUST be in the MSB,
    //to write in the register 0x31, the data value MUST be 0x31000000
    spiErr_t writeRegisterSync(unsigned int data);

    typedef spiErr_t (*SPI_ReadSync_func)(unsigned int* pData);
    static SPI_ReadSync_func SPI_ReadSync;

    unsigned int readRegisterSync();

    typedef spiErr_t (*SPI_SetTxAsync_func)(unsigned int Data);
    static SPI_SetTxAsync_func SPI_SetTxAsync;

    typedef spiErr_t (*SPI_GetRxAsync_func)(unsigned int *pData);
    static SPI_GetRxAsync_func SPI_GetRxAsync;

    typedef spiErr_t (*SPI_StartAsync_func)();
    static SPI_StartAsync_func SPI_StartAsync;

    typedef spiErr_t (*SPI_Abort_func)();
    static SPI_Abort_func SPI_Abort;

    typedef spiErr_t (*SPI_SetCallback_func)(spiCallback_t CallbackFunc);
    static SPI_SetCallback_func SPI_SetCallback;

    typedef spiStatus_t (*SPI_GetStatus_func)();
    static SPI_GetStatus_func SPI_GetStatus;

    typedef void (*SPI_ISR_func)();
    static SPI_ISR_func SPI_ISR;
};
__END_SYS

#endif
