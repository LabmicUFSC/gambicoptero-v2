// EPOS AVR Machine Common Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __avr_machine_h
#define __avr_machine_h

#include <cpu.h>
#include <machine.h>

__BEGIN_SYS

class AVR_Machine
{
protected:
    AVR_Machine() {}

public:
    static void panic() { CPU::halt(); }
};

__END_SYS

#endif
