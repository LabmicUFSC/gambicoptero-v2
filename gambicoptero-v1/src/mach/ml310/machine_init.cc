// EPOS ML310 Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>
#include <system.h>

extern "C" { void __epos_library_app_entry(void); }

__BEGIN_SYS

void ML310::init()
{
    db<ML310>(TRC) << "ML310::init()\n";

    // Set all interrupt handlers to int_not()
    for(unsigned int i = 0; i < INT_VECTOR_SIZE; i++)
       int_vector(i, int_not);

    //Install Processor Exception Handlers
    int_vector(ML310_IC::INT_PROGRAM, exc_program);

    // Initialize the hardware
    if(Traits<ML310_PCI>::enabled)
	ML310_PCI::init();
    if(Traits<ML310_IC>::enabled)
	ML310_IC::init();
    if(Traits<ML310_Timer>::enabled)
	ML310_Timer::init();
    if(Traits<ML310_UART>::enabled)
	ML310_UART::init();
    if(Traits<ML310_NIC>::enabled)
	ML310_NIC::init();

}

__END_SYS
