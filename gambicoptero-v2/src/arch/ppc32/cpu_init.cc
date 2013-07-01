// EPOS PPC32 CPU Mediator Initialization

#include <cpu.h>
#include <mmu.h>
#include <system.h>

extern "C" { void __epos_library_app_entry(void); }

__BEGIN_SYS

void PPC32::init()
{
    db<Init, PPC32>(TRC) << "PPC32::init()\n";

    // Initialize the MMU
    if(Traits<PPC32_MMU>::enabled)
	PPC32_MMU::init();
    else
	db<Init, PPC32>(WRN) << "MMU is disabled!\n";
}

__END_SYS
