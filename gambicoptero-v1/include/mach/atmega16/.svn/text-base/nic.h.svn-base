// EPOS ATMega16 NIC Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega16_nic_h
#define __atmega16_nic_h

#include <radio.h>

__BEGIN_SYS

class ATMega16_NIC: public Radio_Common
{

public:
    ATMega16_NIC() {    }
    ATMega16_NIC(unsigned int u) {    }
    ~ATMega16_NIC() {    }
    
    int send(const Address & dst, const Protocol & prot, 
	     const void * data, unsigned int size) {
	return 0;
    }
    int receive(Address * src, Protocol * prot,
		void * data, unsigned int size) {
	return 0;
    }
    
    void reset() { }

    unsigned int mtu() const {return 0; }
    
    const Address & address() { return _addr; }

    const Statistics & statistics() { return _stats; }

    static void init();

 private:

    Address _addr;
    Statistics _stats;

};

__END_SYS

#endif
