// EPOS AVR8 MMU Mediator Initialization

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

