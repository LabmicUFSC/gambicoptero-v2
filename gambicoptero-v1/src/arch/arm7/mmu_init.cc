// EPOS-- ARM7 MMU Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mmu.h>
#include <system.h>

extern "C" unsigned __bss_end__;

__BEGIN_SYS

void ARM7_MMU::init()
{
    db<Init, ARM7_MMU>(TRC) << "ARM7_MMU::init()\n";
    
    unsigned int base = (unsigned int)&__bss_end__+1;
	if(base%4 != 0){
		base = base + (4 - base%4);
	}

	// let our stack breath!
	const unsigned int limit = 
		Memory_Map<Machine>::TOP - Traits<Machine>::APPLICATION_STACK_SIZE;
    ARM7_MMU::free(base, limit - base);
}

__END_SYS

