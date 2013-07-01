// EPOS AXI4LITE Mediator Implementation

#include <machine.h>
////extern "C" { void _exit(int s); }

__BEGIN_SYS

// Class attributes
void AXI4LITE::panic() {
    db<AXI4LITE>(ERR) << "PANIC!\n";
	CPU::int_disable();
	CPU::halt();
}

__END_SYS

