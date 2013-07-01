// EPOS AVR8 FPU Mediator Declarations

#ifndef __avr8_fpu_h
#define __avr8_fpu_h

#include <fpu.h>

__BEGIN_SYS

class AVR8_FPU: public FPU_Common
{
public:
    AVR8_FPU() {}
};

__END_SYS

#endif
