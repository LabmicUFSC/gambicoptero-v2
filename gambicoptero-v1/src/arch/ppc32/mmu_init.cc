// EPOS PPC32 MMU Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mmu.h>
#include <system.h>

__BEGIN_SYS

void PPC32_MMU::init()
{
    db<PPC32_MMU>(TRC) << "PPC32_MMU::init()\n";

    System_Info<Machine> * si = System::info();
    db<Init, PPC32_MMU>(INF) << "PPC32_MMU::memory={base=" 
			     << (void *) si->pmm.mem_base << ",size="
			     << (si->bm.mem_top - si->bm.mem_base) / 1024
			     << "KB}\n";
    db<Init, PPC32_MMU>(INF) << "PPC32_MMU::free1={base=" 
			     << (void *) si->pmm.free1_base << ",size="
			     << (si->pmm.free1_top - si->pmm.free1_base) / 1024
			     << "KB}\n";
    db<Init, PPC32_MMU>(INF) << "PPC32_MMU::free2={base=" 
			     << (void *) si->pmm.free2_base << ",size="
			     << (si->pmm.free2_top - si->pmm.free2_base) / 1024
			     << "KB}\n";

    // Insert all free memory into the free list
    free(si->pmm.free1_base, si->pmm.free1_top -si->pmm.free1_base);
    free(si->pmm.free2_base, si->pmm.free2_top -si->pmm.free2_base);

}

__END_SYS
