// EPOS ML310 NIC Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <mach/ml310/machine.h>
#include <mach/ml310/nic.h>

__BEGIN_SYS

template <int unit>
inline static void call_init()
{
    Traits<ML310_NIC>::NICS::template Get<unit>::Result::init(unit);
    call_init<unit + 1>();
};

template <>
inline static void call_init<Traits<ML310_NIC>::NICS::Length>() {};

void ML310_NIC::init()
{
    return call_init<0>();
}

__END_SYS
