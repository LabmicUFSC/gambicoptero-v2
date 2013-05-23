// EPOS AVR8 MMU Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mmu.h>
#include <system.h>

extern "C" unsigned __bss_end;

__BEGIN_SYS

void AVR8_MMU::init()
{
    System_Info<Machine> * si = System::info();

    db<Init, AVR8_MMU>(TRC) << "AVR8_MMU::init()\n";

    db<Init, AVR8_MMU>(INF) << "AVR8_MMU::memory={base=" 
			    << (void *) si->pmm.mem_base << ",size="
			    << (si->bm.mem_top - si->bm.mem_base) / 1024
			    << "KB}\n";
    db<Init, AVR8_MMU>(INF) << "AVR8_MMU::free={base=" 
			    << (void *) si->pmm.free_base << ",size="
			    << (si->pmm.free_top - si->pmm.free_base) / 1024
			    << "KB}\n";
    
    unsigned int base = (unsigned)&__bss_end+1;
    AVR8_MMU::free(base, Memory_Map<Machine>::TOP - base);

}

__END_SYS

