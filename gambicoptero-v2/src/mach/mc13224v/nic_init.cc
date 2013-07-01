// EPOS-- MC13224V NIC Mediator Initialization

#include <system/kmalloc.h>
#include <mach/mc13224v/machine.h>

__BEGIN_SYS

MC13224V_NIC::OP_Mode MC13224V_NIC::_mode = 0;

template <int unit>
inline static void call_init()
{
    typedef typename Traits<MC13224V_NIC>::NICS::template Get<unit>::Result NIC;
    if(Traits<NIC>::enabled)
        NIC::init(unit);
    call_init<unit + 1>();
};

template <> 
inline void call_init<Traits<MC13224V_NIC>::NICS::Length>()
{
};

void MC13224V_NIC::init()
{
    call_init<0>();
}

__END_SYS

