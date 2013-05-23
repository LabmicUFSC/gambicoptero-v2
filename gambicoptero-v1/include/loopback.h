// EPOS Loopback network interface

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __loopback_h
#define __loopback_h

#include <nic.h>
#include <mutex.h>

__BEGIN_SYS

class Loopback : public NIC_Common {

    static const unsigned int MTU = 1024;

public:
    typedef NIC_Common::Address<1> Address;
    typedef NIC_Common::Statistics Statistics;
    typedef NIC_Common::Protocol   Protocol;
    typedef char                   PDU[MTU];

    enum {
        ELP,
        IP,
        ARP,
        RARP
    };

    Loopback(unsigned int unit=0)
    : _unit(unit), _size(0) {}

    int send(const Address& dst,const Protocol& prot,
             const char * data,unsigned int size);

    int receive(Address * src,Protocol * prot,
                char * data,unsigned int size);

    void reset();

    unsigned int mtu() const { return MTU; }

    const Address & address() { return _address; }

    const Statistics & statistics() { return _stats; }

    static void init() {}

protected:
    unsigned int _unit;
    Address      _address;
    Statistics   _stats;
    PDU          _data;
    unsigned int _size;
    Protocol     _prot;
    Mutex        _lock;
};

__END_SYS

#endif
