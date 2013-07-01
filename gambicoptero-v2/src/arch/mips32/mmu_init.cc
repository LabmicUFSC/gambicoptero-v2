// EPOS MIPS32 MMU Mediator Initialization

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
