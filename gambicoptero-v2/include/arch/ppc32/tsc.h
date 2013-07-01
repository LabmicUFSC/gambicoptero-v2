// EPOS PPC32_TSC Declarations

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
