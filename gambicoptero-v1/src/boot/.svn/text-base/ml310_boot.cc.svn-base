// EPOS ML310 Boot Strap

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/elf.h>
#include <machine.h>

__USING_SYS

// System_Info Imports
typedef System_Info<ML310> SI;
typedef Memory_Map<ML310> MM;
typedef void (function_type)();

// Constants
static const unsigned int BOOT_IMAGE = 2 * 1024;

// Prototypes
extern "C" { void _start(); }
inline void _panic(unsigned int code);

void _start()
{
    //Light on GPIO Leds
    *((volatile unsigned int *)(Traits<Machine>::LEDS_BASEADDR+4)) = 0x00000000;
    //Light Stage1 Boot.
    *((volatile unsigned int *)(Traits<Machine>::LEDS_BASEADDR)) = 0xFFFFFFFE;

    // Setting a stack for EPOS initialization
    unsigned int stack_pointer = MM::SYS_STACK; 
    ASMV("mr 1,%0" : : "r" (stack_pointer));

    // The boot image was loaded by the monitor at BOOT_IMAGE
    char * tmp = reinterpret_cast<char *>(BOOT_IMAGE);

    // System_Info is the first thing in the boot image
    SI * si = reinterpret_cast<SI *>(tmp);

    // And will be moved to the place indicated in Traits
    char * bi = reinterpret_cast<char *>(Traits<ML310>::BOOT_IMAGE_ADDR);
    memcpy(bi, tmp, si->bm.img_size);

    // Reset si to its definitive location
    si = reinterpret_cast<SI *>(bi);

    // Load SETUP
    ELF * stp_elf = reinterpret_cast<ELF *>(&bi[si->bm.setup_offset]);
    if(!stp_elf->valid()) {
	_panic(0x0F);
    }
    if(stp_elf->load_segment(0) < 0) {
	_panic(0x06);
    }
    for(int i = 1; i < stp_elf->segments(); i++) {
	if(stp_elf->load_segment(i) < 0) {
	    _panic(0x09);
	}
    }

    // Call setup
    void (*entry)() = reinterpret_cast<void (*)()>(stp_elf->entry());
    *((volatile unsigned int *)(Traits<Machine>::LEDS_BASEADDR)) = 0xFFFFFFFC;
    entry();
}

void _panic(unsigned int code){
    while(1){
       *((volatile unsigned int *)(Traits<Machine>::LEDS_BASEADDR)) = code;
       for(int i = 0; i < 0x1000000; i++);
       *((volatile unsigned int *)(Traits<Machine>::LEDS_BASEADDR)) = 0;
       for(int i = 0; i < 0x1000000; i++);
    }
}

