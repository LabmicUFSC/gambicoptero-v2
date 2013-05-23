

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
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

