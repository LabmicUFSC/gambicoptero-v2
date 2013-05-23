// EPOS MIPS_TSC Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mips32_tsc_h
#define __mips32_tsc_h

#include <cpu.h>
#include <tsc.h>

__BEGIN_SYS

class MIPS32_TSC: public TSC_Common
{
private:
    static const unsigned int CLOCK = Traits<Machine>::CLOCK;

public:
    MIPS32_TSC() {}

    static Hertz frequency() { return CLOCK; }

    static Time_Stamp time_stamp() {
		volatile unsigned int *counter = (unsigned int *)(Traits<MIPS32_TSC>::ADDRESS);
        return *counter;
    }
};

__END_SYS

#endif
