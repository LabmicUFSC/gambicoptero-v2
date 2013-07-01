// EPOS Memory Map for the PLASMA

#ifndef __plasma_memory_map_h
#define __plasma_memory_map_h

#include <system/memory_map.h>

__BEGIN_SYS

template <>
struct Memory_Map<PLASMA>
{
    enum Memory {
        MEM_BASE =              0x10000010,
        MEM_SIZE =              (1 * 1024 * 1024) - 16,
    };

    enum Map {
        BASE =          0x10000010,
        TOP =           0x10100000,
        APP_LO =        0x10000010,
        APP_CODE =      0x10000010,
        APP_DATA =      0x10080000,
        APP_HI =        0x100A0000,
        PHY_MEM =       0x10000004,
        IO_MEM =        0x20000000,
        INT_VEC =       0x0000003C,
        SYS_INFO =      0x1FFFF000,
        SYS =           0x100A0000,
        SYS_CODE =      0x100A0000,
        SYS_DATA =      0x100A2000,
        SYS_STACK =     0x1FFFE000
    };
};

__END_SYS

#endif
