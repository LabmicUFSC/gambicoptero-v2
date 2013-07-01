// EPOS IA32 PMU Mediator Initialization

#include <pmu.h>

__BEGIN_SYS


void IA32_PMU::init()
{
    db<Init, IA32_PMU>(TRC) << "IA32_PMU::init()\n";
    
    /* Part of the code is from 
 	 * linux/arch/x86/kernel/cpu/perf_event_intel.c
	 */
    cpuid10_edx edx;
    cpuid10_eax eax;
    Reg32 unused = 0;
    Reg32 ebx;
    

    /* PCE - Performance-Monitoring Counter Enable (bit 8 of CR4)
     * Enables execution of the RDPMC instruction for programs or procedures running at any protection level when set; 
     * RDPMC instruction can be executed only at protection level 0 when clear.
     */
    IA32::cr4((IA32::cr4() | PSE));
    
    /* Get vendor name */
    cpuid(0x00000000, (Reg32 *)&_cpuinfo.cpuid_level,
	  (Reg32 *)&_cpuinfo.x86_vendor_id[0],
	  (Reg32 *)&_cpuinfo.x86_vendor_id[8],
	  (Reg32 *)&_cpuinfo.x86_vendor_id[4]);
    
    _cpuinfo.x86 = 4;
    /* Intel-defined flags: level 0x00000001 */
    if (_cpuinfo.cpuid_level >= 0x00000001) {
	Reg32 junk = 0;
	Reg32 tfms, cap0, misc;

	cpuid(0x00000001, &tfms, &misc, &junk, &cap0);
	_cpuinfo.x86 = (tfms >> 8) & 0xf;
	_cpuinfo.x86_model = (tfms >> 4) & 0xf;
	_cpuinfo.x86_mask = tfms & 0xf;

	if (_cpuinfo.x86 == 0xf)
		_cpuinfo.x86 += (tfms >> 20) & 0xff;
	if (_cpuinfo.x86 >= 0x6)
		_cpuinfo.x86_model += ((tfms >> 16) & 0xf) << 4;

	if (cap0 & (1<<19)) {
		_cpuinfo.x86_clflush_size = ((misc >> 8) & 0xff) * 8;
		_cpuinfo.x86_cache_alignment = _cpuinfo.x86_clflush_size;
	}
    }
    
    switch(_cpuinfo.x86_model) {
		case 14: /* 65 nm core solo/duo, "Yonah" */
        case 15: /* original 65 nm celeron/pentium/core2/xeon, "Merom"/"Conroe" */        
		case 22: /* single-core 65 nm celeron/core2solo "Merom-L"/"Conroe-L" */
        case 23: /* current 45 nm celeron/core2/xeon "Penryn"/"Wolfdale" */
        case 29: /* six-core 45 nm xeon "Dunnington" */
        case 26: /* 45 nm nehalem, "Bloomfield" */
        case 30: /* 45 nm nehalem, "Lynnfield" */
        case 46: /* 45 nm nehalem-ex, "Beckton" */
        case 28: /* Atom */
        case 37: /* 32 nm nehalem, "Clarkdale" */
        case 44: /* 32 nm nehalem, "Gulftown" */              
        default:
	    /*
	      * default constraints for v2 and up
	      */
	    //x86_pmu.event_constraints = intel_gen_event_constraints;
	    break;
    }
    
    /*
     * Get PerfMon information
     */
    cpuid(10, &eax.full, &ebx, &unused, &edx.full);

    _version = eax.split.version_id;
    /*if (version < 2)
	x86_pmu = core_pmu;
    else
	x86_pmu = intel_pmu;*/

    _num_counters            = eax.split.num_counters;
    _cntval_bits             = eax.split.bit_width;
    _cntval_mask             = (1ULL << eax.split.bit_width) - 1;
    
    /*
    * Quirk: v2 perfmon does not report fixed-purpose events, so
    * assume at least 3 events:
    */
    if (_version > 1)
	_num_counters_fixed = ((int)edx.split.num_counters_fixed) > 3 ? (int)edx.split.num_counters_fixed : 3;

    /*
      * v2 and above have a perf capabilities MSR
      */
    if (_version > 1) {
	_intel_cap.capabilities = rdmsr(CAPABILITIES);
    }
    
    //intel_ds_init // init PEBS
    
}

__END_SYS
