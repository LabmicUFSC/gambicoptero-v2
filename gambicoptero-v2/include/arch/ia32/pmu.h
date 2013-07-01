// EPOS IA32 PMU Mediator Declarations

#ifndef __ia32_pmu_h
#define __ia32_pmu_h

#include <cpu.h>
#include <pmu.h>
#include <ic.h>
#include <utility/handler.h>

__BEGIN_SYS

// Non-Architectural PMU support
class IA32_PMU: public PMU_Common
{
public:  
    typedef CPU::Reg8  Reg8;
    typedef CPU::Reg16 Reg16;
    typedef CPU::Reg32 Reg32;
    typedef CPU::Reg64 Reg64;
    typedef CPU::Log_Addr Log_Addr;
public:
    IA32_PMU() {}
    
    static void init();
    
    //Architectural PM Version 1 Section 30.2.1.1
    //MAR address range between 0x40000000 to 0x400000FF
    enum {
	MSR_BASE = 0x40000000,
	PERFEVTSEL_BASE = 0x0186
    };
    
    //Performance Monitoring Counters - used as input to the rdpmc instruction
    enum {
	PMC0 = 0x0000, //PERFCTR0
	PMC1 = 0x0001, //PERFCTR1
	PMC2 = 0x0002,
	PMC3 = 0x0003,
	PMC4 = 0x0004,
	PMC5 = 0x0005,
	PMC6 = 0x0006,
	PMC7 = 0x0007,
    };
    
    //Memory-mapped PMU related registers - Apprendix B - MSRs
    enum {
	PMC_BASE_ADDR = 0x00C1, //PERFCTR0
	/*PMC1_ADDR = 0x00C2, //PERFCTR1
	PMC2_ADDR = 0x00C3,
	PMC3_ADDR = 0x00C4,
	PMC4_ADDR = 0x00C5,
	PMC5_ADDR = 0x00C6,
	PMC6_ADDR = 0x00C7,
	PMC7_ADDR = 0x00C8,*/
	MPERF = 0x00E7,
	APERF = 0x00E8,
	EVTSEL0 = 0x0186,
	EVTSEL1 = 0x0187,
	EVTSEL2 = 0x0188,
	EVTSEL3 = 0x0189,
	STATUS = 0x0198,
	CTL 	  = 0x0199,
	FIXED_CTR0  = 0x0309,
	FIXED_CTR1  = 0x030A,
	FIXED_CTR2  = 0x030B,
	CAPABILITIES  = 0x0345,
	FIXED_CTR_CTL = 0x038D,
	GLOBAL_STATUS = 0x038E,
	GLOBAL_CTRL   = 0x038F,
	GLOBAL_OVF    = 0x0390,
	PEBS_ENABLE   = 0x03F1
    };
    
    //Flags
    enum {
	//EVTSEL registers
	USR  	= 0x10000, // bit 16
	OS   	= 0x20000, // bit 17
	EDGE 	= 0x40000, // bit 18
	PC  	= 0x80000, // bit 19
	INT	= 0x100000, // bit 20
	LOGICAL = 0x200000, //bit 21
	ENABLE 	= 0x400000, // bit 22
	INV 	= 0x800000, //bit 23 invert the CMASK
	
	//CR4 Performance Counter Enable
	PSE	= 0x0100
    };
    
    /* Pre-defined architectural performance events. From Intel Architecture Software Developer's Manual Section 30.2.
     * Bit Position CPUID.AH.EBX - Event Name - UMask - Event Select
     * 0 - UnHalted Core Cycles - 00H - 3CH
     * 1 - Instruction Retired -  00H - C0H
     * 2 - UnHalted Reference Cycles - 01H - 3CH
     * 3 - LLC Reference - 4FH - 2EH
     * 4 - LLC Misses - 41H - 2EH
     * 5 - Branch Instruction Retired - 00H - C4H
     * 6 - Branch Misses Retired - 00H - C5H
     */
    enum {
	UNHALTED_CORE	 	= 0x3C,
	INST_RET		    = 0xC0,
	UNHALTED_REF		= 0x3C | (0x01 << 8),
	LLC_REF			    = 0x2E | (0x4F << 8),
	LLC_MISSES		    = 0x2E | (0x41 << 8),
	BRANCH_INST		    = 0xC4,
	BRANCH_MISSES		= 0xC5
    };
    
    static void cpuid(Reg32 op, Reg32 *eax, Reg32 *ebx,
                                Reg32 *ecx, Reg32 *edx)
    {
	*eax = op;
	/* ecx is often an input as well as an output. */
	asm volatile("cpuid"
	    : "=a" (*eax),
	      "=b" (*ebx),
	      "=c" (*ecx),
	      "=d" (*edx)
	    : "0" (*eax), "2" (*ecx));
    }

    
    static void config(Reg32 event_reg, Reg32 flags) {
        db<IA32_PMU>(TRC) << "IA32_PMU::config()\n";
        wrmsr(event_reg, flags);
    }
    
    static Reg64 rdmsr(Reg32 reg) {
        db<IA32_PMU>(TRC) << "IA32_PMU::rdmsr()\n";
        Reg64 v;
        asm volatile("rdmsr" : "=A" (v) : "c" (reg));
        return v;
    }
    
    static void wrmsr(Reg32 reg, Reg64 v) {
        db<IA32_PMU>(TRC) << "IA32_PMU::wrmsr()\n";
        Reg32 l, h;
        (void)((l) = (Reg32)v);
        (void)((h) = (Reg32)(v >> 32));
        asm volatile("wrmsr" : : "c" (reg), "a"(l), "d" (h) : "memory");
    }
    
    static void reset(Reg32 reg) {
        db<IA32_PMU>(TRC) << "IA32_PMU::reset()\n";
        int l = 0;
        int h = 0;
        asm volatile("wrmsr" : : "c" (reg), "a"(l), "d" (h) : "memory");
    }
    
    /*
     * both i386 and x86_64 returns 64-bit value in edx:eax, but gcc's "A"
     * constraint has different meanings. For i386, "A" means exactly
     * edx:eax, while for x86_64 it doesn't mean rdx:rax or edx:eax. Instead,
     * it means rax *or* rdx.
     */
    static Reg64 rdpmc(Reg32 counter) {
        db<IA32_PMU>(TRC) << "IA32_PMU::rdpmc()\n";
        Reg64 val;
            asm volatile("rdpmc" : "=A" (val) : "c" (counter));
            return val;
    }
    
    static void enable(Reg32 reg) {
        db<IA32_PMU>(TRC) << "IA32_PMU::enable()\n";
        wrmsr(reg, (rdmsr(reg) | ENABLE));
    }
    
    static void disable(Reg32 reg) {
        db<IA32_PMU>(TRC) << "IA32_PMU::disable()\n";
        wrmsr(reg, (rdmsr(reg) & ~ENABLE));
    }
    
    static Reg32 num_counters() { return _num_counters; }
    
private:
    /*
    * Intel "Architectural Performance Monitoring" CPUID
    * detection/enumeration details:
    */
    typedef union {
	struct {
	    Reg32 version_id:8;
	    Reg32 num_counters:8;
	    Reg32 bit_width:8;
	    Reg32 mask_length:8;
	} split;
	Reg32 full;
    } cpuid10_eax;

    typedef union {
	struct {
	    Reg32 num_counters_fixed:5;
	    Reg32 bit_width_fixed:8;
	    Reg32 reserved:19;
	} split;
	Reg32 full;
    } cpuid10_edx;
    
    typedef union {
        struct {
	    Reg64 lbr_format    : 6;
	    Reg64 pebs_trap     : 1;
	    Reg64 pebs_arch_reg : 1;
	    Reg64 pebs_format   : 4;
	    Reg64 smm_freeze    : 1;
        };
        Reg64 capabilities;
    } perf_capabilities;

    typedef struct {
        Reg8                    x86;            /* CPU family */
        Reg8                    x86_vendor;     /* CPU vendor */
        Reg8                    x86_model;
        Reg8                    x86_mask;
	/* Maximum supported CPUID level, -1=no CPUID: */
        int                     cpuid_level;
        Reg32                   x86_capability[10];
        char                    x86_vendor_id[16];
        char                    x86_model_id[64];
        /* in KB - valid for CPUS which support this call: */
        int                     x86_cache_size;
        int                     x86_cache_alignment;    /* In bytes */
        int                     x86_power;
        Reg32           	    loops_per_jiffy;
	/* cpuid returned max cores value: */
        Reg16                     x86_max_cores;
        Reg16                     apicid;
        Reg16                     initial_apicid;
        Reg16                     x86_clflush_size;
    } cpuinfo_x86;
    
    static int _version;
    static int _max_events;
    static Reg32 _num_counters;
    static Reg32 _num_counters_fixed;
    static int _cntval_bits;
    static Reg64 _cntval_mask;
    //static int apic;
    static Reg64 _max_period;
    /*
     * Intel Arch Perfmon v2+
     */
    static Reg64 _intel_ctrl;
    static perf_capabilities _intel_cap;
    static cpuinfo_x86 _cpuinfo;
};

