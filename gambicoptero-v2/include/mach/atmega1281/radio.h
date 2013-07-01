// EPOS ATMega1281 Radio (AT86RF230) NIC Mediator Declarations

#ifndef __at86rf230_nic_h
#define __at86rf230_nic_h

#include "at86rf230.h"
#include <radio.h>
#include <cmac.h>

__BEGIN_SYS

class Radio_Wrapper
{
public:
    static const int FRAME_BUFFER_SIZE = AT86RF230::FRAME_MAX_LENGTH;

    typedef AT86RF230 Radio;
    typedef AT86RF230::Event Event;
    typedef unsigned char Address;

    Radio_Wrapper() {}
    ~Radio_Wrapper() {}

    static void init();

    static void set_event_handler(AT86RF230::event_handler * handler);

    int send(unsigned char * data, unsigned int size);

    int receive(unsigned char * data);

    void on();

    void off();

    void sleep();

    void listen();

    void reset();

    bool cca();

    unsigned int lqi();

    unsigned int rssi();

private:
    static AT86RF230 * device;
};

class ATMega1281_Radio: public CMAC<Radio_Wrapper>, Radio_Common
{
public:
    ATMega1281_Radio(int unit = 0): CMAC<Radio_Wrapper>(unit) {}

    ~ATMega1281_Radio() {}

    static void init(unsigned int n) {}

    void reset() {}
};

__END_SYS

#endif

