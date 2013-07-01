// EPOS ARM7 FPU Mediator Declarations

#ifndef __arm7_fpu_h
#define __arm7_fpu_h

#include <fpu.h>

__BEGIN_SYS

class ARM7_FPU: public FPU_Common
{
public:
    ARM7_FPU() {}
};

__END_SYS

#endif
