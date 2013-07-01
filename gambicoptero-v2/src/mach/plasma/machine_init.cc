// EPOS PLASMA Mediator Initialization

#include <machine.h>
#include <system.h>
#include <uart.h>

extern "C" { void __epos_library_app_entry(void); }

__BEGIN_SYS

void PLASMA::init()
{
    db<PLASMA>(TRC) << "PLASMA::init()\n";

    if(Traits<PLASMA_IC>::enabled)
	PLASMA_IC::init();
    if(Traits<PLASMA_Timer>::enabled)
	PLASMA_Timer::init();
    if(Traits<PLASMA_UART>::enabled)
	PLASMA_UART::init();
    if(Traits<PLASMA_NIC>::enabled)
	PLASMA_NIC::init();
}

__END_SYS
