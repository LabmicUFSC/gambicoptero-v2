// EPOS ML310_RTC Declarations

#ifndef __ml310_rtc_h
#define __ml310_rtc_h

#include <rtc.h>

__BEGIN_SYS

class ML310_RTC: public RTC_Common
{
private:
    static const unsigned int EPOCH_DAYS = Traits<ML310_RTC>::EPOCH_DAYS;

public:
    ML310_RTC() { }

    static Date date();
    static void date(const Date & d);

    static Second seconds_since_epoch() { 
        return date().to_offset(EPOCH_DAYS); 
    }
};

__END_SYS

#endif
