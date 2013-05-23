// EPOS Application-level Dynamic Memory Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __malloc_h
#define __malloc_h

#include <application.h>

extern "C"
{
    inline void * malloc(size_t bytes) {
	return __SYS(Application)::heap()->alloc(bytes);
    }

    inline void * calloc(size_t n, unsigned int bytes) {
	return __SYS(Application)::heap()->calloc(n * bytes);
    }

    inline void free(void * ptr) {
	__SYS(Application)::heap()->free(ptr);
    }

    inline void * p_malloc(size_t bytes) {
        return __SYS(Application)::priority_heap()->alloc(bytes);
    }

    inline void * p_calloc(size_t n, unsigned int bytes) {
        return __SYS(Application)::priority_heap()->calloc(n * bytes);
    }

    inline void p_free(void * ptr) {
        __SYS(Application)::priority_heap()->free(ptr);
    }
}

inline void * decide_malloc(unsigned int bytes,
        __SYS(alloc_priority) p = __SYS(ALLOC_P_NORMAL)) {
    if(__SYS(Heap)::to_priority_heap(bytes, p)){
        void *rtn = p_malloc(bytes);
        if(rtn == 0) rtn = malloc(bytes);
        return rtn;
    }
    else {
        void *rtn = malloc(bytes);
        if(rtn == 0) rtn = p_malloc(bytes);
        return rtn;
    }
}
inline void * decide_calloc(unsigned int n, unsigned int bytes,
        __SYS(alloc_priority) p = __SYS(ALLOC_P_NORMAL)) {
    if(__SYS(Heap)::to_priority_heap(n*bytes, p))
            return p_calloc(n, bytes);
        else
            return calloc(n, bytes);
}
inline void decide_free(void * ptr) {
    if(__SYS(Heap)::from_priority_heap(ptr))
        p_free(ptr);
    else
        free(ptr);
}

inline void * operator new(size_t bytes, __SYS(alloc_priority) p) {
    if(__SYS(Traits)<__SYS(Heap)>::priority_alloc)
        return decide_malloc(bytes, p);
    else
        return malloc(bytes);
}
inline void * operator new[](size_t bytes, __SYS(alloc_priority) p) {
    if(__SYS(Traits)<__SYS(Heap)>::priority_alloc)
        return decide_malloc(bytes, p);
    else
        return malloc(bytes);
}

inline void * operator new(size_t bytes) {
    if(__SYS(Traits)<__SYS(Heap)>::priority_alloc)
        return decide_malloc(bytes);
    else
        return malloc(bytes);
}
inline void * operator new[](size_t bytes) {
    if(__SYS(Traits)<__SYS(Heap)>::priority_alloc)
        return decide_malloc(bytes);
    else
        return malloc(bytes);
}
inline void operator delete(void * object) {
    if(__SYS(Traits)<__SYS(Heap)>::priority_alloc)
        return decide_free(object);
    else
        return free(object);
}
inline void operator delete[](void * object) {
    if(__SYS(Traits)<__SYS(Heap)>::priority_alloc)
        return decide_free(object);
    else
        return free(object);
}

#endif
