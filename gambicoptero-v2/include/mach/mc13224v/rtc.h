// EPOS-- MC13224V RTC Mediator Declarations

#ifndef __mc13224v_rtc_h
#define __mc13224v_rtc_h

#include <rtc.h>

__BEGIN_SYS

class MC13224V_RTC: public RTC_Common
{
private:
    static const unsigned int EPOCH_DAYS = Traits<MC13224V_RTC>::EPOCH_DAYS;

public:
	MC13224V_RTC() {}

    static Date date();
    static void date(const Date & d);

    static Second seconds_since_epoch() { 
    return date().to_offset(EPOCH_DAYS);
    }
};

__END_SYS

#endif
