// EPOS PC SETUP

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/elf.h>
#include <utility/string.h>
#include <utility/ostream.h>
#include <utility/debug.h>
#include <machine.h>

// Interrupt Handler Prototypes
extern "C" { 
    void __critical_int(); 
    void __noncritical_int();
    void __machine_check();
    void __debug_int();
    void __watchdog_int();
    void __insttblmiss_int();
    void __inststor_int();
    void __program_int();
    void __syscall_int();
    void __datatblmiss_int();
    void __datastor_int();
    void __alignment_int();
    void __timer_wrapper1();
    void __timer_wrapper2();
    void __fit_int();
    void __pit_int();
};

__BEGIN_SYS

OStream kout, kerr;

class ML310_Setup
{
private:
    static const unsigned int INT_BASE = Traits<ML310>::INT_BASE;
    static const unsigned int BOOT_IMAGE_ADDR = Traits<ML310>::BOOT_IMAGE_ADDR;
    static const unsigned int SYS_STACK_SIZE = Traits<ML310>::SYSTEM_STACK_SIZE;

    static const unsigned int SYS_INFO = Memory_Map<ML310>::SYS_INFO;
    static const unsigned int PHY_MEM = Memory_Map<ML310>::PHY_MEM;
    static const unsigned int IO_MEM = Memory_Map<ML310>::IO_MEM;
    static const unsigned int INT_VEC = Memory_Map<ML310>::INT_VEC;
    static const unsigned int SYS = Memory_Map<ML310>::SYS;
    static const unsigned int SYS_DATA = Memory_Map<ML310>::SYS_DATA;
    static const unsigned int SYS_CODE = Memory_Map<ML310>::SYS_CODE;
    static const unsigned int SYS_STACK = Memory_Map<ML310>::SYS_STACK;
    
    // PPC32 Imports
    typedef CPU::Reg32 Reg32;
    typedef CPU::Phy_Addr Phy_Addr;
    typedef CPU::Log_Addr Log_Addr;

    // System_Info Imports
    typedef System_Info<ML310>::Boot_Map BM;
    typedef System_Info<ML310>::Physical_Memory_Map PMM;
    typedef System_Info<ML310>::Logical_Memory_Map LMM;
    typedef System_Info<ML310>::Load_Map LM;

public:
    ML310_Setup(char * boot_image) {
	*((volatile unsigned int *)(Traits<Machine>::LEDS_BASEADDR)) = 0xFFFFFFF8;

	// Get boot image loaded by the bootstrap
 	bi = reinterpret_cast<char *>(boot_image);
	si = reinterpret_cast<System_Info<ML310> *>(bi);

 	db<Setup>(TRC) << "ML310_Setup(bi=" << (void *)bi
 		       << ",sp=" << (void *)CPU::sp() << ")\n";

	// Disable hardware interrupts
	IC::init();
  	CPU::int_disable();

	build_lm();
	build_pmm();
	build_lmm();
	get_node_id();

	say_hi();

	setup_int();
	setup_pci();

	load_parts();

	// SETUP ends here, transfer control to next stage (INIT or APP)
	call_next();

	// SETUP is now part of the free memory and this point should never be
	// reached, but, just in case ... :-)
	panic();
    }

private:
    void build_lm();
    void build_pmm();
    void build_lmm();
    void get_node_id();

    void say_hi();

    void setup_int();
    void setup_pci();

    void load_parts();
    void call_next();

    void pci_aperture(unsigned int * base, unsigned int * top);

    static void panic() { Machine::panic(); }

private:
    char * bi;
    System_Info<ML310> * si;
};

