// EPOS-- IntegratorCP Memory Map

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __memory_map_integratorcp_h
#define __memory_map_integratorcp_h

#include <system/memory_map.h>

__BEGIN_SYS

template <>
struct Memory_Map<IntegratorCP>
{
    enum {
        MEM_BASE =  0,
        MEM_SIZE =  16 * 1024 * 1024
    };

    enum {
        BASE =      0x00000000,
        TOP =       0x01EFFFFF,
        APP_LO =    0x00000000,
        APP_CODE =  0x00000000,
        APP_DATA =  0x01000000,
        APP_HI =    0x01EFFFFF,
        PHY_MEM =   0x00000000,
        IO_MEM =    0x10000000,
        APIC_MEM =  0x00000000,
        SYS =       0x01F00000,
        IDT =       SYS + 0x00010000,
        GDT =       SYS + 0x00020000,
        SYS_PT =    SYS + 0x00030000,
        SYS_PD =    SYS + 0x00040000,
        SYS_INFO =  SYS + 0x00050000,
        SYS_CODE =  SYS + 0x00060000,
        SYS_DATA =  SYS + 0x00070000,
        SYS_STACK = SYS + 0x00080000
    };
};

__END_SYS

#endif

