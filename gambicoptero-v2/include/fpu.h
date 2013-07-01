// EPOS CPU Mediator Common Package

#ifndef __fpu_h
#define __fpu_h

#include <system/config.h>

__BEGIN_SYS

class FPU_Common
{	
protected:
    FPU_Common() {}

public:
    typedef unsigned char Reg8;
    typedef unsigned short Reg16;
    typedef unsigned long Reg32;
    typedef unsigned long long Reg64;

    class Context;

public:
    static void init() { }

	static void save(volatile Context * context) { }
	static void load(volatile Context * context) { } 

};

__END_SYS

#ifdef __FPU_H
#include __FPU_H
#endif

#endif
