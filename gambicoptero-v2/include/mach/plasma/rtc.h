// EPOS PLASMA_RTC Declarations

#ifndef __plasma_rtc_h
#define __plasma_rtc_h

#include <rtc.h>

__BEGIN_SYS

class PLASMA_RTC: public RTC_Common
{
private:
   static const unsigned int EPOCH_DAYS = 0;

public:
    PLASMA_RTC() { }

    static Date date();
    static void date(const Date & d);

    static Second seconds_since_epoch() { 
        return date().to_offset(EPOCH_DAYS); 
    }
};

__END_SYS

#endif
