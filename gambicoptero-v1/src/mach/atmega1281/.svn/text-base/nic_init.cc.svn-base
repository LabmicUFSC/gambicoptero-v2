// EPOS ATMega1281 NIC Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <mach/atmega1281/machine.h>

__BEGIN_SYS

template <int unit>
inline static void call_init()
{
    typedef typename Traits<ATMega1281_NIC>::NICS::template Get<unit>::Result NIC;
    if(Traits<NIC>::enabled)
        NIC::init(unit);
    call_init<unit + 1>();
};

template <> 
inline void call_init<Traits<ATMega1281_NIC>::NICS::Length>() {};

void ATMega1281_NIC::init()
{
    call_init<0>();
}

__END_SYS

