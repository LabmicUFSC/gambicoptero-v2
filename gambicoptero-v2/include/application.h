// EPOS Global Application Abstraction Declarations

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
