// EPOS PC NIC Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __pc_nic_h
#define __pc_nic_h

#include "machine.h"
#include <ethernet.h>
#include "pcnet32.h"

__BEGIN_SYS

class PC_Ethernet: public Ethernet
{
private:
    typedef Traits<PC_Ethernet>::NICS NICS;
    static const unsigned int UNITS = NICS::Length;

public:
    
    //TODO: fix multiple NIC support
    
    PC_Ethernet(unsigned int u = 0) {
        _dev = new Meta_NIC<NICS>::Get<0>::Result(u);
    }

    ~PC_Ethernet() { delete _dev; }
    
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
