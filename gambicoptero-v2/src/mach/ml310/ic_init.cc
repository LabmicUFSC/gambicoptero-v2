// EPOS ML310_IC Initialization

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
