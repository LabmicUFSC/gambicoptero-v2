
// EPOS PLASMA Mediator Implementation

#include <mach/plasma/machine.h>
////extern "C" { void _exit(int s); }

__BEGIN_SYS

// Class attributes
void PLASMA::panic() {
        db<PLASMA>(ERR) << "PANIC!\n";
	CPU::int_disable();
	CPU::halt();
}

__END_SYS

