// EPOS IA32 CPU Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <cpu.h>
#include <mmu.h>
#include <system.h>
#include <system/info.h>

extern "C" { void __epos_library_app_entry(void); }

__BEGIN_SYS

void IA32::init()
{
    db<Init, IA32>(TRC) << "IA32::init()\n";

    _cpu_clock = System::info()->tm.cpu_clock;
    _bus_clock = System::info()->tm.bus_clock;

    // Initialize the MMU
    if(Traits<IA32_MMU>::enabled)
	IA32_MMU::init();
    else
	db<Init, IA32>(WRN) << "MMU is disabled!\n";
}

__END_SYS
