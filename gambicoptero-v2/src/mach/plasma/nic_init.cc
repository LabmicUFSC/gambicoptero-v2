// EPOS PLASMA NULL NIC Mediator Initialization

#include <system/kmalloc.h>
#include <mach/plasma/machine.h>
#include <mach/plasma/nic.h>

__BEGIN_SYS

/*
template <int unit>
inline static void call_init()
{
    Traits<PLASMA_NIC>::NICS::template Get<unit>::Result::init(unit);
    call_init<unit + 1>();
};

template <>
inline static void call_init<Traits<PLASMA_NIC>::NICS::Length>() {};
*/

void PLASMA_NIC::init()
{
    //return call_init<0>();
    db<PLASMA_NIC>(TRC) << "PLASMA_NIC::init()\n";
    
}

__END_SYS
