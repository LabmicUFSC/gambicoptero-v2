#ifndef __integrator_rtc_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __integrator_rtc_h

#include <rtc.h>

__BEGIN_SYS

class IntegratorCP_RTC: public RTC_Common
{
public:
	IntegratorCP_RTC() {}

    static Date date();
    static void date(const Date & d);

    static Second seconds_since_epoch() { 
        return 0;
    }
};

__END_SYS

#endif
