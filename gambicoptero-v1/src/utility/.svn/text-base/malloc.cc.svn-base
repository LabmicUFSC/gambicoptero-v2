// EPOS Application-level Dynamic Memory Utility Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#define __malloc_h

#include <utility/malloc.h>
#include <application.h>

__USING_SYS

extern "C"
{
    void * malloc(size_t bytes) {
	return Application::heap()->alloc(bytes);
    }

    void * calloc(size_t n, unsigned int bytes) {
	return Application::heap()->calloc(n * bytes);
    }

    void free(void * ptr) {
	Application::heap()->free(ptr);
    }

    void * p_malloc(size_t bytes) {
    return Application::priority_heap()->alloc(bytes);
    }

    void * p_calloc(size_t n, unsigned int bytes) {
    return Application::priority_heap()->calloc(n * bytes);
    }

    void p_free(void * ptr) {
    Application::priority_heap()->free(ptr);
    }
}

