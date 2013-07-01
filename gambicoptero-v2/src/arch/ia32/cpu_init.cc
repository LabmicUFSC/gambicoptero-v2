// EPOS IA32 CPU Mediator Initialization

#include <cpu.h>
#include <mmu.h>
#include <pmu.h>
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

	// Initialize the PMU
    if(Traits<IA32_PMU>::enabled)
    IA32_PMU::init();
    else 
    db<Init, IA32_PMU>(WRN) << "PMU is disabled!\n";
}

__END_SYS