//========================================================================
void ML310_Setup::build_lm()
{
    // Get boot image structure
    si->lm.has_stp = (si->bm.setup_offset != -1);
    si->lm.has_ini = (si->bm.init_offset != -1);
    si->lm.has_sys = (si->bm.system_offset != -1);
    si->lm.has_app = (si->bm.application_offset != -1);
    si->lm.has_ext = (si->bm.extras_offset != -1);

    // Check SETUP integrity and get the size of its segments
    si->lm.stp_entry = 0;
    si->lm.stp_code = 0;
    si->lm.stp_code_size = 0;
    si->lm.stp_data = 0;
    si->lm.stp_data_size = 0;
    if(si->lm.has_stp) {
	ELF * stp_elf = reinterpret_cast<ELF *>(&bi[si->bm.setup_offset]);
	if(!stp_elf->valid()) {
	    db<Setup>(ERR) << "SETUP ELF image is corrupted!\n";
	    panic();
	}

	si->lm.stp_entry = stp_elf->entry();
	si->lm.stp_code = stp_elf->segment_address(0);
	si->lm.stp_code_size = stp_elf->segment_size(0);
	if(stp_elf->segments() > 1) {
	    si->lm.stp_data = stp_elf->segment_address(1);
	    for(int i = 1; i < stp_elf->segments(); i++) {
		if(stp_elf->segment_address(i) < si->lm.stp_data)
		    si->lm.stp_data = stp_elf->segment_address(i);
		si->lm.stp_data_size += stp_elf->segment_size(i);
	    }
	}
    }

    // Check INIT integrity and get the size of its segments
    si->lm.ini_entry = 0;
    si->lm.ini_segments = 0;
    si->lm.ini_code = 0;
    si->lm.ini_code_size = 0;
    si->lm.ini_data = 0;
    si->lm.ini_data_size = 0;
    if(si->lm.has_ini) {
	ELF * ini_elf = reinterpret_cast<ELF *>(&bi[si->bm.init_offset]);
	if(!ini_elf->valid()) {
	    db<Setup>(ERR) << "INIT ELF image is corrupted!\n";
	    panic();
	}

	si->lm.ini_entry = ini_elf->entry();
	si->lm.ini_code = ini_elf->segment_address(0);
	si->lm.ini_code_size = ini_elf->segment_size(0);
	if(ini_elf->segments() > 1) {
	    si->lm.ini_data = ini_elf->segment_address(1);
	    for(int i = 1; i < ini_elf->segments(); i++) {
		if(ini_elf->segment_address(i) < si->lm.ini_data)
		    si->lm.ini_data = ini_elf->segment_address(i);
		si->lm.ini_data_size += ini_elf->segment_size(i);
	    }
	}
    }

    // Check SYSTEM integrity and get the size of its segments
    si->lm.sys_entry = 0;
    si->lm.sys_segments = 0;
    si->lm.sys_code = 0;
    si->lm.sys_code_size = 0;
    si->lm.sys_data = 0;
    si->lm.sys_data_size = 0;
    si->lm.sys_stack = SYS_STACK;
    si->lm.sys_stack_size = SYS_STACK_SIZE;
    if(si->lm.has_sys) {
	ELF * sys_elf = reinterpret_cast<ELF *>(&bi[si->bm.system_offset]);
	if(!sys_elf->valid()) {
	    db<Setup>(ERR) << "OS ELF image is corrupted!\n";
	    panic();
	}

	si->lm.sys_entry = sys_elf->entry();
	si->lm.sys_code = sys_elf->segment_address(0);
	si->lm.sys_code_size = sys_elf->segment_size(0);
	if(sys_elf->segments() > 1) {
	    si->lm.sys_data = sys_elf->segment_address(1);
	    for(int i = 1; i < sys_elf->segments(); i++) {
		if(sys_elf->segment_address(i) < si->lm.sys_data)
		    si->lm.sys_data = sys_elf->segment_address(i);
		si->lm.sys_data_size += sys_elf->segment_size(i);
	    }
	}

	if(si->lm.sys_code != SYS_CODE) {
	    db<Setup>(ERR) << "OS code segment address do not match "
			   << "the machine's memory map!\n";
	    panic();
	}
	if(si->lm.sys_code + si->lm.sys_code_size > si->lm.sys_data) {
	    db<Setup>(ERR) << "OS code segment is too large!\n";
	    panic();
	}
	if(si->lm.sys_data != SYS_DATA) {
	    db<Setup>(ERR) << "OS code segment address do not match "
			   << "the machine's memory map!\n";
	    panic();
	}
	if(si->lm.sys_data + si->lm.sys_data_size > si->lm.sys_stack) {
	    db<Setup>(ERR) << "OS data segment is too large!\n";
	    panic();
	}
	if(si->lm.sys_data + si->lm.sys_data_size > si->lm.sys_stack) {
	    db<Setup>(ERR) << "OS data segment is too large!\n";
	    panic();
	}
	if((si->lm.sys_stack + si->lm.sys_stack_size) > si->pmm.mem_top) {
	    db<Setup>(ERR) << "OS stack segment is too large!\n";
	    panic();
	}
    }

    // Check APPLICATION integrity and get the size of its segments
    si->lm.app_entry = 0;
    si->lm.app_segments = 0;
    si->lm.app_code = 0;
    si->lm.app_code_size = 0;
    si->lm.app_data = 0;
    si->lm.app_data_size = 0;
    if(si->lm.has_app) {
	ELF * app_elf =
	    reinterpret_cast<ELF *>(&bi[si->bm.application_offset]);
	if(!app_elf->valid()) {
	    db<Setup>(ERR) << "Application ELF image is corrupted!\n";
	    panic();
	}
	si->lm.app_entry = app_elf->entry();
	si->lm.app_code = app_elf->segment_address(0);
	si->lm.app_code_size = app_elf->segment_size(0);
	if(app_elf->segments() > 1) {
	    si->lm.app_data = app_elf->segment_address(1);
	    si->lm.app_data_size = 0;
	    for(int i = 1; i < app_elf->segments(); i++) {
		if(app_elf->segment_address(i) < si->lm.app_data)
		    si->lm.app_data = app_elf->segment_address(i);
		si->lm.app_data_size += app_elf->segment_size(i);
	    }
	}
    }

    // Check for EXTRA data in the boot image		
    if(si->lm.has_ext) {
	si->lm.ext = Phy_Addr(&bi[si->bm.extras_offset]);
	si->lm.ext_size = si->bm.img_size - si->bm.extras_offset;
    }
}

//========================================================================
void ML310_Setup::build_pmm()
{
    // Allocate (reserve) memory for all entities we have to setup.
    // We'll start at the highest address to make possible a memory model
    // on which the application's logical and physical address spaces match.

    // Interrupt Vector
    si->pmm.int_vec = INT_VEC;

    // System Info
    si->pmm.sys_info = SYS_INFO;

    // SYSTEM code segment
    si->pmm.sys_code = SYS_CODE;

    // SYSTEM data segment
    si->pmm.sys_data = SYS_DATA;

    // SYSTEM stack segment
    si->pmm.sys_stack = SYS_STACK;

    // The memory allocated so far will "disapear" from the system as we
    // set mem_top as follows:
    si->pmm.mem_base = si->bm.mem_base;
    si->pmm.mem_top = INT_VEC;

    // Free chuncks (passed to MMU::init)
    si->pmm.free1_base = si->lm.app_code + si->lm.app_code_size;
    si->pmm.free1_top = si->lm.app_data;
    si->pmm.free2_base = si->lm.app_data + si->lm.app_data_size;
    si->pmm.free2_top = si->pmm.mem_top;

    if(si->lm.has_ext) {
	si->pmm.ext_base = si->lm.ext;
	si->pmm.ext_top = si->lm.ext + si->lm.ext_size;
    } else {
	si->pmm.ext_base = 0;
	si->pmm.ext_top = 0;
    }	
    db<Setup>(TRC) << "setup_sys_pt(pmm={"
		   << "int="  << (void *)si->pmm.int_vec
		   << ",info=" << (void *)si->pmm.sys_info
		   << ",sysc=" << (void *)si->pmm.sys_code
		   << ",sysd=" << (void *)si->pmm.sys_data
		   << ",syss=" << (void *)si->pmm.sys_stack
		   << ",memb=" << (void *)si->pmm.mem_base
		   << ",memt=" << (void *)si->pmm.mem_top
		   << ",fr1b=" << (void *)si->pmm.free1_base
		   << ",fr1t=" << (void *)si->pmm.free1_top
		   << ",fr2b=" << (void *)si->pmm.free2_base
		   << ",fr2t=" << (void *)si->pmm.free2_top
		   << "}"
		   << ",code_size=" << MMU::pages(si->lm.sys_code_size)
		   << ",data_size=" << MMU::pages(si->lm.sys_data_size)
		   << ",stack_size=" << MMU::pages(si->lm.sys_stack_size)
		   << ")\n";
}

