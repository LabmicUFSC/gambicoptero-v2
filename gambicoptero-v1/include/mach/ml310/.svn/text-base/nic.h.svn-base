// EPOS ML310 NIC Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __ml310_nic_h
#define __ml310_nic_h

#include "machine.h"
#include "e100.h"

__BEGIN_SYS

class ML310_NIC: public NIC_Common
{
private:
    typedef Traits<ML310_NIC>::NICS NICS;
    static const unsigned int UNITS = NICS::Length;

public:
    ML310_NIC() {
        _dev = new Meta_NIC<NICS>::Get<0>::Result;
    }
    template<unsigned int UNIT>
    ML310_NIC(unsigned int u) {
        _dev = new typename Meta_NIC<NICS>::Get<UNIT>::Result(UNIT);
    }
    ~ML310_NIC() {
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

    void reset() { _dev->reset(); }

    unsigned int mtu() const { return _dev->mtu(); }

    const Address & address() { return _dev->address(); }

    const Statistics & statistics() { return _dev->statistics(); }

    static void init();

private:
    Meta_NIC<NICS>::Base * _dev;
};

__END_SYS

#endif
