// EPOS Global System Abstraction Declarations

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
