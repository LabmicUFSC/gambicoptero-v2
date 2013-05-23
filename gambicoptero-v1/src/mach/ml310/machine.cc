// EPOS ML310 Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/ml310/machine.h>

extern "C" { void _exit(int s); }

__BEGIN_SYS

// Class attributes

// Class methods
void ML310::panic()
{
    db<ML310>(ERR) << "PANIC!\n";
    CPU::int_disable(); 
    CPU::halt();
}

void ML310::int_not(unsigned int i)
{
    db<ML310>(WRN) << "\nInt " << i
                   << " occurred, but no handler installed\n";
    panic();
}

void ML310::exc_program(unsigned int i)
{
    Reg32 esr = CPU::_mfspr(CPU::ESR);
    Reg32 addr = CPU::_mfspr(CPU::SSR0);
    if(esr & CPU::ESR_PIL){
        db<ML310>(ERR) << "Illegal Instruction Executed! (addr=" << (void*)addr << ", data=" << (void*)(*((unsigned int*)addr)) << ")\n";
        panic();
    }
    if(esr & CPU::ESR_PPR){
        db<ML310>(ERR) << "Privileged Instruction executed in ProblemState! (" << (void*)addr << ")\n";
        panic();
    }
    if(esr & CPU::ESR_PTR){
        db<ML310>(ERR) << "Trap executed, but not implemented! (" << (void*)addr << ")\n";
        panic();
    }
}

__END_SYS