class Intel_PMU_Version2 : public IA32_PMU
{
public:
    //Flags
    enum {
    //GLOBAL CTRL MSR
    PMC0_ENABLE = 0x1,
    PMC1_ENABLE = 0x2,
    FIXED_CTR0_ENABLE = (0x1LLU << 32),
    FIXED_CTR1_ENABLE = (0x1LLU << 33),
    FIXED_CTR2_ENABLE = (0x1LLU << 34),
    
    //FIXED_CTR_CTRL0
    FIXED_CTR0_OS           = 0x01,
    FIXED_CTR0_USER         = 0x02,
    FIXED_CTR0_ALL          = 0x03,
    FIXED_CTR0_PMI          = (0x01 << 3),
    //FIXED_CTR_CTRL1
    FIXED_CTR1_OS           = (0x01 << 4),
    FIXED_CTR1_USER         = (0x02 << 4),
    FIXED_CTR1_ALL          = (0x03 << 4),
    FIXED_CTR1_PMI          = (0x01 << 7),
    //FIXED_CTR_CTRL2
    FIXED_CTR2_OS           = (0x01 << 8),
    FIXED_CTR2_USER         = (0x02 << 8),
    FIXED_CTR2_ALL          = (0x03 << 8),
    FIXED_CTR2_PMI          = (0x01 << 11),
    
    //GLOBAL STATUS 
    PMC0_OVF = 0x01,
    PMC1_OVF = (1 << 0x01),
    FIXED_CTR0_OVF = (1LLU << 0x20),
    FIXED_CTR1_OVF = (1LLU << 0x21),
    FIXED_CTR2_OVF = (1LLU << 0x22),
    
    };
  
    static int config(Reg32 reg, Reg32 flags) {
        db<IA32_PMU>(TRC) << "Intel_Core_Duo_PMU::config()\n";
        if((reg - EVTSEL0) > num_counters()) {
            db<IA32_PMU>(WRN) << "Intel_Core_Duo_PMU::config() the number of available counters is " << num_counters() << "\n";
            return 0; //error
        }
        PMU::config(reg, flags);
        return 1;
    }
    
    static void enable(Reg32 reg) {
        db<IA32_PMU>(TRC) << "IA32_PMU::enable()\n";
        wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | (reg == EVTSEL0 ? PMC0_OVF : PMC1_OVF))); //clear OVF flag
        wrmsr(GLOBAL_CTRL, (rdmsr(GLOBAL_CTRL) | (reg == EVTSEL0 ?  PMC0_ENABLE :  PMC1_ENABLE)));
    }
    
    static void disable(Reg32 reg) {
        db<IA32_PMU>(TRC) << "IA32_PMU::disable()\n";
        wrmsr(GLOBAL_CTRL, (rdmsr(GLOBAL_CTRL) & ~(reg == EVTSEL0 ?  PMC0_ENABLE :  PMC1_ENABLE)));
    }
    
    static bool overflow(Reg32 reg) {
        db<IA32_PMU>(TRC) << "IA32_PMU::overflow()\n";
        //kout << "GLOBAL_STATUS = " << rdmsr(GLOBAL_STATUS) << "\n";
        return (rdmsr(GLOBAL_STATUS) & (reg == PMC0 ? PMC0_OVF : PMC1_OVF)) != 0;
    }
    
    static bool fixed_ctr0_overflow(void) {
        //long long global_status = rdmsr(GLOBAL_STATUS);
        //kout << "global_status = " << global_status << "\n";
        return (rdmsr(GLOBAL_STATUS) & FIXED_CTR0_OVF) != 0;
    }
    
    static void enable_fixed_ctr0(void) {
        //wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR0_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) | FIXED_CTR0_ALL));
        wrmsr(GLOBAL_CTRL, (rdmsr(GLOBAL_CTRL) | FIXED_CTR0_ENABLE));
    }
    
    static void enable_fixed_ctr1(void) {
        //wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR1_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) | FIXED_CTR1_ALL));
        wrmsr(GLOBAL_CTRL, (rdmsr(GLOBAL_CTRL) | FIXED_CTR1_ENABLE));
    }
    
    static void enable_fixed_ctr2(void) {
        //wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR2_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) | FIXED_CTR2_ALL));
        wrmsr(GLOBAL_CTRL, (rdmsr(GLOBAL_CTRL) | FIXED_CTR2_ENABLE));
    }
    
    static void disable_fixed_ctr0(void) {
        wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR0_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) & ~FIXED_CTR0_ALL));
    }
    
    static void disable_fixed_ctr1(void) {
        wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR1_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) & ~FIXED_CTR1_ALL));
    }
    
    static void disable_fixed_ctr2(void) {
        wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR2_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) & ~FIXED_CTR2_ALL));
    }
  
};

class Intel_PMU_Version3 : public Intel_PMU_Version2
{
public:
    // flags
    enum {
    // PERFEVTSEL MSR any thread bit figure 18-16
    ANY_THREAD = (0x01 << 21),
        
    //GLOBAL STATUS and GLOBAL OVF STATUS
    PMC0_OVF = 0x01,
    PMC1_OVF = (1 << 0x01),
    PMC2_OVF = (1 << 0x02),
    PMC3_OVF = (1 << 0x03),
    PMC4_OVF = (1 << 0x04),
    PMC5_OVF = (1 << 0x05),
    PMC6_OVF = (1 << 0x06),
    PMC7_OVF = (1 << 0x07),
    FIXED_CTR0_OVF = (1LLU << 0x20),
    FIXED_CTR1_OVF = (1LLU << 0x21),
    FIXED_CTR2_OVF = (1LLU << 0x22),
    OVF_BUFFER = (1LLU << 0x3E),
    COND_CHGD = (1LLU << 0x3F)
    };
    
    // FIXED_CTR_CTL MSR any thread bit
    enum {
        ANY_THREAD_FIXED0 = (0x01 << 2),
        ANY_THREAD_FIXED1 = (0x01 << 6),
        ANY_THREAD_FIXED2 = (0x01 << 10),
    };
    
    static bool fixed_ctr0_overflow(void) {
        //long long global_status = rdmsr(GLOBAL_STATUS);
        //kout << "global_status = " << global_status << "\n";
        return (rdmsr(GLOBAL_STATUS) & FIXED_CTR0_OVF) != 0;
    }
    
    // INST_RETIRED.ANY
    static void enable_fixed_ctr0(void) {
        //wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR0_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) | FIXED_CTR0_ALL));
        wrmsr(GLOBAL_CTRL, (rdmsr(GLOBAL_CTRL) | FIXED_CTR0_ENABLE));
    }
    
    // CPU_CLK_UNHALTED.THREAD
    static void enable_fixed_ctr1(void) {
        //wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR1_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) | FIXED_CTR1_ALL));
        wrmsr(GLOBAL_CTRL, (rdmsr(GLOBAL_CTRL) | FIXED_CTR1_ENABLE));
    }
    
    // CPU_CLK_UNHALTED.REF
    static void enable_fixed_ctr2(void) {
        //wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR2_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) | FIXED_CTR2_ALL));
        wrmsr(GLOBAL_CTRL, (rdmsr(GLOBAL_CTRL) | FIXED_CTR2_ENABLE));
    }
    
    static void disable_fixed_ctr0(void) {
        wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR0_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) & ~FIXED_CTR0_ALL));
    }
    
    static void disable_fixed_ctr1(void) {
        wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR1_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) & ~FIXED_CTR1_ALL));
    }
    
    static void disable_fixed_ctr2(void) {
        wrmsr(GLOBAL_OVF, (rdmsr(GLOBAL_OVF) | FIXED_CTR2_OVF)); //clear OVF flag
        wrmsr(FIXED_CTR_CTL, (rdmsr(FIXED_CTR_CTL) & ~FIXED_CTR2_ALL));
    }
  
};

// Intel core solo and Intel core duo microarchitectures PMU support
class Intel_Core_Duo_PMU : public Intel_PMU_Version2 
{
public:
    Intel_Core_Duo_PMU() { }
    
    //UMASK specific to Intel core solo and Intel core duo
    enum {
	//Core Specificity Encoding within a Non-Architectural Umask bits 15-14
	ALL_CORES 	= (0x03 << 14),
	THIS_CORE 	= (0x01 << 14),
	
	//Bit 13
	THIS_AGENT 	= (0x0 << 13),
	ALL_AGENTS 	= (0x1 << 13),
	
	//HW Prefetch Qualification Encoding within a Non-Architectural Umask bits 13-12
	ALL_INCLUSIVE 	= (0x03 << 12),
	HW_PREFETCH  	= (0x01 << 12),
	EXC_HW_PREFETCH = (0x00 << 12),
	
	//MESI Qualification Encoding within a Non-Architectural Umask bits 11-8
	M_STATE		= (0x1 << 11), //bit 11
	E_STATE		= (0x1 << 10), //bit 10
	S_STATE		= (0x1 << 9),  //bit 9
	I_STATE		= (0x1 << 8),  //bit 8
	
    };
        
