// EPOS PLASMA Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

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
