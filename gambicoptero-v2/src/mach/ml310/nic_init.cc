// EPOS ML310 NIC Mediator Initialization

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
