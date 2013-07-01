// EPOS IA32 FPU Mediator Implementation

#include <arch/ia32/fpu.h>

__BEGIN_SYS

// Class methods
void IA32_FPU::save(volatile Context * context)
{
     ASMV("fnsave %0\n"
          "fnclex"
         :
         : "m"(*context)
         : );
}

void IA32_FPU::load(volatile Context * context)
{

     ASMV("nop ; frstor %0\n"
         :
         : "m"(*context)
         : );
} 
__END_SYS
