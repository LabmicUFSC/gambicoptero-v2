// EPOS-- MC13224V (Transceiver) NIC Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mc13224v_radio_h
#define __mc13224v_radio_h

#include <radio.h>
#include <cmac.h>
#include "transceiver.h"

__BEGIN_SYS

class Radio_Wrapper
{
public:
    static const int FRAME_BUFFER_SIZE = MC13224V_Transceiver::MAX_PACKET_SIZE;

    enum {
        SFD_DETECTED,
        FRAME_END
    };

    typedef MC13224V_Transceiver::Event Event;
    typedef unsigned char Address;

    Radio_Wrapper() {}
    ~Radio_Wrapper() {}

    static void init();

    static void set_event_handler(MC13224V_Transceiver::event_handler * handler);

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
    static MC13224V_Transceiver * device;
};

class MC13224V_Radio: public CMAC<Radio_Wrapper>, Radio_Common
{
public:
    MC13224V_Radio(int unit = 0): CMAC<Radio_Wrapper>(unit) {}

    ~MC13224V_Radio() {}

    static void init(unsigned int n) {}

    void reset() {}
};

__END_SYS

#endif

