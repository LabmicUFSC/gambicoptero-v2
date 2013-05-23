// EPOS System-level Dynamic Memory Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __kmalloc_h
#define __kmalloc_h

#include <system.h>

__BEGIN_SYS

inline void * kmalloc(unsigned int bytes) { 
    return System::heap()->alloc(bytes);
}
inline void * kcalloc(unsigned int n, unsigned int bytes) {
    return System::heap()->calloc(n * bytes);
}
inline void kfree(void * ptr) {
    System::heap()->free(ptr);
}

__END_SYS

#endif
