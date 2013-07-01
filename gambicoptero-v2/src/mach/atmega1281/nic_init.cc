// EPOS ATMega1281 NIC Mediator Initialization

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