    //List of all supported events
    enum {
	LD_BLOCKS 		= 0x03,
	SD_DRAINS 		= 0x04,
	MISALIGN_MEM_REF	= 0x05,
	SEG_REG_LOAS		= 0x06,
	SSE_PREFNTA_RET		= 0x07,
	SSE_PREFNTA_RET_UMASK	= (0x00 << 8),
	SSE_PREFT1_RET		= 0x07,
	SSE_PREFT1_RET_UMASK	= (0x01 << 8),
	SSE_PREFT2_RET		= 0x07,
	SSE_PREFT2_RET_UMASK	= (0x02 << 8),
	SSE_NTSTORES_RET	= 0x07,
	SSE_NTSTORES_RET_UMASK	= (0x03 << 8),
	FP_COMPS_OP_EXE		= 0x10,
	FP_ASSIST		= 0x11, //PMC1 only
	MUL			= 0x12, //PMC1 only
	DIV			= 0x13, //PMC1 only
	CYCLES_DIV_BUSY		= 0x14, //PMC0 only
	L2_ADS			= 0x21, //Requires core specificity
	DBUS_BUSY		= 0x22, //Requires core specificity
	DBUS_BUSY_RD		= 0x23, //Requires core specificity
	L2_LINES_IN		= 0x24, //Requires core specificity and HW prefetch qualification
	L2_M_LINES_IN		= 0x25, //Requires core specificity
	L2_LINES_OUT		= 0x26, //Requires core specificity and HW prefetch qualification
	L2_M_LINES_OUT		= 0x27, //Requires core specificity and HW prefetch qualification
	L2_IFETCH		= 0x28, //Requires core specificity
	L2_LD			= 0x29, //Requires core specificity
	L2_ST			= 0x2A, //Requires core specificity
	L2_RQSTS		= 0x2E, //Requires core specificity, HW prefetch qualification, and MESI qualification
	L2_REJECT_CYCLES	= 0x30, //Requires core specificity, HW prefetch qualification, and MESI qualification
	L2_NO_REQUEST_CYCLES	= 0x32, //Requires core specificity, HW prefetch qualification, and MESI qualification
	EST_TRANS_ALL		= 0x3A,
	EST_TRANS_ALL_UMASK	= (0x10 << 8), // Intel enhanced SpeedStep frequency transitions,
	THERMAL_TRIP		= 0xC0, //miss UMASK not completed
	NONHLT_REF_CYCLES	= 0x3C, 
    NONHLT_REF_CYCLES_UMASK = (0x01 << 8),
	SERIAL_EXECUTION_CYCLES = 0x3C,
	DCACHE_CACHE_LD		= 0x40, //Requires MESI qualification
	DCACHE_CACHE_ST		= 0x41, //Requires MESI qualification
	DCACHE_CACHE_LOCK	= 0x42, //Requires MESI qualification
	DATA_MEM_REF		= 0x43,
	DATA_MEM_REF_UMASK	= (0x01 << 8),
	DATA_MEM_CACHE_REF	= 0x44,
	DATA_MEM_CACHE_REF_UMASK = (0x02 << 8),
	DCACHE_REPL		    = 0x45,
	DCACHE_REPL_UMASK	= (0xF << 8),
	DCACHE_M_REPL		= 0x46,
	DCACHE_M_REPL_UMASK	= (0x00 << 8),
	DCACHE_M_EVICT		= 0x47,
	DCACHE_PEND_MISS	= 0x48, // use CMASK = 1 to count duration
	DTLB_MISS		    = 0x49,
	SSE_PREFNTA_MISS	= 0x4B,
	SSE_PREFNTA_MISS_UMASK	= (0x00 << 8),
	SSE_PREFT1_MISS		= 0x4B,
	SSE_PREFT1_MISS_UMASK	= (0x01 << 8),
	SSE_PREFT2_MISS		= 0x4B,
	SSE_PREFT2_MISS_UMASK	= (0x02 << 8),
	SSE_NTSTORES_MISS	= 0x4B,
	SSE_NTSTORES_MISS_UMASK	= (0x03 << 8),
	L1_PREF_REQ		    = 0x4F,
	BUS_REQ_OUTSTANDING	= 0x60, //Requires core specificity and agent specificity
	BUS_BNR_CLOCKS		= 0x61,
	BUS_DRDY_CLOCKS		= 0x62, //Requires core specificity and agent specificity
	BUS_LOCKS_CLOCKS	= 0x63, //Requires core specificity and agent specificity
	BUS_DATA_RCV		= 0x64,
	BUS_DATA_RCV_UMASK	= (0x40 << 8),
	BUS_TRANS_BRD		= 0x65, //Requires core specificity 
	BUS_TRANS_RFO		= 0x66, //Requires core specificity and agent specificity
	BUS_TRANS_IFETCH	= 0x68, //Requires core specificity and agent specificity
	BUS_TRANS_INVAL		= 0x69, //Requires core specificity and agent specificity
	BUS_TRANS_PWR		= 0x6A, //Requires core specificity and agent specificity
	BUS_TRANS_P		    = 0x6B, //Requires core specificity and agent specificity
	BUS_TRANS_IO		= 0x6C, //Requires core specificity and agent specificity
	BUS_TRANS_DEF		= 0x6D, //Requires core specificity 
	BUS_TRANS_WB		= 0x67, //Requires agent specificity 
	BUS_TRANS_BURST		= 0x6E, //Requires agent specificity 
	BUS_TRANS_MEM		= 0x6F, //Requires agent specificity 
	BUS_TRANS_ANY		= 0x70, //Requires agent specificity 
    BUS_TRANS_ANY_MASK  = (0xC0 << 8),
	BUS_SNOOPS		    = 0x77, //Requires MESI qualification and Agent specificity
	DCU_SNOOP_TO_SHARE	= 0x78, //Requires core specificity
	DCU_SNOOP_TO_SHARE_UMASK = (0x01 << 8),
	BUS_NOT_IN_USE		= 0x7D, //Requires core specificity
	BUS_SNOOP_STALL		= 0x7E, 
	ICACHE_READS		= 0x80,
	ICACHE_MISSES		= 0x81,
	ITLB_MISSES		    = 0x85,
	IFU_MEM_STALL		= 0x86,
	ILD_STALL		    = 0x87,
	BR_INST_EXEC		= 0x88,
	BR_MISSP_EXEC		= 0x89,
	BR_BAC_MISSP_EXEC	= 0x8A,
	BR_CND_EXEC		    = 0x8B,
	BR_CND_MISSP_EXEC	= 0x8C,
	BR_IND_EXEC		    = 0x8D,
	BR_IND_MISSP_EXEC	= 0x8E,
	BR_RET_EXEC		    = 0x8F,
	BR_RET_MISSP_EXEC	= 0x90,
	BR_RET_BAC_MISSP_EXEC	= 0x91,
	BR_CALL_EXEC		= 0x92,
	BR_CALL_MISSP_EXEC	= 0x93,
	BR_IND_CALL_EXEC	= 0x94,
	RESOURCE_STALL		= 0xA2,
	MMX_INSTR_EXEC		= 0xB0,
	SIMD_INT_SAT_EXEC	= 0xB1,
	SIMD_INT_PMUL_EXEC	= 0xB3,
	SIMD_INT_PMUL_EXEC_UMASK = (0x01 << 8),
	SIMD_INT_PSTF_EXEC	= 0xB3,
	SIMD_INT_PSTF_EXEC_UMASK = (0x02 << 8),
	SIMD_INT_PCK_EXEC	= 0xB3,
	SIMD_INT_PCK_EXEC_UMASK = (0x04 << 8),
	SIMD_INT_UPCK_EXEC	= 0xB3,
	SIMD_INT_UPCK_EXEC_UMASK = (0x08 << 8),
	SIMD_INT_PLOG_EXEC	= 0xB3,
	SIMD_INT_PLOG_EXEC_UMASK = (0x10 << 8),
	SIMD_INT_PARI_EXEC	= 0xB3,
	SIMD_INT_PARI_EXEC_UMASK = (0x20 << 8),
	INSTR_RET		= 0xC0,
	FP_COMP_INSTR_RET	= 0xC1, // PMC0 only
	UOPS_REF		= 0xC2,
	SMC_DETECTED		= 0xC3,
	BR_INSTR_RET		= 0xC4,
	BR_MISPRED_RET		= 0xC5,
	CYCLES_INT_MASKED	= 0xC6,
	CYCLES_INT_PEDNING_MASKED = 0xC7,
	HW_INT_RX		= 0xC8,
	BR_TAKEN_RET		= 0xC9,
	BR_MISPRED_TAKEN_RET	= 0xCA,
	MMX_FP_TRANS		= 0xCC,
	MMX_FP_TRANS_UMASK	= (0x00 << 8),
	FP_MMX_TRANS		= 0xCC,
	FP_MMX_TRANS_UMASK	= (0x01 << 8),
	MMX_ASSIST		= 0xCD,
	MMX_INSTR_RET		= 0xCE,
	INSTR_DECODED		= 0xD0,
	ESP_UOPS		= 0xD7,
	SIMD_FP_SP_RET		= 0xD8,
	SIMD_FP_SP_RET_UMASK	= (0x00 << 8),
	SIMD_FP_SP_S_RET	= 0xD8,
	SIMD_FP_SP_S_RET_UMASK  = (0x01 << 8),
	SIMD_FP_DP_P_RET	= 0xD8,
	SIMD_FP_DP_P_RET_UMASK	= (0x02 << 8),
	SIMD_FP_DP_S_RET	= 0xD8,
	SIMD_FP_DP_S_RET_UMASK  = (0x03 << 8),
	SIMD_INT_128_RET	= 0xD8,
	SIMD_INT_128_RET_UMASK	= (0x04 << 8),
	SIMD_FP_SP_P_COMP_RET	= 0xD9,
	SIMD_FP_SP_P_COMP_RET_UMASK = (0x00 << 8),
	SIMD_FP_SP_S_COMP_RET	= 0xD9,
	SIMD_FP_SP_S_COMP_RET_UMAS = (0x01 << 8),
	SIMD_FP_DP_P_COMP_RET	= 0xD9,
	SIMD_FP_DP_P_COMP_RET_UMASK = (0x02 << 8),
	SIMD_FP_DP_S_COMP_RET	= 0xD9,
	SIMD_FP_DP_S_COMP_RET_UMASK = (0x03 << 8),
	FUSED_UOPS_REG		= 0xDA,
	FUSED_UOPS_REG_UMASK	= (0x00 << 8),
	FUSED_LD_UOPS_RET	= 0xDA,
	FUSED_LD_UOPS_RET_UMASK	= (0x01 << 8),
	FUSED_ST_UOPS_REF	= 0xDA,
	FUSED_ST_UOPS_REF_UMASK	= (0x02 << 8),
	UNFUSION		= 0xDB,
	BR_INSTR_DECODED	= 0xE0,
	BTB_MISSES		= 0xE2,
	BR_BOGUS		= 0xE4,
	BACLEARS		= 0xE6,
	PREF_RQSTS_UP		= 0xF0,
	PREF_RQSTS_DN		= 0xF8,
    };
    
};

