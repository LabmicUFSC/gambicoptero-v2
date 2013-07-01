// EPOS ATMega16 RTC Mediator Declarations

#ifndef __atmega16_rtc_h
#define __atmega16_rtc_h

#include <rtc.h>

__BEGIN_SYS

class ATMega16_RTC: public RTC_Common
{
private:
    static const unsigned int EPOCH_DAYS = Traits<ATMega16_RTC>::EPOCH_DAYS;

public:
    ATMega16_RTC() {}

    static Date date();
    static void date(const Date & d);

    static Second seconds_since_epoch() { 
	return date().to_offset(EPOCH_DAYS); 
    }
};

__END_SYS

#endif
