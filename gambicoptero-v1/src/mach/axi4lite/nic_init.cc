// EPOS AXI4LITE NULL NIC Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <machine.h>
#include <nic.h>

__BEGIN_SYS

/*
template <int unit>
inline static void call_init()
{
    Traits<AXI4LITE_NIC>::NICS::template Get<unit>::Result::init(unit);
    call_init<unit + 1>();
};

template <>
inline static void call_init<Traits<AXI4LITE_NIC>::NICS::Length>() {};
*/

void AXI4LITE_NIC::init()
{
    //return call_init<0>();
    db<AXI4LITE_NIC>(TRC) << "AXI4LITE_NIC::init()\n";
    
}

__END_SYS
