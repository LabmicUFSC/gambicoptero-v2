// EPOS AT90CAN128 Implementation

#include <mach/at90can128/machine.h>

__USING_SYS

extern "C" void __epos_free_init_mem();

extern "C" void __epos_free_init_mem() {

    MMU::free(Memory_Map<AT90CAN128>::TOP,
          Memory_Map<AT90CAN128>::MEM_SIZE -
          Memory_Map<AT90CAN128>::TOP + 
          Memory_Map<AT90CAN128>::PHY_MEM);

}
