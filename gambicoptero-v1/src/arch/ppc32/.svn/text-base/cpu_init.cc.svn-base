// EPOS PPC32 CPU Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

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
