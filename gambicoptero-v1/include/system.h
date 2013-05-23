// EPOS Global System Abstraction Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __system_h
#define __system_h

#include <utility/heap.h>

__BEGIN_SYS

//class Machine;

class System
{
public:
    static System_Info<Machine> * const info();
    static Heap * const heap() { return &_heap; }

    static void init();

private:
    static System_Info<Machine> * _si;
    static Heap _heap;
};

__END_SYS

#endif