// Intel core microarchitecture PMU support
class Intel_Core_Micro_PMU : public Intel_PMU_Version2 
{
public:
    Intel_Core_Micro_PMU() { }
    
    //UMASK specific to Intel core microarchitecture
    enum {
    //Core Specificity Encoding within a Non-Architectural Umask bits 15-14
    ALL_CORES   = (0x03 << 14),
    //0x2 and 0x0 reserved
    THIS_CORE   = (0x01 << 14),
    
    //Agent Specificity Encoding within a Non-Architectural Umask Bit 13
    THIS_AGENT  = (0x0 << 13),
    ALL_AGENTS  = (0x1 << 13),
    
    //HW Prefetch Qualification Encoding within a Non-Architectural Umask bits 13-12
    ALL_INCLUSIVE   = (0x03 << 12),
    //0x02 reserved
    HW_PREFETCH     = (0x01 << 12),
    EXC_HW_PREFETCH = (0x00 << 12),
    
    //Bus Snoop Qualification Definitions within a Non-Architectural Umask bits 11-8
    HITM            = (0x1 << 11), //bit 11
    //bit 10 reserved
    HIT             = (0x1 << 9),  //bit 9
    CLEAN           = (0x1 << 8),  //bit 8
    
    //MESI Qualification Encoding within a Non-Architectural Umask bits 11-8
    M_STATE     = (0x1 << 11), //bit 11
    E_STATE     = (0x1 << 10), //bit 10
    S_STATE     = (0x1 << 9),  //bit 9
    I_STATE     = (0x1 << 8),  //bit 8
    
    //Snoop Type Qualification Definitions within a Non-Architectural Umask bits 9-8
    CMP2I_SNOOPS = (0x1 << 9),
    CMP2S_SNOOPS = (0x1 << 8),
    };
    
