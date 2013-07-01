// EPOS AVR Machine Common Declarations

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
