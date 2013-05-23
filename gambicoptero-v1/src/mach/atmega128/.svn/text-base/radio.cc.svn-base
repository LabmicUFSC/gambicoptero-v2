// EPOS ATMega128 Radio (CC1000) NIC Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <mach/atmega128/machine.h>
#include <mach/atmega128/radio.h>

__BEGIN_SYS

// Class attributes
ATMega128_Radio::Device ATMega128_Radio::_devices[UNITS];

// Class Methods
void ATMega128_Radio::int_handler(unsigned int interrupt)
{
    ATMega128_Radio * dev = get(interrupt);

    db<ATMega128_Radio>(TRC) << "ATMega128_Radio::int_handler(int=" << interrupt
		    << ",dev=" << dev << ")\n";
    if(!dev)
	db<ATMega128_Radio>(WRN) << "ATMega128_Radio::int_handler: handler not found\n";
    else 
	dev->handle_int();
}

// Methods
ATMega128_Radio::ATMega128_Radio(unsigned int unit)
{
    db<ATMega128_Radio>(TRC) << "ATMega128_Radio(unit=" << unit << ")\n";

    // Share control
    if(unit >= UNITS) {
	db<ATMega128_Radio>(WRN) << "ATMega128_Radio: requested unit (" << unit 
			 << ") does not exist!\n";
	return;
    }

    // Share control
    if(_devices[unit].in_use) {
	db<ATMega128_Radio>(WRN) << "ATMega128_Radio: device already in use!\n";
	return;
    }
    
    *this = *_devices[unit].device;

    // Lock device
    _devices[unit].in_use = true;
}

ATMega128_Radio::~ATMega128_Radio()
{
    db<ATMega128_Radio>(TRC) << "~ATMega128_Radio(unit=" << _unit << ")\n";

    // Unlock device
    _devices[_unit].in_use = false;
}

ATMega128_Radio::ATMega128_Radio(unsigned int unit, 
		 int io_port, int irq, void * dma_buf)
{
    db<ATMega128_Radio>(TRC) << "ATMega128_Radio(unit=" << unit << ",io=" << io_port 
		     << ",irq=" << irq << ",dma=" << dma_buf << ")\n";

}

void ATMega128_Radio::reset()
{
}

int ATMega128_Radio::send(const Address & dst, const Protocol & prot,
		  const void * data, unsigned int size)
{
    db<ATMega128_Radio>(TRC) << "ATMega128_Radio::send(src=" << _address
		     << ",dst=" << dst
		     << ",prot=" << prot
		     << ",data=" << data
		     << ",size=" << size
		     << ")\n";

    return size;
}

int ATMega128_Radio::receive(Address * src, Protocol * prot,
		     void * data, unsigned int size)
{
    db<ATMega128_Radio>(TRC) << "ATMega128_Radio::receive(src=" << *src
		    << ",prot=" << *prot
		    << ",data=" << data
		    << ",size=" << size
		    << ")\n";

    return 0;
}

void ATMega128_Radio::handle_int()
{
    CPU::int_disable();

    CPU::int_enable();
}

__END_SYS