    //List of all supported events - Table A-9
    enum {
    LD_BLOCK_STA                = 0x03 | (0x02 << 8),
    LD_BLOCK_STD                = 0x03 | (0x04 << 8),
    LD_BLOCK_OVERLAP_STORE      = 0x03 | (0x08 << 8),
    LD_BLOCK_UNTIL_RETIRE       = 0x03 | (0x10 << 8),
    LD_BLOCK_L1D                = 0x03 | (0x20 << 8),
    SD_DRAIN_CYCLES             = 0x04 | (0x01 << 8),
    STORE_BLOCK_ORDER           = 0x04 | (0x02 << 8),
    STORE_BLOCK_SNOOP           = 0x04 | (0x08 << 8),
    SEG_REG_LOADS               = 0x06,
    SSE_PRE_EXEC_NTA            = 0x07,
    SSE_PRE_EXEC_L1             = 0x07 | (0x01 << 8),
    SSE_PRE_EXEC_L2             = 0x07 | (0x02 << 8),
    SSE_PRE_EXEC_STORES         = 0x07 | (0x03 << 8),
    DTLB_MISSES_ANY             = 0x08 | (0x01 << 8),
    DTLB_MISSES_LD              = 0x08 | (0x02 << 8),
    DTLB_MISSES_LO_MISS_LD      = 0x08 | (0x04 << 8),
    DTLB_MISSES_MISS_ST         = 0x08 | (0x08 << 8),
    MEMORY_DIS_RESET            = 0x09 | (0x01 << 8),
    MEMORY_DIS_SUCCESS          = 0x09 | (0x02 << 8),
    PAGE_WALKS_COUNT            = 0x0C | (0x01 << 8),
    PAGE_WALKS_CYCLES           = 0x0C | (0x02 << 8),
    FP_COMP_OPS_EXE             = 0x10, //PMC0 only
    FP_ASSIST                   = 0x11, //PMC1 only
    MUL                         = 0x12, //PMC1 only
    DIV                         = 0x13, //PMC1 only
    CYCLES_DIV_BUSY             = 0x14, //PMC0 only
    IDLE_DURING_DIV             = 0x18, //PMC0 only
    DELAYED_BYPASS_FP           = 0x19, //PMC1 only
    DELAYED_BYPASS_SIMD         = 0x19 | (0x01 << 8), //PMC1 only
    DELAYED_BYPASS_LOAD         = 0x19 | (0x02 << 8), //PMC1 only
    L2_ADS                      = 0x21, //Requires core specificity
    DBUS_BUSY_RD                = 0x23, //Requires core specificity
    L2_LINES_IN                 = 0x24, //Requires core specificity and HW prefetch qualification
    L2_M_LINES_IN               = 0x25, //Requires core specificity
    L2_LINES_OUT                = 0x26, //Requires core specificity and HW prefetch qualification
    L2_M_LINES_OUT              = 0x27, //Requires core specificity and HW prefetch qualification
    L2_IFETCH                   = 0x28, //Requires core specificity and MESI qualification
    L2_LD                       = 0x29, //Requires core specificity, HW prefetch and MESI qualification
    L2_ST                       = 0x2A, //Requires core specificity and MESI qualification
    L2_LOCK                     = 0x2B, //Requires core specificity and MESI qualification
    L2_RQSTS                    = 0x2E, //Requires core specificity, HW prefetch qualification, and MESI qualification
    L2_RQSTS_SELF_DEMAND_I_STATE = 0x2E | (0x41 << 8),
    L2_RQSTS_SELF_DEMAND_MESI   = 0x2E | (0x4F << 8),
    L2_REJECT_BUSQ              = 0x30, //Requires core specificity, HW prefetch qualification, and MESI qualification
    L2_NO_REQ                   = 0x32, //Requires core specificity
    EIST_TRANS                  = 0x3A,
    THERMAL_TRIP                = 0x3B | (0xC0 << 8),
    CPU_CLK_UNHALTED_CORE_P     = 0x3C,
    CPU_CLK_UNHALTED_BUS        = 0x3C | (0x01 << 8),
    CPU_CLK_UNHALTED_NO_OTHER   = 0x3C | (0x02 << 8),
    L1D_CACHE_LD                = 0x40, //Requires MESI qualification
    L1D_CACHE_ST                = 0x41, //Requires MESI qualification
    L1D_CACHE_LOCK              = 0x42, //Requires MESI qualification
    L1D_CACHE_LOCK_DURATION     = 0x42 | (0x10 << 8),
    L1D_ALL_REF                 = 0x43 | (0x01 << 8),
    L1D_ALL_CACHE_REF           = 0x43 | (0x02 << 8),
    L1D_REPL                    = 0x45 | (0x0F << 8),
    L1D_M_REPL                  = 0x46,
    L1D_M_EVICT                 = 0x47,
    L1D_PEND_MISS               = 0x48,
    L1D_SPLIT_LOADS             = 0x49 | (0x01 << 8),
    L1D_SPLIT_STORES            = 0x49 | (0x02 << 8),
    SSE_PRE_MISS_NTA            = 0x4B,
    SSE_PRE_MISS_L1             = 0x4B | (0x01 << 8),
    SSE_PRE_MISS_L2             = 0x4B | (0x02 << 8),    
    LOAD_HIT_PRE                = 0x4C,
    L1D_PREFETCH_REQUESTS       = 0x4E | (0x10 << 8),
    BUS_REQUEST_OUTSTANDING     = 0x60, //Requires core specificity and agent specificity
    BUS_BNR_DRV                 = 0x61, //Requires agent specificity
    BUS_DRDY_CLOCKS             = 0x62, //Requires agent specificity
    BUS_LOCK_CLOCKS             = 0x63, //Requires core specificity and agent specificity
    BUS_DATA_RCV                = 0x64, //Requires core specificity
    BUS_TRANS_BRD               = 0x65, //Requires core and agent specificity 
    BUS_TRANS_RFO               = 0x66, //Requires core specificity and agent specificity
    BUS_TRANS_WB                = 0x67, //Requires core specificity and agent specificity
    BUS_TRANS_IFETCH            = 0x68, //Requires core specificity and agent specificity
    BUS_TRANS_INVAL             = 0x69, //Requires core specificity and agent specificity
    BUS_TRANS_PWR               = 0x6A, //Requires core specificity and agent specificity
    BUS_TRANS_P                 = 0x6B, //Requires core specificity and agent specificity
    BUS_TRANS_IO                = 0x6C, //Requires core specificity and agent specificity
    BUS_TRANS_DEF               = 0x6D, //Requires core and agent specificity 
    BUS_TRANS_BURST             = 0x6E, //Requires core and agent specificity 
    BUS_TRANS_MEM               = 0x6F, //Requires core and agent specificity 
    BUS_TRANS_ANY               = 0x70, //Requires core and agent specificity 
    EXT_SNOOPS                  = 0x77, //Requires agent specificity and snoop response
    CMP_SNOOP                   = 0x78, //Requires core specificity and snoop type
    BUS_HIT_DRV                 = 0x7A, //Requires agent specificity
    BUS_HITM_DRV                = 0x7B, //Requires agent specificity
    BUSQ_EMPTY                  = 0x7D, //Requires agent specificity
    SNOOP_STALL_DRV             = 0x7E, //Requires core and agent specificity
    BUS_IO_WAIT                 = 0x7F, //Requires core specificity
    L1I_READS                   = 0x80,
    L1I_MISSES                  = 0x81,
    ITLB_SMALL_MISS             = 0x82 | (0x02 << 8),
    ITLB_LARGE_MISS             = 0x82 | (0x10 << 8),
    ITLB_FLUSH                  = 0x82 | (0x40 << 8),
    ITLB_MISSES                 = 0x82 | (0x12 << 8),   
    INST_QUEUE_FULL             = 0x83 | (0x02 << 8),
    CYCLES_L1I_MEM_STALLED      = 0x86,
    ILD_STALL                   = 0x87,
    BR_INST_EXEC                = 0x88,
    BR_MISSP_EXEC               = 0x89,
    BR_BAC_MISSP_EXEC           = 0x8A,
    BR_CND_EXEC                 = 0x8B,
    BR_CND_MISSP_EXEC           = 0x8C,
    BR_IND_EXEC                 = 0x8D,
    BR_IND_MISSP_EXEC           = 0x8E,
    BR_RET_EXEC                 = 0x8F,
    BR_RET_MISSP_EXEC           = 0x90,
    BR_RET_BAC_MISSP_EXEC       = 0x91,
    BR_CALL_EXEC                = 0x92,
    BR_CALL_MISSP_EXEC          = 0x93,
    BR_IND_CALL_EXEC            = 0x94,
    BR_TKN_BUBBLE_1             = 0x97,
    BR_TKN_BUBBLE_2             = 0x98,
    RS_UOPS_DISPATCHED          = 0xA0,
    RS_UOPS_DISPATCHED_PORT0    = 0xA1 | (0x01 << 8),
    RS_UOPS_DISPATCHED_PORT1    = 0xA1 | (0x02 << 8),
    RS_UOPS_DISPATCHED_PORT2    = 0xA1 | (0x04 << 8),
    RS_UOPS_DISPATCHED_PORT3    = 0xA1 | (0x08 << 8),
    RS_UOPS_DISPATCHED_PORT4    = 0xA1 | (0x10 << 8),
    RS_UOPS_DISPATCHED_PORT5    = 0xA1 | (0x20 << 8),
    MACRO_INSTS_DECODED         = 0xAA | (0x01 << 8),
    MACRO_INSTS_CISC_DECODED    = 0xAA | (0x08 << 8),
    ESP_SYNCH                   = 0xAB | (0x01 << 8),
    ESP_ADDITIONS               = 0xAB | (0x02 << 8),
    SIMD_UOPS_EXEC              = 0xB0,
    SIMD_SAT_UOP_EXEC           = 0xB1,
    SIMD_UOP_TYPE_EXEC_MUL      = 0xB3 | (0x01 << 8),
    SIMD_UOP_TYPE_EXEC_SHIFT    = 0xB3 | (0x02 << 8),
    SIMD_UOP_TYPE_EXEC_PACK     = 0xB3 | (0x04 << 8),
    SIMD_UOP_TYPE_EXEC_UNPACK   = 0xB3 | (0x08 << 8),
    SIMD_UOP_TYPE_EXEC_LOGICAL  = 0xB3 | (0x10 << 8),
    SIMD_UOP_TYPE_EXEC_ARITHMETIC  = 0xB3 | (0x20 << 8),
    INST_RETIRED_ANY_P          = 0xC0,
    INST_RETIRED_LOADS          = 0xC0 | (0x01 << 8),
    INST_RETIRED_STORES         = 0xC0 | (0x02 << 8),
    INST_RETIRED_OTHER          = 0xC0 | (0x04 << 8),
    X87_OPS_RETIRED_FXCH        = 0xC1 | (0x01 << 8),
    X87_OPS_RETIRED_ANY         = 0xC1 | (0xFE << 8),
    UOPS_RETIRED_LD_IND_BR      = 0xC2 | (0x01 << 8),
    UOPS_RETIRED_STD_STA        = 0xC2 | (0x02 << 8),
    UOPS_RETIRED_MACRO_FUSION   = 0xC2 | (0x04 << 8),
    UOPS_RETIRED_FUSED          = 0xC2 | (0x07 << 8),
    UOPS_RETIRED_NON_FUSED      = 0xC2 | (0x08 << 8),
    UOPS_RETIRED_ANY            = 0xC2 | (0x0F << 8),
    MACHINE_NUKES_SMC           = 0xC3 | (0x01 << 8),
    MACHINE_NUKES_MEM_ORDER     = 0xC3 | (0x04 << 8),
    BR_INST_RETIRED_ANY         = 0xC4,
    BR_INST_RETIRED_PRED_NOT_TAKEN = 0xC4 | (0x01 << 8),
    BR_INST_RETIRED_MISPRED_NOT_TAKEN = 0xC4 | (0x02 << 8),
    BR_INST_RETIRED_PRED_TAKEN  = 0xC4 | (0x04 << 8),
    BR_INST_RETIRED_MISPRED_TAKEN = 0xC4 | (0x08 << 8),
    BR_INST_RETIRED_TAKEN       = 0xC4 | (0x0C << 8),
    BR_INST_RETIRED_MISPRED     = 0xC5,
    CYCLES_INT_MASKED           = 0xC6 | (0x01 << 8),
    CYCLES_INT_PEDNING_AND_MASKED = 0xC6 | (0x02 << 8),
    SIMD_INST_RETIRED_PACKED_SINGLE = 0xC7 | (0x01 << 8),
    SIMD_INST_RETIRED_SCALAR_SINGLE = 0xC7 | (0x02 << 8),
    SIMD_INST_RETIRED_PACKED_DOUBLE = 0xC7 | (0x04 << 8),
    SIMD_INST_RETIRED_SCALAR_DOUBLE = 0xC7 | (0x08 << 8),
    SIMD_INST_RETIRED_VECTOR        = 0xC7 | (0x10 << 8),
    SIMD_INST_RETIRED_ANY           = 0xC7 | (0x1F << 8),
    HW_INT_RCV                  = 0xC8,
    ITLB_MISS_RETIRED           = 0xC9,
    SIMD_COMP_INST_RETIRED_PACKED_SINGLE = 0xCA | (0x01 << 8),
    SIMD_COMP_INST_RETIRED_SCALAR_SINGLE = 0xCA | (0x02 << 8),
    SIMD_COMP_INST_RETIRED_PACKED_DOUBLE = 0xCA | (0x04 << 8),
    SIMD_COMP_INST_RETIRED_SCALAR_DOUBLE = 0xCA | (0x08 << 8),
    MEM_LOAD_RETIRED_L1D_MISS       = 0xCB | (0x01 << 8),
    MEM_LOAD_RETIRED_L1D_LINE_MISS  = 0xCB | (0x02 << 8),
    MEM_LOAD_RETIRED_L2_MISS        = 0xCB | (0x04 << 8),
    MEM_LOAD_RETIRED_L2_LINE_MISS   = 0xCB | (0x08 << 8),
    MEM_LOAD_RETIRED_DTLB_MISS      = 0xCB | (0x10 << 8),
    FP_MMX_TRANS_TO_MMX         = 0xCC | (0x01 << 8),
    FP_MMX_TRANS_TO_FP          = 0xCC | (0x02 << 8),
    SIMD_ASSIST                 = 0xCD,
    SIMD_INSTR_RETIRED          = 0xCE,
    SIMD_SAT_INSTR_RETIRED      = 0xCF,
    RAT_STALLS_ROB_READ_PORT    = 0xD2 | (0x01 << 8),
    RAT_STALLS_PARTIAL_CYCLES   = 0xD2 | (0x02 << 8),
    RAT_STALLS_FLAGS            = 0xD2 | (0x04 << 8),
    RAT_STALLS_FPSW             = 0xD2 | (0x08 << 8),
    RAT_STALLS_ANY              = 0xD2 | (0x0F << 8),
    SEG_RENAME_STALLS_ES        = 0xD4 | (0x01 << 8),
    SEG_RENAME_STALLS_DS        = 0xD4 | (0x02 << 8),
    SEG_RENAME_STALLS_FS        = 0xD4 | (0x04 << 8),
    SEG_RENAME_STALLS_GS        = 0xD4 | (0x08 << 8),
    SEG_RENAME_STALLS_ANY       = 0xD4 | (0x0F << 8),
    SEG_REG_RENAMES_ES          = 0xD5 | (0x01 << 8),
    SEG_REG_RENAMES_DS          = 0xD5 | (0x02 << 8),
    SEG_REG_RENAMES_FS          = 0xD5 | (0x04 << 8),
    SEG_REG_RENAMES_GS          = 0xD5 | (0x08 << 8),
    SEG_REG_RENAMES_ANY         = 0xD5 | (0x0F << 8),
    RESOURCE_STALLS_ROB_FULL    = 0xDC | (0x01 << 8),
    RESOURCE_STALLS_RS_FULL     = 0xDC | (0x02 << 8),
    RESOURCE_STALLS_LD_ST       = 0xDC | (0x04 << 8),
    RESOURCE_STALLS_FPCW        = 0xDC | (0x08 << 8),
    RESOURCE_STALLS_BR_MISS_CLEAR  = 0xDC | (0x10 << 8),
    RESOURCE_STALLS_ANY         = 0xDC | (0x1F << 8),
    BR_INST_DECODED             = 0xE0,
    BR_BOGUS                    = 0xE4,
    BACLEARS                    = 0xE6,
    PREF_RQSTS_UP               = 0xF0,
    PREF_RQSTS_DN               = 0xF8,
    };
};

