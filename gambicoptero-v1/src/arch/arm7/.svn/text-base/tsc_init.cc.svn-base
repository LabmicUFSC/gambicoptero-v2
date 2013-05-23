// EPOS-- ARM7 TSC Mediator Initialization

#include <tsc.h>
#include <system.h>

#include <ic.h>

__BEGIN_SYS

void ARM7_TSC::init()
{
   // db<Init, ARM7_TSC>(TRC) << "ARM7_TSC::init()\n";

    ctrl(0x20); // until CNTR reach COMP1
    sctrl(0x0); // reset SCTRL
    load(0x0); // reset LOAD

    comp1(0xffff);
    cmpld1(0xffff);

    csctrl(0x41);

    ctrl( (0x8<<9) | ctrl() );
    cntr(0x0);
    ctrl( (0x1<<13) | ctrl() );

    IC::int_vector(IC::IRQ_TSC, tsc_timer_handler);
    IC::enable(IC::IRQ_TSC);
    enbl(enbl() | 0x1<<instance);

}

__END_SYS

