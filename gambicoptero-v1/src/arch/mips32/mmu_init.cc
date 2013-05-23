// EPOS MIPS32 MMU Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mmu.h>
#include <system.h>

__BEGIN_SYS

void MIPS32_MMU::init()
{
    db<MIPS32_MMU>(TRC) << "MIPS32_MMU::init()\n";

    System_Info<Machine> * si = System::info();
    db<Init, MIPS32_MMU>(INF) << "MIPS32_MMU::memory={base=" 
			     << (void *) si->pmm.mem_base << ",size="
			     << (si->bm.mem_top - si->bm.mem_base) / 1024
			     << "KB}\n";
    db<Init, MIPS32_MMU>(INF) << "MIPS32_MMU::free={base=" 
			     << (void *) si->pmm.free_base << ",size="
			     << (si->pmm.free_top - si->pmm.free_base) / 1024
			     << "KB}\n";

    // Insert all free memory into the free list
    free(si->pmm.free_base, si->pmm.free_top - si->pmm.free_base);

}

__END_SYS