//========================================================================
void ML310_Setup::build_lmm()
{
    si->lmm.app_entry = si->lm.app_entry;
}

//========================================================================
void ML310_Setup::get_node_id()
{
    // If we didn't get our node's id in the boot image, we'll to try to
    // get if from an eventual BOOPT reply used to boot up the system before
    // we allocate more memory
    // if(si->bm.host_id == (unsigned short) -1)
    // get_bootp_info(&si->bm.host_id);
}

//========================================================================
void ML310_Setup::say_hi()
{
    if(!si->lm.has_app) {
	db<Setup>(ERR)
	    << "No APPLICATION in boot image, you don't need EPOS!\n";
	panic();
    }
    if(!si->lm.has_sys)
	db<Setup>(WRN) 
	    << "No SYSTEM in boot image, assuming EPOS is a library!\n";

    kout << "Setting up this machine as follows: \n";
    kout << "  Processor:    PPC32\n";
    kout << "  Memory:       " << (si->bm.mem_top - si->bm.mem_base) / 1024
	 << " Kbytes [" << (void *)si->bm.mem_base
	 << ":" << (void *)si->bm.mem_top << "]\n";
    kout << "  User memory:  "
	 << (si->pmm.mem_top - si->pmm.mem_base) / 1024
	 << " Kbytes [" << (void *)si->pmm.mem_base
	 << ":" << (void *)si->pmm.mem_top << "]\n";
    kout << "  PCI aperture: " 
	 << (si->pmm.io_mem_top - si->pmm.io_mem_base) / 1024
	 << " Kbytes [" << (void *)si->pmm.io_mem_base 
	 << ":" << (void *)si->pmm.io_mem_top << "]\n";
    kout << "  Node Id:      ";
    if(si->bm.node_id != -1)
	kout << si->bm.node_id << " (" << si->bm.n_nodes << ")\n";
    else
	kout << "will get from the network!\n";
    if(si->lm.has_stp)
	kout << "  Setup:        "
	     << si->lm.stp_code_size + si->lm.stp_data_size << " bytes\n";
    if(si->lm.has_ini)
	kout << "  Init:         " 
	     << si->lm.ini_code_size + si->lm.ini_data_size << " bytes\n";
    if(si->lm.has_sys) {
	kout << "  OS code:      " << si->lm.sys_code_size << " bytes";
	kout << "\tdata: " << si->lm.sys_data_size << " bytes";
	kout << "\tstack: " << si->lm.sys_stack_size << " bytes\n";
    }
    if(si->lm.has_app) {
	kout << "  APP code:     " << si->lm.app_code_size << " bytes";
	kout << "\tdata: " << si->lm.app_data_size << " bytes\n";
    }
    if(si->lm.has_ext) {
	kout << "  Extras:       " << si->lm.ext_size << " bytes\n";
    }

    // Test if we didn't overlap SETUP and the boot image
    if(si->pmm.mem_top
       <= si->lm.stp_code + si->lm.stp_code_size + si->lm.stp_data_size) {
  	db<Setup>(ERR) << "SETUP would have been overwritten!\n";
  	panic();
    }
}

//========================================================================
void ML310_Setup::setup_int()
{
    db<Setup>(TRC) << "setup_int()\n";

   //Clear interrupt structures area
   memset((void *)INT_VEC, 0, 0x6400);

   //Copy handlers routine to apropriate location.
   unsigned int int_addr;
   int_addr = INT_VEC + 0x0100;//Critical Interrupt
   memcpy((void *)int_addr, (void *)__critical_int, 4);
   int_addr = INT_VEC + 0x0200;//Machine Check
   memcpy((void *)int_addr, (void *)__machine_check, 148);
   int_addr = INT_VEC + 0x0300;//Data Storage
   memcpy((void *)int_addr, (void *)__datastor_int, 152);
   int_addr = INT_VEC + 0x0400;//Instruction
   memcpy((void *)int_addr, (void *)__inststor_int, 152);
   int_addr = INT_VEC + 0x0500;//External
   memcpy((void *)int_addr, (void *)__noncritical_int, 232);
   int_addr = INT_VEC + 0x0600;//Alignment
   memcpy((void *)int_addr, (void *)__alignment_int, 152);
   int_addr = INT_VEC + 0x0700;//Program
   memcpy((void *)int_addr, (void *)__program_int, 152);
   int_addr = INT_VEC + 0x0C00;//System Call
   memcpy((void *)int_addr, (void *)__syscall_int, 152);
   int_addr = INT_VEC + 0x1000;//PIT
   memcpy((void *)int_addr, (void *)__timer_wrapper1, 4);
   int_addr = INT_VEC + 0x1010;//FIT
   memcpy((void *)int_addr, (void *)__timer_wrapper2, 4);
   int_addr = INT_VEC + 0x1020;//WatchDog Timer
   memcpy((void *)int_addr, (void *)__watchdog_int, 152);
   int_addr = INT_VEC + 0x1100;//Data TBL Miss
   memcpy((void *)int_addr, (void *)__datatblmiss_int, 152);
   int_addr = INT_VEC + 0x1200;//Instruction TBL Miss
   memcpy((void *)int_addr, (void *)__insttblmiss_int, 152);
   int_addr = INT_VEC + 0x2000;//Debug
   memcpy((void *)int_addr, (void *)__debug_int, 152);
   int_addr = INT_VEC + 0x2100;//Debug
   memcpy((void *)int_addr, (void *)__pit_int, 220);
   int_addr = INT_VEC + 0x2210;//Debug
   memcpy((void *)int_addr, (void *)__fit_int, 152);

   //Set Vector Configuration Address and EVPR
   ASMV("mtspr %1, %0" : : "r"(INT_VEC), "i"(0x3D6));
   //Invalidate all instruction cache and sync.
   ASMV("iccci 0, 1; sync;");//Not portable code!???
}

