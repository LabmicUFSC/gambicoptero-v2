// EPOS ATMega16 Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/atmega16/machine.h>

extern "C" void __epos_free_init_mem();

extern "C" void __epos_free_init_mem() {
    
//    MMU::free(Memory_Map<ATMega128>::TOP,$
//	      Memory_Map<ATMega128>::MEM_SIZE - 
//	      Memory_Map<ATMega128>::TOP + 
//	      Memory_Map<ATMega128>::PHY_MEM);
    
}
