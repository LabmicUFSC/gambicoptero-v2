// EPOS AT90CAN128_RTC Declarations

#ifndef __at90can128_rtc_h
#define __at90can128_rtc_h

#include <rtc.h>

__BEGIN_SYS

class AT90CAN128_RTC: public RTC_Common
{
private:
    static const unsigned int EPOCH_DAYS = Traits<AT90CAN128_RTC>::EPOCH_DAYS;

public:
    AT90CAN128_RTC() {}

    static Date date();
    static void date(const Date & d);

    static Second seconds_since_epoch() { 
    return date().to_offset(EPOCH_DAYS); 
    }
};

__END_SYS

#endif
