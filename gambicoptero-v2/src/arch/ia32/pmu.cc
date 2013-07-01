// EPOS IA32 PMU Mediator Implementation

#include <arch/ia32/pmu.h>

__BEGIN_SYS

// Class attributes
int IA32_PMU::_version;
int IA32_PMU::_max_events;
IA32_PMU::Reg32 IA32_PMU::_num_counters;
IA32_PMU::Reg32 IA32_PMU::_num_counters_fixed;
int IA32_PMU::_cntval_bits;
IA32_PMU::Reg64 IA32_PMU::_cntval_mask;
IA32_PMU::Reg64 IA32_PMU::_max_period;
IA32_PMU::Reg64 IA32_PMU::_intel_ctrl;
IA32_PMU::perf_capabilities IA32_PMU::_intel_cap;
IA32_PMU::cpuinfo_x86 IA32_PMU::_cpuinfo;

__END_SYS
