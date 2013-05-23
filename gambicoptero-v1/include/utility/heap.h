// EPOS Heap Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __heap_h
#define __heap_h

#include <utility/debug.h>
#include <utility/string.h>
#include <utility/list.h>
#include <utility/spin.h>

__BEGIN_SYS

// Priority allocation type definitions
typedef enum {
    ALLOC_P_HIGH,
    ALLOC_P_LOW,
    ALLOC_P_NORMAL,
} alloc_priority;


// Heap Common Packages (actually the non-atomic heap)
class Heap_Common: private Grouping_List<char>
{
public:
    using Grouping_List<char>::empty;
    using Grouping_List<char>::size;

    Heap_Common() {
	db<Init, Heap>(TRC) << "Heap() => " << this << "\n";
    }

    Heap_Common(void * addr, unsigned int bytes) {
	db<Init, Heap>(TRC) << "Heap(addr=" << addr << ",bytes=" << bytes 
			    << ") => " << this << "\n";  
    }

    void * alloc(unsigned int bytes); 

    void * calloc(unsigned int bytes); 

    void free(void * ptr); 

    void free(void * ptr, unsigned int bytes);

    unsigned int allocated();
    void allocated(unsigned int bytes);

    unsigned int max_depth();
    void max_depth(unsigned int bytes);

    static bool to_priority_heap(unsigned int bytes, alloc_priority p);
    static bool from_priority_heap(void * ptr);

private:
    void out_of_memory();
};


//Heap used when priority allocation is enabled
class Heap_Profiled : public Heap_Common {

public:
    Heap_Profiled()
        :Heap_Common(),
         _allocated(0),
         _max_depth(0)
    {}

    Heap_Profiled(void * addr, unsigned int bytes)
        :Heap_Common(addr, bytes),
        _allocated(0),
        _max_depth(0)
    {}

    void * alloc(unsigned int bytes){
        void* aux = Heap_Common::alloc(bytes);
        if(aux){
            _allocated += bytes;
            _max_depth = _allocated > _max_depth ? _allocated : _max_depth;
        }
        return aux;
    }

    void free(void * ptr, unsigned int bytes){
        Heap_Common::free(ptr, bytes);
        _allocated -= bytes;
    }

    void * calloc(unsigned int bytes) {return Heap_Common::calloc(bytes);}

    void free(void * ptr){
        int * addr = reinterpret_cast<int *>(ptr);
        free(&addr[-1], addr[-1]);
    }

    static bool to_priority_heap(unsigned int bytes, alloc_priority p);
    static bool from_priority_heap(void * ptr);

    unsigned int allocated() { return _allocated; }
    void allocated(unsigned int bytes) { _allocated = bytes; }

    unsigned int max_depth() { return _max_depth; }
    void max_depth(unsigned int bytes) { _max_depth = bytes; }

private:
    unsigned int _allocated;
    unsigned int _max_depth;
};


// Wrapper for non-atomic heap  
template <bool atomic>
class Heap_Wrapper: public IF<Traits<Heap>::priority_alloc, Heap_Profiled, Heap_Common>::Result {};


// Wrapper for atomic heap
template<>
class Heap_Wrapper<true>: public IF<Traits<Heap>::priority_alloc, Heap_Profiled, Heap_Common>::Result
{
public:
    typedef IF<Traits<Heap>::priority_alloc, Heap_Profiled, Heap_Common>::Result Base;

    Heap_Wrapper() {}

    Heap_Wrapper(void * addr, unsigned int bytes): Base(addr, bytes) {
	free(addr, bytes); 
    }

    void * alloc(unsigned int bytes) {
	_lock.acquire();
	void * tmp = Base::alloc(bytes);
	_lock.release();
	return tmp;
    }

    void * calloc(unsigned int bytes) {
	_lock.acquire();
	void * tmp = Base::calloc(bytes);
	_lock.release();
	return tmp;
    }

    void free(void * ptr) {
	_lock.acquire();
	Base::free(ptr);
	_lock.release();
    }

    void free(void * ptr, unsigned int bytes) {
	_lock.acquire();
	Base::free(ptr, bytes);
	_lock.release();
    }

private:
    Spin _lock;
};


// Heap
class Heap: public Heap_Wrapper<Traits<Thread>::smp> {};

__END_SYS

#endif
