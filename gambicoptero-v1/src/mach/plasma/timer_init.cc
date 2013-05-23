// EPOS PLASMA Timer Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <timer.h>
#include <ic.h>

__BEGIN_SYS

void PLASMA_Timer::init() {
    
	db<Init, Timer>(TRC) << "PLASMA_Timer::init()\n";

    CPU::int_disable();

	IC::int_vector(IC::IRQ_TIMER, &int_handler);
	IC::int_vector(IC::IRQ_NO_TIMER, &int_handler);
	IC::enable(IC::IRQ_TIMER);
	IC::disable(IC::IRQ_NO_TIMER);

	CPU::int_enable();
	
}

__END_SYS

