// EPOS ATMega16 Radio (CC1000) NIC Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __cc1000_nic_h
#define __cc1000_nic_h

#include "../avr_common/cc1000.h"

__BEGIN_SYS

class Radio: public Ethernet_NIC, private CC1000
{
private:
    static const unsigned int UNITS =
	Traits<ATMega16_NIC>::RADIO_UNITS;

    // Share control and interrupt dispatiching info
    struct Device
    {
	Radio * device;
	unsigned int interrupt;
	bool in_use;
    };
	
public:
    Radio(unsigned int unit = 0);
    ~Radio();

    int send(const Address & dst, const Protocol & prot,
 	     const void * data, unsigned int size);
    int receive(Address * src, Protocol * prot,
		void * data, unsigned int size);

    void reset();

    unsigned int mtu() { return MTU; }

    const Address & address() { return _address; }

    const Statistics & statistics() { return _statistics; }

    static void init(unsigned int unit);

private:
    Radio(unsigned int unit, int io_port, int irq, void * dma);

    void handle_int();

    static void int_handler(unsigned int interrupt);

    static Radio * get(unsigned int interrupt) {
	for(unsigned int i = 0; i < UNITS; i++)
	    if(_devices[i].interrupt == interrupt)
		return _devices[i].device;
	return 0;
    };

private:
    unsigned int _unit;

    Address _address;
    Statistics _statistics;

    int _irq;
    void * _dma_buf;

    static Device _devices[UNITS];
};

__END_SYS

#endif
