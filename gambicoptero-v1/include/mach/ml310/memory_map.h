// EPOS Memory Map for the ML310

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __ml310_memory_map_h
#define __ml310_memory_map_h

#include <system/memory_map.h>

__BEGIN_SYS

template <>
struct Memory_Map<ML310>
{
    enum Memory {
        MEM_BASE =              0,
        MEM_SIZE =              64 * 1024 * 1024,
    };

    enum Map {
        BASE =          0x00000000,
        TOP =           0xFFFFFFFF,
        APP_LO =        0x00000000,
        APP_CODE =      0x00000000,
        APP_DATA =      0x00200000,
        APP_HI =        0x03800000,
        PHY_MEM =       0x00000000,
        IO_MEM =        Traits<ML310_PCI>::MEM_SPACE_LIMIT,
        INT_VEC =       0x03ff0000,
        SYS_INFO =      0x03ff4000,
        SYS =           0x03ff5000,
        SYS_CODE =      0x03ff5000,
        SYS_DATA =      0x03ff9000,
        SYS_STACK =     0x03fff000
    };
};

__END_SYS

#endif
