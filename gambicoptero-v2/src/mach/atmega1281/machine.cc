// EPOS ATMega1281 Mediator Implementation

#include <mach/atmega1281/machine.h>

__USING_SYS

extern "C" void __epos_free_init_mem();

extern "C" void __epos_free_init_mem() {
    
    MMU::free(Memory_Map<ATMega1281>::TOP,
	      Memory_Map<ATMega1281>::MEM_SIZE - 
	      Memory_Map<ATMega1281>::TOP + 
	      Memory_Map<ATMega1281>::PHY_MEM);
    
}
