// EPOS AXI4LITE NULL NIC Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __axi4lite_nic_h
#define __axi4lite_nic_h

#include <nic.h>

__BEGIN_SYS

class AXI4LITE_NIC: public NIC_Common
{
private:
    typedef Traits<AXI4LITE_NIC>::NICS NICS;
    static const unsigned int UNITS = NICS::Length;
  
public:
    typedef NIC_Common::Address<1> Address;
    static const Address BROADCAST;
	typedef unsigned char Protocol;
	static const unsigned int MTU = 1500;
	typedef char PDU[MTU];
	
	enum {
		IP,
		ARP,
		RARP
	};
    
public:
    AXI4LITE_NIC(unsigned int unit = 0) {  }

    ~AXI4LITE_NIC() {}

    int send(const Address & dst, const Protocol & prot, 
	     const void * data, unsigned int size) {
        return 0;
    }

    // Adress<UNITS> ==> UNITS is not what was supposed to be
    int receive(Address * src, Protocol * prot,
		void * data, unsigned int size) {
        return 0;
    }

    void reset() {}
    
    void attach(Observer * obs, const Protocol & prot) {}

    unsigned int mtu() const { return (unsigned int)0; }

    const Address & address() {
        return _Addr;
    }

    const Statistics & statistics() {
	return _Stat;
    }

    static void init();

private:
    Address _Addr;
    Statistics _Stat;
};

__END_SYS

#endif
