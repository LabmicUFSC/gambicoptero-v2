#ifndef __integrator_nic_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __integrator_nic_h

#include <ethernet.h>

__BEGIN_SYS

class IntegratorCP_NIC: public Ethernet
{
public:
    
    IntegratorCP_NIC(unsigned int u = 0) {
    }

    ~IntegratorCP_NIC() { }
    
    int send(const Address & dst, const Protocol & prot, 
             const void * data, unsigned int size) {
        return 0; 
    }

    int receive(Address * src, Protocol * prot,
                void * data, unsigned int size) {
        return 0; 
    }

    void reset() { }

    unsigned int mtu() const { return 0; }
    
    const Address & address() { return _address; }

    const Statistics & statistics() { return _statistics; }

    static void init();

private:
    Address _address;
    Statistics _statistics;
};

__END_SYS

#endif