// Intel Atom microarchitecture
class Intel_Atom_PMU : public PMU 
{
public:
    Intel_Atom_PMU() { }
};

// Intel microarchitecture code name Nehalem and Westmere
class Intel_Nehalem_PMU : public PMU 
{
public:
    Intel_Nehalem_PMU() { }
};

// Intel microarchitecture code name Sandy Bridge
class Intel_Sandy_Bridge_PMU : public Intel_PMU_Version3
{
public:
    Intel_Sandy_Bridge_PMU() { }
    
    //Layout of IA32_PEBS_ENABLE MSR figure 18-28
    enum {
        PS_EN = (0x01LLU << 63),
        LL_EN_PMC3 = (0x01LLU << 35),
        LL_EN_PMC2 = (0x01LLU << 34),
        LL_EN_PMC1 = (0x01LLU << 33),
        LL_EN_PMC0 = (0x01LLU << 32),
        PEBS_EN_PMC3 = (0x01LLU << 3),
        PEBS_EN_PMC2 = (0x01LLU << 2),
        PEBS_EN_PMC1 = (0x01LLU << 1),
        PEBS_EN_PMC0 = 0x01LLU,
        RESET_PEBS = 0x00LLU
    };
    
    // the two uncore MSR addresses
    enum {
        OFFCORE_RSP_0 = 0x1A6,
        OFFCORE_RSP_1 = 0x1A7,
        RESET_VALUE = 0x00000000
    };
    
    // Request type fields bits for OFFCORE_RSP_x
    // bits 15:0
    enum {
        DMND_DATA_RD   = 0x0000000001LLU, // bit 0
        DMND_RFO       = 0x0000000002LLU, // bit 1
        DMND_IFETCH    = 0x0000000004LLU, // bit 2
        WB             = 0x0000000008LLU, // bit 3
        PF_DATA_RD     = 0x0000000010LLU, // bit 4
        PF_RFO         = 0x0000000020LLU, // bit 5
        PF_IFETCH      = 0x0000000040LLU, // bit 6
        PF_LLC_DATA_RD = 0x0000000080LLU, // bit 7
        PF_LLC_RFO     = 0x0000000100LLU, // bit 8
        PF_LLC_IFETCH  = 0x0000000200LLU, // bit 9
        BUS_LOCKS      = 0x0000000400LLU, // bit 10
        STRM_ST        = 0x0000000800LLU, // bit 11
        //bits 12-14 are reserved 
        OTHER          = 0x0000008000LLU // bit 15
    };
    
    // Response supplier and snoop info field bits for OFFCORE_RSP_x
    // bits 30:16
    enum {
      ANY            = 0x0000010000LLU, // bit 16 RSPNS_SUPPLIER
      NO_SUPP        = 0x0000020000LLU, // bit 17 RSPNS_SUPPLIER
      LLC_HITM       = 0x0000040000LLU, // bit 18 RSPNS_SUPPLIER
      LLC_HITE       = 0x0000080000LLU, // bit 19 RSPNS_SUPPLIER
      LLC_HITS       = 0x0000100000LLU, // bit 20 RSPNS_SUPPLIER
      LLC_HITF       = 0x0000200000LLU, // bit 21 RSPNS_SUPPLIER
      LOCAL          = 0x0000400000LLU, // bit 22 RSPNS_SUPPLIER
      // bits 23-30 are reserved RSPNS_SUPPLIER
      SNPI_NONE      = 0x0080000000LLU, // bit 31 RSPNS_SNOOP
      SNP_NOT_NEEDED = 0x0100000000LLU, // bit 32 RSPNS_SNOOP
      SNP_MISS       = 0x0200000000LLU, // bit 33 RSPNS_SNOOP
      HIT_NO_FWD     = 0x0400000000LLU, // bit 34 RSPNS_SNOOP
      HIT_FWD        = 0x0800000000LLU, // bit 35 RSPNS_SNOOP
      HITM           = 0x1000000000LLU, // bit 36 RSPNS_SNOOP
      NON_DRAM       = 0x2000000000LLU, // bit 37 RSPNS_SNOOP
    };
    
    //off-core response event enconding (table 18-24)
    enum {
        PMC0_UNCORE = 0xB7 | (0x1 << 8), //requires OFFCORE_RSP_0
        PMC3_UNCORE = 0xBB | (0x1 << 8), //requires OFFCORE_RSP_1
    };
    
    static void config_uncore(int pmc, Reg64 uncore_flags) {
        if(pmc == 0 ) {
            //wrmsr(OFFCORE_RSP_0, uncore_flags);
            wrmsr(OFFCORE_RSP_0, 0x3F803C0120LLU);
            config(PMU::EVTSEL0, PMC0_UNCORE | OS | USR | ENABLE);
        } else if(pmc == 3) {
            wrmsr(OFFCORE_RSP_1, uncore_flags);
            config(PMU::EVTSEL3, PMC3_UNCORE | OS | USR | ENABLE);
        }
    }
    
    static void enable_pebs_pmc0() {
        wrmsr(PEBS_ENABLE, rdmsr(PEBS_ENABLE) | PEBS_EN_PMC0);
    }
    
    static void enable_pebs_pmc1() {
        wrmsr(PEBS_ENABLE, rdmsr(PEBS_ENABLE) | PEBS_EN_PMC1);
    }
    
    static void enable_pebs_pmc2() {
        wrmsr(PEBS_ENABLE, rdmsr(PEBS_ENABLE) | PEBS_EN_PMC2);
    }
    
    static void enable_pebs_pmc3() {
        wrmsr(PEBS_ENABLE, rdmsr(PEBS_ENABLE) | PEBS_EN_PMC3);
    }
    
    //List of all supported events - Section 19.3 - Table 19-3
    enum {
        LD_BLOCKS_DATA_UNKNOWN = 0x03 | (0x01 << 8),
        LD_BLOCKS_STORE_FORWARD = 0x03 | (0x02 << 8),
        LD_BLOCKS_NO_SR = 0x03 | (0x8 << 8),
        LD_BLOCKS_ALL_BLOCK = 0x03 | (0x10 << 8),
        
        MISALIGN_MEM_REF_LOADS = 0x05 | (0x01 << 8),
        MISALIGN_MEM_REF_STORES = 0x05 | (0x02 << 8),
        
        LD_BLOCKS_PARTIAL_ADDRESS_ALIAS = 0x07 | (0x01 << 8),
        LD_BLOCKS_PARTIAL_ALL_STA_BLCOK = 0x07 | (0x08 << 8),
        
        DTLB_LOAD_MISSES_MISS_CAUSES_A_WALK = 0x08 | (0x01 << 8),
        DTLB_LOAD_MISSES_MISS_WALK_COMPLETED = 0x08 | (0x02 << 8),
        DTLB_LOAD_MISSES_MISS_WALK_DURATION = 0x08 | (0x04 << 8),
        DTLB_LOAD_MISSES_MISS_STLB_HIT = 0x08 | (0x10 << 8),
        
        INT_MISC_RECOVERY_CYCLES = 0x0D | (0x03 << 8), // set edge to count occurences
        INT_MISC_RAT_STALL_CYCLES = 0x0D | (0x40 << 8),
        
        UOPS_ISSUED_ANY = 0x0E | (0x01 << 8), //set cmmask = 1, inv = 1 to count stalled cycles
        
        FP_COMP_OPS_EXE_X87 = 0x10 | (0x01 << 8),
        FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE = 0x10 | (0x10 << 8),
        FP_COMP_OPS_EXE_SSE_FP_SCALAR_SINGLE = 0x10 | (0x20 << 8),
        FP_COMP_OPS_EXE_SSE_PACKED_SINGLE = 0x10 | (0x40 << 8),
        FP_COMP_OPS_EXE_SSE_SCALAR_DOUBLE = 0x10 | (0x80 << 8),
        
