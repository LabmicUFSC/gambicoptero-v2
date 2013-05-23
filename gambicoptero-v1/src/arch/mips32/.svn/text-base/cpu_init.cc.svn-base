// EPOS MIPS32 CPU Mediator Initialization

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

void MIPS32::init()
{
    db<Init, MIPS32>(TRC) << "MIPS32::init()\n";

    //This is a temporary place for this information, since it was 
    // machine dependable and should not be included in the arch_init.
    System_Info<Machine> * si = System::info();
    //Boot map
    si->bm.mem_base           = 0x10000010;	// Memory base address
    si->bm.mem_top            = 0x10100000;	// Memory top address
    si->bm.io_mem_base        = 0x20000000;	// I/O Memory base address
    si->bm.io_mem_top         = 0x20010000;	// I/O Memory top address
    //Physical memory
    si->pmm.mem_base    = Memory_Map<Machine>::BASE;
    si->pmm.mem_top     = Memory_Map<Machine>::TOP;
    si->pmm.io_mem_base = Memory_Map<Machine>::IO_MEM;
    si->pmm.io_mem_top  = Memory_Map<Machine>::IO_MEM + 0x00010000;
    si->pmm.int_vec     = Memory_Map<Machine>::INT_VEC;
    si->pmm.sys_code    = Memory_Map<Machine>::SYS_CODE;
    si->pmm.sys_data    = Memory_Map<Machine>::SYS_DATA;
    si->pmm.sys_stack   = Memory_Map<Machine>::SYS_STACK;
    si->pmm.free_base   = 0x100AA000; //0x10020000; //////////(unsigned)&__bss_end;
    si->pmm.free_top    = Memory_Map<Machine>::TOP - Traits<Machine>::SYSTEM_STACK_SIZE;
    si->bm.extras_offset = -1;//?????
    si->lm.has_sys = 0;

    // Initialize the MMU
    if(Traits<MIPS32_MMU>::enabled)
	MIPS32_MMU::init();
    else
	db<Init, MIPS32>(WRN) << "MMU is disabled!\n";
}

__END_SYS
