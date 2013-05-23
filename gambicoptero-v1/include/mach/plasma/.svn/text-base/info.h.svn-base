// EPOS PLASMA Run-Time System Information

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
// Cancian

#ifndef __plasma_info_h
#define __plasma_info_h

#include <system/info.h>

__BEGIN_SYS

template<>
struct System_Info<PLASMA>
{
private:
    typedef unsigned int LAddr;
    typedef unsigned int PAddr;
    typedef unsigned int Size;

public:
    // The information we have at boot time (built up by MKBI)

    struct Boot_Map      {
       unsigned int n_cpus;     // Number of CPUs in SMPs
       PAddr mem_base;          // Memory base address
       PAddr mem_top;           // Memory top address
       PAddr io_mem_base;       // I/O Memory base address
       PAddr io_mem_top;        // I/O Memory top address
       short node_id;           // Local node id in SAN (-1 => RARP)
       short n_nodes;           // Number of nodes in SAN (-1 => dynamic)
       Size img_size;           // Boot image size (in bytes)
       int setup_offset;        // Image offsets (-1 => not present)
       int init_offset;
       int system_offset;
       int application_offset;
       int extras_offset;
    };
 
    // Physical Memory Map (built up by SETUP)
    struct Physical_Memory_Map
    {
	PAddr mem_base;      // Memory base address
	PAddr mem_top;       // Memory top address
	PAddr io_mem_base;   // I/O Memory base address
	PAddr io_mem_top;    // I/O Memory top address
        PAddr ext_base;      // Boot Image EXTRA segment base address
        PAddr ext_top;       // Boot Image EXTRA segment top address
	PAddr int_vec;       // Interrupt Vector
	PAddr sys_info;      // System Info
	PAddr sys_code;      // OS Code Segment
	PAddr sys_data;      // OS Data Segment
	PAddr sys_stack;     // OS Stack Segment
	PAddr free_base;    // First free memory chunk base address
	PAddr free_top;     // First free memory chunk top address
    };

    // Logical Memory Map (built by MACHINE_INIT)    
    struct Logical_Memory_Map
    {
	LAddr app_entry;       // First application's entry point
    };

    //Verify Latter (dependency on system_init)
    struct Load_Map {
        bool  has_sys; //Always in library mode...
    };

public:
    Boot_Map bm;
    Physical_Memory_Map pmm;
    Logical_Memory_Map lmm;
    Load_Map lm;
};

__END_SYS

#endif
