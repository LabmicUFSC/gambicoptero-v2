// EPOS ATMega128 RTC Mediator Declarations

#ifndef __atmega128_rtc_h
#define __atmega128_rtc_h

#include <rtc.h>

__BEGIN_SYS

class ATMega128_RTC: public RTC_Common
{
private:
    static const unsigned long EPOCH_DAYS = Traits<ATMega128_RTC>::EPOCH_DAYS;

public:
    ATMega128_RTC() {}

    static Date date();
    static void date(const Date & d);

    static Second seconds_since_epoch() { 
	return date().to_offset(EPOCH_DAYS); 
    }
};

__END_SYS

#endif
