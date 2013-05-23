// EPOS Clock Abstraction Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __clock_h
#define __clock_h

#include <rtc.h>

__BEGIN_SYS

class Clock
{
public:
    typedef RTC::Microsecond Microsecond;
    typedef RTC::Second Second;
    typedef RTC::Date Date;

public:
    Clock() {}

    Microsecond resolution() { return 1000000; }

    Second now() { return RTC::seconds_since_epoch(); }

    Date date() { return RTC::date(); }
    void date(const Date & d) { return RTC::date(d); }
};

__END_SYS

#endif
