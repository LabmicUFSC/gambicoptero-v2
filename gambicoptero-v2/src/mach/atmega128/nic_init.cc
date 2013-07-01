// EPOS ATMega128 NIC Mediator Initialization

#include <system/kmalloc.h>
#include <mach/atmega128/machine.h>

__BEGIN_SYS

template <int unit>
inline void call_init()
{
    typedef typename Traits<ATMega128_NIC>::NICS::template Get<unit>::Result NIC;
    if(Traits<NIC>::enabled)
        NIC::init(unit);
    call_init<unit + 1>();
};

template <> 
inline void call_init<Traits<ATMega128_NIC>::NICS::Length>() {};

void ATMega128_NIC::init()
{
    call_init<0>();
}

__END_SYS
