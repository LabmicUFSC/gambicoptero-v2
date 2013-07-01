// EPOS ATMega16 Mediator Implementation

#include <mach/atmega16/machine.h>

extern "C" void __epos_free_init_mem();

extern "C" void __epos_free_init_mem() {
    
//    MMU::free(Memory_Map<ATMega128>::TOP,$
//	      Memory_Map<ATMega128>::MEM_SIZE - 
//	      Memory_Map<ATMega128>::TOP + 
//	      Memory_Map<ATMega128>::PHY_MEM);
    
}
