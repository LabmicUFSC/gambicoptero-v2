// EPOS TSC Mediator Common Package

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __tsc_h
#define __tsc_h

#include <system/config.h>

__BEGIN_SYS

class TSC_Common
{
protected:
    TSC_Common() {}

public:
    typedef unsigned long Hertz;
    typedef unsigned long long Time_Stamp;
};

__END_SYS

#ifdef __TSC_H
#include __TSC_H
#endif

#endif
