// EPOS MIPS32 FPU Mediator Declarations

#ifndef __mips32_fpu_h
#define __mips32_fpu_h

#include <fpu.h>

__BEGIN_SYS

class MIPS32_FPU: public FPU_Common
{
public:
    MIPS32_FPU() {}
};

__END_SYS

#endif
