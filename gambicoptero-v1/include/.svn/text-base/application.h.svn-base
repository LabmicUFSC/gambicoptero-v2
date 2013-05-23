// EPOS Global Application Abstraction Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __application_h
#define __application_h

#include <utility/heap.h>

__BEGIN_SYS

class Application
{
public:
    static Heap * const heap() { return &_heap[0]; }
    static Heap * const priority_heap() { return &_heap[1]; }

    static void init();

private:
    static Heap _heap[IF_INT<Traits<Heap>::priority_alloc, 2, 1>::Result];
};

__END_SYS

#include <utility/malloc.h>

#endif
