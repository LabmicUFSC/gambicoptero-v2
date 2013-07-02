// EPOS MC13224V_I2C Mediator Declarations

#ifndef _MC13224V_I2C_H
#define _MC13224V_I2C_H

#include <system/config.h>
#include <cpu.h>
#include <machine.h>
#include <gpio_pin.h>
#include <tsc.h>

__BEGIN_SYS

class MC13224V_I2C
{
protected:
    static const bool _broadcast = Traits<MC13224V_I2C>::BROADCAST;
    static const unsigned char _freq_div = Traits<MC13224V_I2C>::FREQUENCY_DIVIDER;

    enum RegBits {
        //I2C_ADR
        I2C_ADR_ADDR = 1,
        I2C_ADR_RSV = 0,
        //I2C_CFDR
        I2C_CFDR_RSV = 6,
        I2C_CFDR_FDR = 0,
        //I2C_CR
        I2C_CR_MEN = 7,
        I2C_CR_MIEN = 6,
        I2C_CR_MSTA = 5,
        I2C_CR_MTX = 4,
        I2C_CR_TXAK = 3,
        I2C_CR_RSTA = 2,
        I2C_CR_RSV = 1,
        I2C_CR_BCST = 0,
        //I2C_SR
        I2C_SR_MCF = 7,
        I2C_SR_MASS = 6,
        I2C_SR_MBB = 5,
        I2C_SR_MAL = 4,
        I2C_SR_BCSTM = 3,
        I2C_SR_SRW = 2,
        I2C_SR_MIF = 1,
        I2C_SR_RXAK = 0,
        //I2C_DR
        I2C_DR_DATA = 0,
        //I2C_DFSRR
        I2C_DFSRR_RSV = 6,
        I2C_DFSRR_DFSR = 0,
        //I2C_CKER
        I2C_CKER_RSV = 1,
        I2C_CKER_CKEN = 0
    };
    enum I2C_GPIO_PINS {
        I2C_SCL_GPIO = 12,
        I2C_SDA_GPIO = 13
    };

public:

    MC13224V_I2C();
    ~MC13224V_I2C();

    void enable();
    void disable();

    bool busy();
    void set_timeout(TSC::Time_Stamp tm);

    bool write_register(unsigned char dst, unsigned char reg, unsigned char byte);
    bool write_register(unsigned char dst, unsigned char reg, int len, unsigned char * buff);
    bool read_register(unsigned char dst, unsigned char reg, int len, unsigned char * buff);

    static void init();

private:
    bool sync();
    void start();
    void restart();
    void receive();
    void nack();
    void stop();
    bool put_byte(unsigned char b);
    bool get_byte(unsigned char *b, bool set_nack = false);

    GPIO_Pin _scl;
    GPIO_Pin _sda;

    TSC::Time_Stamp _sync_timeout;
};
__END_SYS

#endif // I2C_H