        SIMD_FP_256_PACKED_SINGLE = 0x11 | (0x01 << 8),
        SIMD_FP_256_PACKED_DOUBLE = 0x11 | (0x02 << 8),
        
        ARITH_FPU_DIV_ACTIVE = 0x12 | (0x01 << 8),
        
        INSTS_WRITTEN_TO_IQ_INSTS = 0x17 | (0x01 << 8),
        
        L2_RQSTS_DEMAND_DATA_RD_HIT = 0x24 | (0x01 << 8),
        L2_RQSTS_ALL_DEMAND_DATA_RD = 0x24 | (0x03 << 8),
        L2_RQSTS_RFO_HITS = 0x24 | (0x04 << 8),
        L2_RQSTS_RFO_MISS = 0x24 | (0x08 << 8),
        L2_RQSTS_ALL_RFO = 0x24 | (0x0C << 8),
        L2_RQSTS_CODE_RD_HIT = 0x24 | (0x10 << 8),
        L2_RQSTS_CODE_RD_MISS = 0x24 | (0x20 << 8),
        L2_RQSTS_ALL_CODE_RD = 0x24 | (0x30 << 8),
        L2_RQSTS_PF_HIT = 0x24 | (0x40 << 8),
        L2_RQSTS_PF_MISS = 0x24 | (0x80 << 8),
        L2_RQSTS_ALL_PF = 0x24 | (0xC0 << 8),
        
        L2_STORE_LOCK_RQSTS_MISS = 0x27 | (0x01 << 8),
        L2_STORE_LOCK_RQSTS_HIT_E = 0x27 | (0x04 << 8),
        L2_STORE_LOCK_RQSTS_HIT_M = 0x27 | (0x08 << 8),
        L2_STORE_LOCK_RQSTS_ALL = 0x27 | (0x0F << 8),
        
        L2_L1D_WB_RQSTS_HIT_E = 0x28 | (0x04 << 8),
        L2_L1D_WB_RQSTS_HIT_M = 0x28 | (0x08 << 8),
        
        LONGEST_LAT_CACHE_REFERENCE = 0x2E | (0x4F << 8), //table 19-1 architectural event
        LONGEST_LAT_CACHE_MISS = 0x2E | (0x41 << 8), //table 19-1 architectural event
        
        CPU_CLK_UNHALTED_THREAD_P = 0x3C | (0x00 << 8), //table 19-1 architectural event
        CPU_CLK_THREAD_UNHALTED_REF_XCLK = 0x3C | (0x01 << 8), //table 1901 architectural event
        
        L1D_PEND_MISS_PENDING = 0x48 | (0x01 << 8), //counter 2 only - set cmask = 1 to count cycles
        
        DTLB_STORE_MISSES_MISS_CAUSES_A_WALK = 0x49 | (0x01 << 8),
        DTLB_STORE_MISSES_WALK_COMPLETED = 0x49 | (0x02 << 8),
        DTLB_STORE_MISSES_WALK_DURATION = 0x49 | (0x04 << 8),
        DTLB_STORE_MISSES_TLB_HIT = 0x49 | (0x10 << 8),
        
        LOAD_HIT_PRE_SW_PF = 0x4C | (0x01 << 8),
        LOAD_HIT_PREHW_PF = 0x4C | (0x02 << 8),
        
        HW_PRE_REQ_DL1_MISS = 0x4E | (0x02 << 8),
        
        L1D_REPLACEMENT = 0x51 | (0x01 << 8),
        L1D_ALLOCATED_IN_M = 0x51 | (0x02 << 8),
        L1D_EVICTION = 0x51 | (0x04 << 8),
        L1D_ALL_M_REPLACEMENT = 0x51 | (0x08 << 8),
        
        PARTIAL_RAT_STALLS_FLAGS_MERGE_UOP = 0x59 | (0x20 << 8),
        PARTIAL_RAT_STALLS_SLOW_LEA_WINDOW = 0x59 | (0x40 << 8),
        PARTIAL_RAT_STALLS_MUL_SINGLE_UOP = 0x59 | (0x80 << 8),
        
        RESOURCE_STALLS2_ALL_FL_EMPTY = 0x5B | (0x0C << 8),
        RESOURCE_STALLS2_ALL_PRF_CONTROL = 0x5B | (0x0F << 8),
        RESOURCE_STALLS2_BOB_FULL = 0x5B | (0x40 << 8),
        RESOURCE_STALLS2_OOO_RSRC = 0x5B | (0x4F << 8),
        
        CPL_CYCLES_RING0 = 0x5C | (0x01 << 8), //use edge to count transition
        CPL_CYCLES_RING123 = 0x5C | (0x02 << 8),
        
        RS_EVENTS_EMPTY_CYCLES = 0x5E | (0x01 << 8),
        
        OFFCORE_REQUESTS_OUTSTANDING_DEMAND_DATA_RD = 0x60 | (0x01 << 8),
        OFFCORE_REQUESTS_OUTSTANDING_DEMAND_RFO = 0x60 | (0x04 << 8),
        OFFCORE_REQUESTS_OUTSTANDING_ALL_DATA_RD = 0x60 | (0x08 << 8),
        
        LOCK_CYCLES_SPLIT_LOCK_UC_LOCK_DURATION = 0x63 | (0x01 << 8),
        LOCK_CYCLES_CACHE_LOCK_DURATION = 0x63 | (0x02 << 8),
        
        IDQ_EMPTY = 0x79 | (0x02 << 8),
        IDQ_MITE_UOPS = 0x79 | (0x04 << 8),
        IDQ_DSB_UOPS = 0x79 | (0x08 << 8),
        IDQ_MS_DSB_UOPS = 0x79 | (0x10 << 8),
        IDQ_MS_MITE_UOPS = 0x79 | (0x20 << 8),
        IDQ_MS_UOPS = 0x79 | (0x30 << 8),
        
        ICACHE_MISSES = 0x80 | (0x02 << 8),
        
        ITLB_MISSES_MISS_CAUSES_A_WALK = 0x85 | (0x01 << 8),
        ITLB_MISSES_WALK_COMPLETED = 0x85 | (0x02 << 8),
        ITLB_MISSES_WALK_DURATION = 0x85 | (0x04 << 8),
        ITLB_MISSES_STLB_HIT = 0x85 | (0x10 << 8),
        
        ILD_STALL_LCP = 0x87 | (0x01 << 8),
        ILD_STALL_IQ_FULL = 0x87 | (0x04 << 8),
        
        BR_INST_EXEC_COND = 0x88 | (0x01 << 8),
        BR_INST_EXEC_DIRECT_JMP = 0x88 | (0x02 << 8),
        BR_INST_EXEC_INDIRECT_JMP_NON_CALL_RET = 0x88 | (0x04 << 8),
        BR_INST_EXEC_RETURN_NEAR = 0x88 | (0x08 << 8),
        BR_INST_EXEC_DIRECT_NEAR_CALL = 0x88 | (0x10 << 8),
        BR_INST_EXEC_INDIRECT_NEAR_CALL = 0x88 | (0x20 << 8),
        BR_INST_EXEC_NON_TAKEN = 0x88 | (0x40 << 8),
        BR_INST_EXEC_TAKEN = 0x88 | (0x80 << 8),
        BR_INST_EXEC_ALL_BRANCHES = 0x88 | (0xFF << 8),
        
        BR_MISP_EXEC_COND = 0x89 | (0x01 << 8),
        BR_MISP_EXEC_INDIRECT_JMP_NON_CALL_RET = 0x89 | (0x04 << 8),
        BR_MISP_EXEC_RETURN_NEAR = 0x89 | (0x08 << 8),
        BR_MISP_EXEC_DIRECT_NEAR_CALL = 0x89 | (0x10 << 8),
        BR_MISP_EXEC_INDIRECT_NEAR_CALL = 0x89 | (0x20 << 8),
        BR_MISP_EXEC_NON_TAKEN = 0x89 | (0x40 << 8),
        BR_MISP_EXEC_TAKEN = 0x89 | (0x80 << 8),
        BR_MISP_EXEC_ALL_BRANCHES = 0x89 | (0xFF << 8),
        
        IDQ_UOPS_NOT_DELIVERED_CORE = 0x9C | (0x01 << 8),
        
        UOPS_DISPATCHED_PORT_PORT_0 = 0xA1 | (0x01 << 8),
        UOPS_DISPATCHED_PORT_PORT_1 = 0xA1 | (0x02 << 8),
        UOPS_DISPATCHED_PORT_PORT_2_LD = 0xA1 | (0x04 << 8),
        UOPS_DISPATCHED_PORT_PORT_2_STA = 0xA1 | (0x08 << 8),
        UOPS_DISPATCHED_PORT_PORT_2 = 0xA1 | (0x0C << 8),
        UOPS_DISPATCHED_PORT_PORT_3_LD = 0xA1 | (0x10 << 8),
        UOPS_DISPATCHED_PORT_PORT_3_STA = 0xA1 | (0x20 << 8),
        UOPS_DISPATCHED_PORT_PORT_3 = 0xA1 | (0x30 << 8),
        UOPS_DISPATCHED_PORT_PORT_4 = 0xA1 | (0x40 << 8),
        UOPS_DISPATCHED_PORT_PORT_5 = 0xA1 | (0x80 << 8),
        
