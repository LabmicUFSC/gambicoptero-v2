// EPOS PC Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>

__BEGIN_SYS

void PC::init()
{
    db<Init, PC>(TRC) << "PC::init()\n";

    if(Traits<PC_IC>::enabled)
	PC_IC::init();
    if(Traits<PC_PCI>::enabled)
	PC_PCI::init();
    if(Traits<PC_Timer>::enabled)
	PC_Timer::init();
    if(Traits<PC_Ethernet>::enabled)
	PC_Ethernet::init();
}

__END_SYS
