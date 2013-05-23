// EPOS-- MC13224V NIC Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mc13224v_nic_h
#define __mc13224v_nic_h

#include <nic.h>
#include <cmac.h>
#include "radio.h"
#include "buck_regulator.h"

__BEGIN_SYS

class MC13224V_NIC: public Radio_Common
{
private:
    typedef Traits<MC13224V_NIC>::NICS NICS;
    static const unsigned int UNITS = NICS::Length;

public:
    MC13224V_NIC(unsigned int unit = 0) {
        _dev = new Meta_NIC<NICS>::Get<0>::Result(unit);
    }

    ~MC13224V_NIC() {
        delete _dev;
    }

    int send(const Address & dst, const Protocol & prot,
            const void * data, unsigned int size) {
        return _dev->send(dst, prot, data, size);
    }

    int receive(Address * src, Protocol * prot,
            void * data, unsigned int size) {
        return _dev->receive(src, prot, data, size);
    }

    void reset() {_dev->reset();}

    unsigned int mtu() const {return _dev->mtu();}

    const Address & address() {return _dev->address();}

    const Statistics & statistics() {return _dev->statistics();}

    void attach(Observer * obs, const Protocol & prot) { _dev->attach(obs, prot); }
    void detach(Observer * obs, const Protocol & prot) { _dev->detach(obs, prot); }
    void notify(const Protocol & prot) { _dev->notify(prot); }

    static void init();

    typedef char OP_Mode;
    enum {
        OFF = 0,
        SEND_ONLY = 1,
        RECV_ONLY = 2,
        FULL = 3,
    };

    static OP_Mode power() { return _mode; }
    static void power(OP_Mode mode)
    {
        if(mode == _mode) return;

        switch(mode)
        {
        case FULL:
            MC13224V_Buck_Regulator::Radio_1P5V_txrx_enable();
            init();
            break;
        case SEND_ONLY:
            break;
        case RECV_ONLY:
            break;
        case OFF:
            MC13224V_Buck_Regulator::Radio_1P5V_disable();
            break;
        }
    }

private:
    static OP_Mode _mode;
    Meta_NIC<NICS>::Base * _dev;
};

__END_SYS

#endif

