// EPOS IA32 Time-Stamp Counter Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __ia32_tsc_h
#define __ia32_tsc_h

#include <cpu.h>
#include <tsc.h>

__BEGIN_SYS

class IA32_TSC: public TSC_Common
{
public:
    IA32_TSC() {}

    static Hertz frequency() { return IA32::clock(); }

    static Time_Stamp time_stamp() {
	Time_Stamp ts;
	ASMV("rdtsc" : "=A" (ts) : ); // must be volatile!
	return ts;
    }
};

__END_SYS

#endif
