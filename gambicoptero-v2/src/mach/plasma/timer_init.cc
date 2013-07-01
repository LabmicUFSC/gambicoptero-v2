// EPOS PLASMA Timer Mediator Initialization

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

