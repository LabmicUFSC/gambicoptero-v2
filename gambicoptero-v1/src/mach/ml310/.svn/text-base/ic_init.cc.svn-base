// EPOS ML310_IC Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>

__BEGIN_SYS

// Class initialization
void ML310_IC::init()
{
    db<ML310_IC>(TRC) << "ML310_IC::init()\n";

    PPC32::int_disable();

    //Master Enable and Hardware Generation of Interrupts !!!
    *(volatile unsigned int*)(MER) = 0x00000003;

    //Disable All Interrupts (Will be enabled on each HW init)
    disable();

    PPC32::int_enable();
}

__END_SYS