//========================================================================
void ML310_Setup::setup_pci()
{
    db<Setup>(TRC) << "setup_pci()\n";

    // Set PCI Memory Region to work on little endian Mode
    // Set SLER
    CPU::_mtspr(CPU::SLER, 0x0F000000);
    // Set SGR
    CPU::_mtspr(CPU::SGR, 0x0F000000);
}

//========================================================================
void ML310_Setup::load_parts()
{
    // Relocate System_Info
    memcpy(reinterpret_cast<void *>(SYS_INFO), si, sizeof(System_Info<ML310>));

    // Load INIT
    if(si->lm.has_ini) {
	db<Setup>(TRC) << "ML310_Setup::load_init()\n";
	ELF * ini_elf = reinterpret_cast<ELF *>(&bi[si->bm.init_offset]);
	if(ini_elf->load_segment(0) < 0) {
	    db<Setup>(ERR)
		<< "INIT code+data segment was corrupted during SETUP!\n";
	    panic();
	}
    }

    // Load SYSTEM
    if(si->lm.has_sys) {
	db<Setup>(TRC) << "ML310_Setup::load_sys()\n";
	ELF * sys_elf = reinterpret_cast<ELF *>(&bi[si->bm.system_offset]);
	if(sys_elf->load_segment(0) < 0) {
	    db<Setup>(ERR)
		<< "OS code segment was corrupted during SETUP!\n";
	    panic();
	}
	for(int i = 1; i < sys_elf->segments(); i++)
	    if(sys_elf->load_segment(i) < 0) {
		db<Setup>(ERR)
		    << "OS data segment was corrupted during SETUP!\n";
		panic();
	    }
    }

    // Load APP
    if(si->lm.has_app) {
	ELF * app_elf = reinterpret_cast<ELF *>(&bi[si->bm.application_offset]);
	db<Setup>(TRC) << "ML310_Setup::load_app()\n";
	if(app_elf->load_segment(0) < 0) {
	    db<Setup>(ERR)
		<< "Application code segment was corrupted during SETUP!\n";
	    panic();
	}
	for(int i = 1; i < app_elf->segments(); i++)
	    if(app_elf->load_segment(i) < 0) {
		db<Setup>(ERR) << 
		    "Application data segment was corrupted during SETUP!\n";
		panic();
	    }
    }
}

//========================================================================
void ML310_Setup::call_next()
{
    // Check for next stage
    Log_Addr ip;
    if(si->lm.has_ini) {
	db<Setup>(TRC) << "Executing system global constructors ...\n";
	reinterpret_cast<void (*)()>((void *)si->lm.sys_entry)();
	ip = si->lm.ini_entry;
    } else if(si->lm.has_sys)
	ip = si->lm.sys_entry;
    else
	ip = si->lm.app_entry;

    db<Setup>(TRC) << "ML310_Setup::call_next(ip=" << ip
		   << ",sp=" << (void *)(SYS_STACK + SYS_STACK_SIZE
					 - 2 * sizeof(int))
		   << ") => ";
    if(si->lm.has_ini)
	db<Setup>(TRC) << "INIT\n";
    else if(si->lm.has_sys)
	db<Setup>(TRC) << "SYSTEM\n";
    else
	db<Setup>(TRC) << "APPLICATION\n";

    db<Setup>(INF) << "ML310_Setup ends here!\n\n";

    ASMV("mr    1,%0\n" //setting sys_stack
         "mtctr %1  \n" //setting jump address
         "bctrl     \n" : : "r"(SYS_STACK),
                            "r"((unsigned)ip));
}

__END_SYS

//========================================================================
// SETUP of BASIC interrupt handlers
//
// Desc: This functions loads the apropriated handlers address and call it.
// Since PowerPC has two class of interrupts, the returning  command (differently
// for each class is insued here, it means that the Machine::handler() could
// be the same for critical or non-critical interrupts.
//------------------------------------------------------------------------

__USING_SYS

