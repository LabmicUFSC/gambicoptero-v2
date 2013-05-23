// EPOS PPC32_TSC Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __ppc32_tsc_h
#define __ppc32_tsc_h

#include <cpu.h>
#include <tsc.h>

__BEGIN_SYS

class PPC32_TSC: public TSC_Common
{
private:
    static const unsigned int CLOCK = Traits<Machine>::CLOCK;

public:
    PPC32_TSC() {}

    static Hertz frequency() { return CLOCK; }

    static Time_Stamp time_stamp() {
        CPU::Reg32 lower_ts;
        CPU::Reg32 upper_ts;
        CPU::Reg32 upper_cmp;
        do {
          ASMV("mftbu %0" :  "=r" (upper_ts)  : );
          ASMV("mftb %0"  :  "=r" (lower_ts)  : );
          ASMV("mftbu %0" :  "=r" (upper_cmp) : );
        } while (upper_cmp != upper_ts);

        Time_Stamp ts = upper_ts;
        ts = (ts << 32) | lower_ts;

        return ts;
    }
};

__END_SYS

#endif
