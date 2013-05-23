// EPOS Heap Utility Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/heap.h>
#include <machine.h>
#include <application.h>

__BEGIN_SYS

// Methods
void Heap_Common::out_of_memory()
{
    db<Heap>(ERR) << "Heap::alloc(this=" << this
				  << "): out of memory!\n";
    Machine::panic();
}

void * Heap_Common::alloc(unsigned int bytes) {
    if(!bytes)
        return 0;

    while ((bytes % sizeof(void*)) != 0) ++bytes;
    
    bytes += sizeof(int);
    if(bytes < sizeof(Element))
        bytes = sizeof(Element);
    
    Element * e = search_decrementing(bytes);
    if(!e) {
        out_of_memory();
        return 0;
    }
    int * addr = reinterpret_cast<int *>(e->object() + e->size());
        
    db<Heap>(TRC) << "Heap::alloc(this=" << this
              << ",bytes=" << bytes 
              << ") => " << (void *)addr << "\n";

    addr[0] = bytes;
    return &addr[1];
}

void * Heap_Common::calloc(unsigned int bytes) {
    void * addr = alloc(bytes);
    memset(addr, bytes, 0);
    return addr;    
}

void Heap_Common::free(void * ptr) {
    int * addr = reinterpret_cast<int *>(ptr);
    free(&addr[-1], addr[-1]);
}

void Heap_Common::free(void * ptr, unsigned int bytes) {
    db<Heap>(TRC) << "Heap::free(this=" << this
              << ",ptr=" << ptr
              << ",bytes=" << bytes << ")\n";

    if(ptr && (bytes >= sizeof(Element))) {
        Element * e = new (ptr)
        Element(reinterpret_cast<char *>(ptr), bytes);
        Element * m1, * m2;
        insert_merging(e, &m1, &m2);
    }
}

bool Heap_Profiled::to_priority_heap(unsigned int bytes, alloc_priority p){

    bool ret = false;
    switch (p) {
        case ALLOC_P_HIGH:
            if(bytes < (Traits<Machine>::PRIORITY_HEAP_SIZE - Application::priority_heap()->allocated()))
                ret = true;
            else
                ret = false;
            break;
        case ALLOC_P_LOW:
            if(bytes < (Traits<Machine>::APPLICATION_HEAP_SIZE - Application::heap()->allocated()))
                ret = false;
            else
                ret = true;
            break;
        case ALLOC_P_NORMAL:{
            unsigned int spm_used =
                    100 / (Traits<Machine>::PRIORITY_HEAP_SIZE / Application::priority_heap()->allocated());
            unsigned int main_used =
                    100 / (Traits<Machine>::APPLICATION_HEAP_SIZE / Application::heap()->allocated());

            bool spm_fit =
                    bytes < (Traits<Machine>::PRIORITY_HEAP_SIZE - Application::priority_heap()->allocated());
            bool main_fit =
                    bytes < (Traits<Machine>::APPLICATION_HEAP_SIZE - Application::heap()->allocated());
            bool spm_more_free = spm_used <= main_used;

            if(spm_more_free)
                ret = spm_fit;
            else
                ret = !main_fit;

            break;
        }
        default:
            break;
    }
    return ret;
}

bool Heap_Profiled::from_priority_heap(void * ptr){
    return (reinterpret_cast<unsigned int>(ptr) >= Traits<Machine>::PRIORITY_HEAP_BASE_ADDR) &&
            (reinterpret_cast<unsigned int>(ptr) <= Traits<Machine>::PRIORITY_HEAP_TOP_ADDR);
}

__END_SYS