void __machine_check() {

   //Stack Frame Create: We allocate on stack all CPU Context, less PC and MSR since
   //they are already handle in Exception hardware (They must be handled here if is
   //enabled cascate exceptions - Maybe a conf.Feature ? -Think better latter...)
   ASMV("stwu  %r1, -152(%r1)     \n" //Create the frame
        "stmw  %r0,   24(%r1)     \n" //Save r0 - r31
        "mflr  %r3                \n" //Get Link Register
        "mfctr %r4                \n" //Get Counter Register
        "mfxer %r5                \n" //Get Fixed Point Exception Register
        "mfcr  %r6                \n" //Get Conditional Register
        "stw   %r3,  156(%r1)     \n" //Save Link Register
        "stw   %r4,   12(%r1)     \n" //Save Counter Register
        "stw   %r5,   16(%r1)     \n" //Save Fixed Point Exception Register
        "stw   %r6,   20(%r1)     \n" //Save Conditional Register
       );

   // Load the Address of Interruption from the Vector Table
   ASMV("lwz   0, 0(%0)           \n" //Load Address from Exception Vector
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_MACHINE * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_MACHINE)); //Set Handler Parameter

   // Call the Interrupt Handler, Handle unexpected interrupts, restore the 
   // context of CPU after exception and return from interrupt.
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz   %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz   %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz   %r4,   12(%r1)     \n" //Load Counter Register
        "lwz   %r3,  156(%r1)     \n" //Load Link Register
        "mtcr  %r6                \n" //Put Conditional Register
        "mtxer %r5                  \n" //Put Fixed Point Register
        "mtctr %r4                  \n" //Put Counter Register
        "mtlr  %r3                  \n" //Put Link Register
        "lwz   %r0,   24(%r1)     \n" //Restore r0
        "lmw   %r2,   32(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 152          \n" //Restore Stack Pointer (r1) 
        "rfci                     \n" //Return from interrupt
        );
}
void __debug_int() {
   ASMV("stwu  %r1, -152(%r1)     \n" //Create the frame
        "stmw  %r0,   24(%r1)     \n" //Save r0 - r31
        "mflr  %r3                \n" //Get Link Register
        "mfctr %r4                \n" //Get Counter Register
        "mfxer %r5                \n" //Get Fixed Point Exception Register
        "mfcr  %r6                \n" //Get Conditional Register
        "stw   %r3,  156(%r1)     \n" //Save Link Register
        "stw   %r4,   12(%r1)     \n" //Save Counter Register
        "stw   %r5,   16(%r1)     \n" //Save Fixed Point Exception Register
        "stw   %r6,   20(%r1)     \n" //Save Conditional Register
       );
   ASMV("lwz   0, 0(%0)            \n"
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_DEBUG * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_DEBUG)); //Set Handler Parameter
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz   %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz   %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz   %r4,   12(%r1)     \n" //Load Counter Register
        "lwz   %r3,  156(%r1)     \n" //Load Link Register
        "mtcr  %r6                \n" //Put Conditional Register
        "mtxer %r5                  \n" //Put Fixed Point Register
        "mtctr %r4                  \n" //Put Counter Register
        "mtlr  %r3                  \n" //Put Link Register
        "lwz   %r0,   24(%r1)     \n" //Restore r0
        "lmw   %r2,   32(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 152          \n" //Restore Stack Pointer (r1) 
        "rfci                     \n" //Return from interrupt
        );
}
void __watchdog_int() {
   ASMV("stwu  %r1, -152(%r1)     \n" //Create the frame
        "stmw  %r0,   24(%r1)     \n" //Save r0 - r31
        "mflr  %r3                \n" //Get Link Register
        "mfctr %r4                \n" //Get Counter Register
        "mfxer %r5                \n" //Get Fixed Point Exception Register
        "mfcr  %r6                \n" //Get Conditional Register
        "stw   %r3,  156(%r1)     \n" //Save Link Register
        "stw   %r4,   12(%r1)     \n" //Save Counter Register
        "stw   %r5,   16(%r1)     \n" //Save Fixed Point Exception Register
        "stw   %r6,   20(%r1)     \n" //Save Conditional Register
       );
   ASMV("lwz   0, 0(%0)           \n" //Load Address from Exception Vector
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_WATCHDOG * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_WATCHDOG)); //Set Handler Parameter
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz   %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz   %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz   %r4,   12(%r1)     \n" //Load Counter Register
        "lwz   %r3,  156(%r1)     \n" //Load Link Register
        "mtcr  %r6                \n" //Put Conditional Register
        "mtxer %r5                  \n" //Put Fixed Point Register
        "mtctr %r4                  \n" //Put Counter Register
        "mtlr  %r3                  \n" //Put Link Register
        "lwz   %r0,   24(%r1)     \n" //Restore r0
        "lmw   %r2,   32(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 152          \n" //Restore Stack Pointer (r1) 
        "rfci                     \n" //Return from interrupt
        );
}
void __insttblmiss_int() {
   ASMV("stwu  %r1, -152(%r1)     \n" //Create the frame
        "stmw  %r0,   24(%r1)     \n" //Save r0 - r31
        "mflr  %r3                \n" //Get Link Register
        "mfctr %r4                \n" //Get Counter Register
        "mfxer %r5                \n" //Get Fixed Point Exception Register
        "mfcr  %r6                \n" //Get Conditional Register
        "stw   %r3,  156(%r1)     \n" //Save Link Register
        "stw   %r4,   12(%r1)     \n" //Save Counter Register
        "stw   %r5,   16(%r1)     \n" //Save Fixed Point Exception Register
        "stw   %r6,   20(%r1)     \n" //Save Conditional Register
       );
   ASMV("lwz   0, 0(%0)           \n" //Load Address from Exception Vector
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_INSTTBLMISS * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_INSTTBLMISS)); //Set Handler Parameter
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz   %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz   %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz   %r4,   12(%r1)     \n" //Load Counter Register
        "lwz   %r3,  156(%r1)     \n" //Load Link Register
        "mtcr  %r6                \n" //Put Conditional Register
        "mtxer %r5                  \n" //Put Fixed Point Register
        "mtctr %r4                  \n" //Put Counter Register
        "mtlr  %r3                  \n" //Put Link Register
        "lwz   %r0,   24(%r1)     \n" //Restore r0
        "lmw   %r2,   32(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 152          \n" //Restore Stack Pointer (r1) 
        "rfi                      \n" //Return from interrupt
        );
}
void __inststor_int() {
   ASMV("stwu  %r1, -152(%r1)     \n" //Create the frame
        "stmw  %r0,   24(%r1)     \n" //Save r0 - r31
        "mflr  %r3                \n" //Get Link Register
        "mfctr %r4                \n" //Get Counter Register
        "mfxer %r5                \n" //Get Fixed Point Exception Register
        "mfcr  %r6                \n" //Get Conditional Register
        "stw   %r3,  156(%r1)     \n" //Save Link Register
        "stw   %r4,   12(%r1)     \n" //Save Counter Register
        "stw   %r5,   16(%r1)     \n" //Save Fixed Point Exception Register
        "stw   %r6,   20(%r1)     \n" //Save Conditional Register
       );
   ASMV("lwz   0, 0(%0)           \n" //Load Address from Exception Vector
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_INSTSTORE * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_INSTSTORE)); //Set Handler Parameter
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz   %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz   %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz   %r4,   12(%r1)     \n" //Load Counter Register
        "lwz   %r3,  156(%r1)     \n" //Load Link Register
        "mtcr  %r6                \n" //Put Conditional Register
        "mtxer %r5                  \n" //Put Fixed Point Register
        "mtctr %r4                  \n" //Put Counter Register
        "mtlr  %r3                  \n" //Put Link Register
        "lwz   %r0,   24(%r1)     \n" //Restore r0
        "lmw   %r2,   32(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 152          \n" //Restore Stack Pointer (r1) 
        "rfi                      \n" //Return from interrupt
        );
}
void __program_int() {
   ASMV("stwu  %r1, -152(%r1)     \n" //Create the frame
        "stmw  %r0,   24(%r1)     \n" //Save r0 - r31
        "mflr  %r3                \n" //Get Link Register
        "mfctr %r4                \n" //Get Counter Register
        "mfxer %r5                \n" //Get Fixed Point Exception Register
        "mfcr  %r6                \n" //Get Conditional Register
        "stw   %r3,  156(%r1)     \n" //Save Link Register
        "stw   %r4,   12(%r1)     \n" //Save Counter Register
        "stw   %r5,   16(%r1)     \n" //Save Fixed Point Exception Register
        "stw   %r6,   20(%r1)     \n" //Save Conditional Register
       );
   ASMV("lwz   0, 0(%0)           \n" //Load Address from Exception Vector
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_PROGRAM * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_PROGRAM)); //Set Handler Parameter
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz   %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz   %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz   %r4,   12(%r1)     \n" //Load Counter Register
        "lwz   %r3,  156(%r1)     \n" //Load Link Register
        "mtcr  %r6                \n" //Put Conditional Register
        "mtxer %r5                  \n" //Put Fixed Point Register
        "mtctr %r4                  \n" //Put Counter Register
        "mtlr  %r3                  \n" //Put Link Register
        "lwz   %r0,   24(%r1)     \n" //Restore r0
        "lmw   %r2,   32(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 152          \n" //Restore Stack Pointer (r1) 
        "rfi                      \n" //Return from interrupt
        );
}
void __syscall_int() {
   ASMV("stwu  %r1, -152(%r1)     \n" //Create the frame
        "stmw  %r0,   24(%r1)     \n" //Save r0 - r31
        "mflr  %r3                \n" //Get Link Register
        "mfctr %r4                \n" //Get Counter Register
        "mfxer %r5                \n" //Get Fixed Point Exception Register
        "mfcr  %r6                \n" //Get Conditional Register
        "stw   %r3,  156(%r1)     \n" //Save Link Register
        "stw   %r4,   12(%r1)     \n" //Save Counter Register
        "stw   %r5,   16(%r1)     \n" //Save Fixed Point Exception Register
        "stw   %r6,   20(%r1)     \n" //Save Conditional Register
       );
   ASMV("lwz   0, 0(%0)           \n" //Load Address from Exception Vector
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_SYSCALL * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_SYSCALL)); //Set Handler Parameter
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz   %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz   %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz   %r4,   12(%r1)     \n" //Load Counter Register
        "lwz   %r3,  156(%r1)     \n" //Load Link Register
        "mtcr  %r6                \n" //Put Conditional Register
        "mtxer %r5                  \n" //Put Fixed Point Register
        "mtctr %r4                  \n" //Put Counter Register
        "mtlr  %r3                  \n" //Put Link Register
        "lwz   %r0,   24(%r1)     \n" //Restore r0
        "lmw   %r2,   32(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 152          \n" //Restore Stack Pointer (r1) 
        "rfi                      \n" //Return from interrupt
        );
}
void __datatblmiss_int() {
   ASMV("stwu  %r1, -152(%r1)     \n" //Create the frame
        "stmw  %r0,   24(%r1)     \n" //Save r0 - r31
        "mflr  %r3                \n" //Get Link Register
        "mfctr %r4                \n" //Get Counter Register
        "mfxer %r5                \n" //Get Fixed Point Exception Register
        "mfcr  %r6                \n" //Get Conditional Register
        "stw   %r3,  156(%r1)     \n" //Save Link Register
        "stw   %r4,   12(%r1)     \n" //Save Counter Register
        "stw   %r5,   16(%r1)     \n" //Save Fixed Point Exception Register
        "stw   %r6,   20(%r1)     \n" //Save Conditional Register
       );
   ASMV("lwz   0, 0(%0)           \n" //Load Address from Exception Vector
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_DATATBLMISS * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_DATATBLMISS)); //Set Handler Parameter
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz   %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz   %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz   %r4,   12(%r1)     \n" //Load Counter Register
        "lwz   %r3,  156(%r1)     \n" //Load Link Register
        "mtcr  %r6                \n" //Put Conditional Register
        "mtxer %r5                  \n" //Put Fixed Point Register
        "mtctr %r4                  \n" //Put Counter Register
        "mtlr  %r3                  \n" //Put Link Register
        "lwz   %r0,   24(%r1)     \n" //Restore r0
        "lmw   %r2,   32(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 152          \n" //Restore Stack Pointer (r1) 
        "rfi                      \n" //Return from interrupt
        );

}
void __datastor_int() {
   ASMV("stwu  %r1, -152(%r1)     \n" //Create the frame
        "stmw  %r0,   24(%r1)     \n" //Save r0 - r31
        "mflr  %r3                \n" //Get Link Register
        "mfctr %r4                \n" //Get Counter Register
        "mfxer %r5                \n" //Get Fixed Point Exception Register
        "mfcr  %r6                \n" //Get Conditional Register
        "stw   %r3,  156(%r1)     \n" //Save Link Register
        "stw   %r4,   12(%r1)     \n" //Save Counter Register
        "stw   %r5,   16(%r1)     \n" //Save Fixed Point Exception Register
        "stw   %r6,   20(%r1)     \n" //Save Conditional Register
       );
   ASMV("lwz   0, 0(%0)           \n" //Load Address from Exception Vector
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_DATASTORE * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_DATASTORE)); //Set Handler Parameter
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz   %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz   %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz   %r4,   12(%r1)     \n" //Load Counter Register
        "lwz   %r3,  156(%r1)     \n" //Load Link Register
        "mtcr  %r6                \n" //Put Conditional Register
        "mtxer %r5                  \n" //Put Fixed Point Register
        "mtctr %r4                  \n" //Put Counter Register
        "mtlr  %r3                  \n" //Put Link Register
        "lwz   %r0,   24(%r1)     \n" //Restore r0
        "lmw   %r2,   32(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 152          \n" //Restore Stack Pointer (r1) 
        "rfi                      \n" //Return from interrupt
        );

}
void __alignment_int() {
   ASMV("stwu  %r1, -152(%r1)     \n" //Create the frame
        "stmw  %r0,   24(%r1)     \n" //Save r0 - r31
        "mflr  %r3                \n" //Get Link Register
        "mfctr %r4                \n" //Get Counter Register
        "mfxer %r5                \n" //Get Fixed Point Exception Register
        "mfcr  %r6                \n" //Get Conditional Register
        "stw   %r3,  156(%r1)     \n" //Save Link Register
        "stw   %r4,   12(%r1)     \n" //Save Counter Register
        "stw   %r5,   16(%r1)     \n" //Save Fixed Point Exception Register
        "stw   %r6,   20(%r1)     \n" //Save Conditional Register
       );
   ASMV("lwz   0, 0(%0)           \n" //Load Address from Exception Vector
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_ALIGNMENT * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_ALIGNMENT)); //Set Handler Parameter
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz   %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz   %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz   %r4,   12(%r1)     \n" //Load Counter Register
        "lwz   %r3,  156(%r1)     \n" //Load Link Register
        "mtcr  %r6                \n" //Put Conditional Register
        "mtxer %r5                  \n" //Put Fixed Point Register
        "mtctr %r4                  \n" //Put Counter Register
        "mtlr  %r3                  \n" //Put Link Register
        "lwz   %r0,   24(%r1)     \n" //Restore r0
        "lmw   %r2,   32(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 152          \n" //Restore Stack Pointer (r1) 
        "rfi                      \n" //Return from interrupt
        );
}
void __timer_wrapper1() {
   ASMV("b 4352");
}
void __timer_wrapper2() {
   ASMV("b 4608");
}
void __fit_int() {
   ASMV("stwu  %r1, -152(%r1)  \n" //Create the frame
        "stmw  %r0,   24(%r1)  \n" //Save r0 - r31
        "mflr  %r3             \n" //Get Link Register
        "mfctr %r4             \n" //Get Counter Register
        "mfxer %r5             \n" //Get Fixed Point Exception Register
        "mfcr  %r6             \n" //Get Conditional Register
        "stw   %r3,  156(%r1)  \n" //Save Link Register
        "stw   %r4,   12(%r1)  \n" //Save Counter Register
        "stw   %r5,   16(%r1)  \n" //Save Fixed Point Exception Register
        "stw   %r6,   20(%r1)  \n" //Save Conditional Register
       );
   ASMV("lwz   0, 0(%0)     \n" //Load Address from Exception Vector
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_FITIMER * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_FITIMER)); //Set Handler Parameter
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz   %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz   %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz   %r4,   12(%r1)     \n" //Load Counter Register
        "lwz   %r3,  156(%r1)     \n" //Load Link Register
        "mtcr  %r6                \n" //Put Conditional Register
        "mtxer %r5                  \n" //Put Fixed Point Register
        "mtctr %r4                  \n" //Put Counter Register
        "mtlr  %r3                  \n" //Put Link Register
        "lwz   %r0,   24(%r1)     \n" //Restore r0
        "lmw   %r2,   32(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 152          \n" //Restore Stack Pointer (r1) 
        "rfi                      \n" //Return from interrupt
        );
}
void __pit_int() {
   ASMV("stwu   %r1, -160(%r1)  \n" //Create the frame
        "stmw   %r0,   32(%r1)  \n" //Save r0 - r31
        "mflr   %r3             \n" //Get Link Register
        "mfctr  %r4             \n" //Get Counter Register
        "mfxer  %r5             \n" //Get Fixed Point Exception Register
        "mfcr   %r6             \n" //Get Conditional Register
        "mfsrr0 %r7             \n" //Get Save/Restore 0
        "mfsrr1 %r8             \n" //Get Save/Restore 1
        "stw    %r3,  164(%r1)  \n" //Save Link Register
        "stw    %r4,   12(%r1)  \n" //Save Counter Register
        "stw    %r5,   16(%r1)  \n" //Save Fixed Point Exception Register
        "stw    %r6,   20(%r1)  \n" //Save Conditional Register
        "stw    %r7,   24(%r1)  \n" //Save Save/Restore 0
        "stw    %r8,   28(%r1)  \n" //Save Save/Restore 1
        "sync                   \n" //Sync Store & Instruction execution
       );
   ASMV("mtspr %0, %1             \n" //Clear Interrupt.
        : : "i"(ML310_Timer::SPR_TSR),"r"(0x08000000) : "%0");
   ASMV("lwz   0, 0(%0)           \n" //Load Address from Interrupt Vector
        : : "r"(Traits<ML310>::INT_BASE + (IC::INT_PITIMER * 4)) : "%0");
   ASMV("li    3, %0              \n" : : "i"(IC::INT_PITIMER)); //Set Handler Parameter
   ASMV("mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n" //Halt CPU (Put in Wait State)
        //Restore Context:
        "lwz    %r8,   28(%r1)     \n" //Load Save/Restore 1
        "lwz    %r7,   24(%r1)     \n" //Load Save/Restore 0
        "lwz    %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz    %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz    %r4,   12(%r1)     \n" //Load Counter Register
        "lwz    %r3,  164(%r1)     \n" //Load Link Register
        "mtsrr1 %r8                \n" //Get Save/Restore 1
        "mtsrr0 %r7                \n" //Get Save/Restore 0
        "mtcr   %r6                \n" //Put Conditional Register
        "mtxer  %r5                   \n" //Put Fixed Point Register
        "mtctr  %r4                   \n" //Put Counter Register
        "mtlr   %r3                   \n" //Put Link Register
        //Disable Wait State if in Wait State !!!
        "mfsrr1 0                  \n" //Load MSR (on SSR1)
        "lis    3, 0xFFFB          \n" // Upper Mask of Wait State
        "ori    3, 3, 0xFFFF       \n" // Lower Mask of Wait State
        "and    0, 0, 3            \n" // Disable Wait State
        "mtsrr1 0                  \n" //Update MSR (on SSR1)
        //^--For CPU::halt() method !
        "lwz   %r0,   32(%r1)     \n" //Restore r0
        "lmw   %r2,   40(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 160          \n" //Restore Stack Pointer (r1) 
        "sync                     \n" //Sync Storage & Instruction execution
        "rfi                      \n" //Return from interrupt
        );
}
void __noncritical_int() {
   ASMV("stwu   %r1, -160(%r1)  \n" //Create the frame
        "stmw   %r0,   32(%r1)  \n" //Save r0 - r31
        "mflr   %r3             \n" //Get Link Register
        "mfctr  %r4             \n" //Get Counter Register
        "mfxer  %r5             \n" //Get Fixed Point Exception Register
        "mfcr   %r6             \n" //Get Conditional Register
        "mfsrr0 %r7             \n" //Get Save/Restore 0
        "mfsrr1 %r8             \n" //Get Save/Restore 1
        "stw    %r3,  164(%r1)  \n" //Save Link Register
        "stw    %r4,   12(%r1)  \n" //Save Counter Register
        "stw    %r5,   16(%r1)  \n" //Save Fixed Point Exception Register
        "stw    %r6,   20(%r1)  \n" //Save Conditional Register
        "stw    %r7,   24(%r1)  \n" //Save Save/Restore 0
        "stw    %r8,   28(%r1)  \n" //Save Save/Restore 1
        "sync                   \n" //Sync Store & Instruction execution
       );

        // Clear Interrupt
   ASMV("lwz   4, 0(%0)         \n" : : "r"(ML310_IC::IVR) : "%0");
   ASMV("li    5, 1             \n"
        "slw   6, 5, 4          \n"
        "stw   6, 0(%0)         \n" : : "r"(ML310_IC::IAR) : "%0");

   //volatile unsigned int ivr;
   //ASMV("lwz   %0, 0(%1)           \n" : "=r"(ivr) : "r"(ML310_IC::IVR) : "%0");
   //ivr = 1 << ivr;
   //ASMV("stw   %0, 0(%1)           \n" : : "r"(ivr), "r"(ML310_IC::IAR) : "%0");

   //ASMV("lwz   9, 0(%0)           \n" : : "r"(ML310_IC::IVR) : "%0");
   ASMV("addi  4, 4, 12           \n" // Add IRQ displacement
        "mr    3, 4               \n" // Set Handler Parameter
        "slwi  4, 4,  2           \n" // Multiply by 4
        "add   4, 4, %0           \n" : : "r"(Traits<ML310>::INT_BASE)); //Sum with base addr
   ASMV("lwz   0, 0(4)            \n" //Load Handler Addr.
        "mtctr 0                  \n" //Load Handler Address in CounterRegister
        "cmpwi 0, 0               \n" //compare If null
        "beq 12                   \n" //If == 0, branch PANIC
        "bctrl                    \n" //If != 0, branch to handler
        "b 32                     \n" //After Handler Routine, restore_context
        //Panic:
        "lis   3, 0x9000          \n" //LED UPPER ADDR
        "ori   3, 3, 0x0000       \n" //LED LOWER ADDR
        "lis   4, 0x0000          \n" //Value Upper
        "ori   4, 4, 0x0000       \n" //Value Lower
        "stw   4, 0(3)            \n" //Turn On LEDS
        "li    0, 8192            \n" //Set MSR for HALT
        "mtmsr 0                  \n"); //Halt CPU (Put in Wait State)
        //Restore Context:
   ASMV("lwz    %r8,   28(%r1)     \n" //Load Save/Restore 1
        "lwz    %r7,   24(%r1)     \n" //Load Save/Restore 0
        "lwz    %r6,   20(%r1)     \n" //Load Conditional Register
        "lwz    %r5,   16(%r1)     \n" //Load Fixed Point Exception Register
        "lwz    %r4,   12(%r1)     \n" //Load Counter Register
        "lwz    %r3,  164(%r1)     \n" //Load Link Register
        "mtsrr1 %r8                \n" //Get Save/Restore 1
        "mtsrr0 %r7                \n" //Get Save/Restore 0
        "mtcr   %r6                \n" //Put Conditional Register
        "mtxer  %r5                \n" //Put Fixed Point Register
        "mtctr  %r4                \n" //Put Counter Register
        "mtlr   %r3                \n" //Put Link Register
        "lwz   %r0,   32(%r1)     \n" //Restore r0
        "lmw   %r2,   40(%r1)     \n" //Restore r2 - r31
        "addi  1, 1, 160          \n" //Restore Stack Pointer (r1) 
        "sync                     \n" //Sync Storage & Instruction execution
        "rfi                      \n" //Return from interrupt
        );
}
//This interrupt is disable and never should occur (CPU PIN Disconnected)
void __critical_int() {
   ASMV("rfci");
}

extern "C" { void _start(); }

void _start() 
{
    __USING_SYS
    ML310_Setup ml310_setup(
	reinterpret_cast<char *>(Traits<ML310>::BOOT_IMAGE_ADDR));
}
