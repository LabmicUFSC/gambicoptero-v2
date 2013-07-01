// EPOS ATMega1281 RTC Mediator Declarations

#ifndef __atmega1281_rtc_h
#define __atmega1281_rtc_h

#include <rtc.h>

__BEGIN_SYS

class ATMega1281_RTC: public RTC_Common
{
private:
    static const unsigned long EPOCH_DAYS = Traits<ATMega1281_RTC>::EPOCH_DAYS;

public:
    ATMega1281_RTC() {}

    static Date date();
    static void date(const Date & d);

    static Second seconds_since_epoch() { 
	return date().to_offset(EPOCH_DAYS); 
    }
};

__END_SYS

#endif

