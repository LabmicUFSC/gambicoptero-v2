// EPOS PC Timer Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <timer.h>
#include <ic.h>

__BEGIN_SYS

int PC_Timer::init()
{
    db<Init, Timer>(TRC) << "Timer::init()\n";

    CPU::int_disable();
    
    Engine::config(0, Engine::clock() / FREQUENCY);

    IC::int_vector(IC::INT_TIMER, int_handler);
    IC::enable(IC::INT_TIMER);

    CPU::int_enable();

    return 0;
}

__END_SYS