        RESOURCE_STALLS_ANY = 0xA2 | (0x01 << 8),
        RESOURCE_STALLS_LB = 0xA2 | (0x02 << 8),
        RESOURCE_STALLS_RS = 0xA2 | (0x04 << 8),
        RESOURCE_STALLS_SB = 0xA2 | (0x08 << 8),
        RESOURCE_STALLS_ROB = 0xA2 | (0x10 << 8),
        RESOURCE_STALLS_FCSW = 0xA2 | (0x20 << 8),
        RESOURCE_STALLS_MXCSR = 0xA2 | (0x40 << 8),
        RESOURCE_STALLS_OTHER = 0xA2 | (0x80 << 8),
        
        DSB2MITE_SWITCHES_COUNT = 0xAB | (0x01 << 8),
        DSB2MITE_SWITCHES_PENALTY_CYCLES = 0xAB | (0x02 << 8),
        
        DSB_FILL_OTHER_CANCEL = 0xAC | (0x02 << 8),
        DSB_FILL_EXCEED_DSB_LINES = 0xAC | (0x04 << 8),
        DSB_FILL_ALL_CANCEL = 0xAC | (0x08 << 8),
        
        ITLB_ITLB_FLUSH = 0xAE | (0x01 << 8),
        
        OFFCORE_REQUESTS_DEMAND_DATA_RD = 0xB0 | (0x01 << 8),
        OFFCORE_REQUESTS_DEMAND_RFO = 0xB0 | (0x04 << 8),
        OFFCORE_REQUESTS_ALL_DATA_RD = 0xB0 | (0x08 << 8),
        
        UOPS_DISPATCHED_THREAD = 0xB1 | (0x01 << 8),
        UOPS_DISPATCHED_CORE = 0xB1 | (0x02 << 8),
        
        OFFCORE_REQUESTS_BUFFER_SQ_FULL = 0xB2 | (0x01 << 8),
        
        AGU_BYPASS_CANCEL_COUNT = 0xB6 | (0x01 << 8),
        
        OFF_CORE_RESPONSE_0 = 0xB7 | (0x01 << 8),
        
        OFF_CORE_RESPONSE_1 = 0xBB | (0x01 << 8),
        
        TLB_FLUSH_DTLB_THREAD = 0xBD | (0x01 << 8),
        TLB_FLUSH_STLB_ANY = 0xBD | (0x20 << 8),
        
        L1D_BLOCKS_BANK_CONFLICT_CYCLES = 0xBF | (0x05 << 8),
        
        INST_RETIRED_ANY_P = 0xC0 | (0x00 << 8), //table 19-1 architectural event
        INST_RETIRED_PREC_DIST = 0xC0 | (0x01 << 8), //PMC1 only; must quiesce other PMCs
        
        OTHER_ASSISTS_ITLB_MISS_RETIRED = 0xC1 | (0x02 << 8),
        OTHER_ASSISTS_AVX_STORE = 0xC1 | (0x08 << 8),
        OTHER_ASSISTS_AVX_TO_SSE = 0xC1 | (0x10 << 8),
        OTHER_ASSISTS_SSE_TO_AVX = 0xC1 | (0x20 << 8),
        
        UOPS_RETIRED_ALL = 0xC2 | (0x01 << 8),
        UOPS_RETIRED_RETIRE_SLOTS = 0xC2 | (0x02 << 8),       
        
        MACHINE_CLEARS_MEMORY_ORDERING = 0xC3 | (0x02 << 8),
        MACHINE_CLEARS_SMC = 0xC3 | (0x04 << 8),
        MACHINE_CLEARS_MASKMOV = 0xC3 | (0x20 << 8),
        
        BR_INST_RETIRED_ALL_BRANCHES_ARCH = 0xC4 | (0x00 << 8), //table 19-1
        BR_INST_RETIRED_CONDITIONAL = 0xC4 | (0x01 << 8),
        BR_INST_RETIRED_NEAR_CALL = 0xC4 | (0x02 << 8),
        BR_INST_RETIRED_ALL_BRANCHES = 0xC4 | (0x04 << 8),
        BR_INST_RETIRED_NEAR_RETURN = 0xC4 | (0x08 << 8),
        BR_INST_RETIRED_NOT_TAKEN = 0xC4 | (0x10 << 8),
        BR_INST_RETIRED_NEAR_TAKEN = 0xC4 | (0x20 << 8),
        BR_INST_RETIRED_FAR_BRANCH = 0xC4 | (0x40 << 8),
        
        BR_MISP_RETIRED_ALL_BRANCHES_ARCH = 0xC5 | (0x00 << 8), //table 19-1
        BR_MISP_RETIRED_CONDITIONAL = 0xC5 | (0x01 << 8),
        BR_MISP_RETIRED_NEAR_CALL = 0xC5 | (0x02 << 8),
        BR_MISP_RETIRED_ALL_BRANCHES = 0xC5 | (0x04 << 8),
        BR_MISP_RETIRED_NOT_TAKEN = 0xC5 | (0x10 << 8),
        BR_MISP_RETIRED_TAKEN = 0xC5 | (0x20 << 8),
        
        FP_ASSIST_X87_OUTPUT = 0xCA | (0x02 << 8),
        FP_ASSIST_X87_INPUT = 0xCA | (0x04 << 8),
        FP_ASSIST_SIMD_OUTPUT = 0xCA | (0x08 << 8),
        FP_ASSIST_SIMD_INPUT = 0xCA | (0x10 << 8),
        FP_ASSIST_ANY = 0xCA | (0x1E << 8),
        
        ROB_MISC_EVENTS_LBR_INSERTS = 0xCC | (0x20 << 8),
        
        MEM_TRANS_RETIRED_LOAD_LATENCY = 0xCD | (0x01 << 8), //specify threshold in MSR 0x3F6
        MEM_TRANS_RETIRED_PRECISE_STORE = 0xCD | (0x02 << 8), //see section 18.8.4.3
        
        MEM_UOP_RETIRED_LOADS = 0xD0 | (0x01 << 8),
        MEM_UOP_RETIRED_STORES = 0xD0 | (0x02 << 8),
        MEM_UOP_RETIRED_STLB_MISS = 0xD0 | (0x10 << 8),
        MEM_UOP_RETIRED_LOCK = 0xD0 | (0x20 << 8),
        MEM_UOP_RETIRED_SPLIT = 0xD0 | (0x40 << 8),
        MEM_UOP_RETIRED_ALL = 0xD0 | (0x80 << 8),
        
        MEM_LOAD_UOPS_RETIRED_L1_HIT = 0xD1 | (0x01 << 8),
        MEM_LOAD_UOPS_RETIRED_L2_HIT = 0xD1 | (0x02 << 8),
        MEM_LOAD_UOPS_RETIRED_L3_HIT = 0xD1 | (0x04 << 8),
        MEM_LOAD_UOPS_RETIRED_HIT_LFB = 0xD1 | (0x40 << 8),
        
        XSNP_MISS = 0xD2 | (0x01 << 8),
        XSNP_HIT  = 0xD2 | (0x02 << 8),
        XSNP_HITM = 0xD2 | (0x04 << 8),
        XSNP_NONE = 0xD2 | (0x08 << 8), 
        
        MEM_LOAD_UOPS_MISC_RETIRED_LLC_MISS  = 0xD4 | (0x02 << 8),
        
        L2_TRANS_DEMAND_DATA_RD = 0xF0 | (0x01 << 8),
        L2_TRANS_RFO = 0xF0 | (0x02 << 8),
        L2_TRANS_CODE_RD = 0xF0 | (0x04 << 8),
        L2_TRANS_ALL_PF = 0xF0 | (0x08 << 8),
        L2_TRANS_L1D_WB = 0xF0 | (0x10 << 8),
        L2_TRANS_L2_FILL = 0xF0 | (0x20 << 8),
        L2_TRANS_L2_WB = 0xF0 | (0x40 << 8),
        L2_TRANS_ALL_REQ_UESTS = 0xF0 | (0x80 << 8),
        
        L2_LINES_IN_I = 0xF1 | (0x01 << 8),
        L2_LINES_IN_S = 0xF1 | (0x02 << 8),
        L2_LINES_IN_E = 0xF1 | (0x04 << 8),
        L2_LINES_IN_ALL = 0xF1 | (0x07 << 8),
        
        L2_LINES_OUT_DEMAND_CLEAN = 0xF2 | (0x01 << 8),
        L2_LINES_OUT_DEMAND_DIRTY = 0xF2 | (0x02 << 8),
        L2_LINES_OUT_DEMAND_PF_CLEAN = 0xF2 | (0x04 << 8),
        L2_LINES_OUT_DEMAND_PF_DIRTY = 0xF2 | (0x08 << 8),
        L2_LINES_OUT_DEMAND_DIRTY_ALL = 0xF2 | (0x0A << 8),
        
        SQ_MISC_SPLIT_LOCK = 0xF4 | (010 << 8)
    };
};

// Intel Netburst microarchitecture
class Intel_Netburst_PMU : public PMU 
{
public:
    Intel_Netburst_PMU() { }
};


__END_SYS

#endif
