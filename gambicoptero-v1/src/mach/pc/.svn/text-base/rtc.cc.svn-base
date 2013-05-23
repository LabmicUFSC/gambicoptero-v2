// EPOS PC RTC Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/pc/rtc.h>

__BEGIN_SYS

PC_RTC::Date PC_RTC::date()
{
    unsigned int tmp = reg(SECONDS);
    Date date(reg(YEAR), reg(MONTH), reg(DAY), 
	      reg(HOURS), reg(MINUTES), tmp);

    if(tmp != reg(SECONDS)) // RTC update in between?
	date = Date(reg(YEAR), reg(MONTH), reg(DAY), 
		    reg(HOURS), reg(MINUTES), reg(SECONDS));

    date.adjust_year(1900);
    if(date.year() < EPOCH_YEAR)
	date.adjust_year(100);

    db<RTC>(TRC) << "RTC::date() => " << date << "\n";

    return date;
}

void PC_RTC::date(const Date & d)
{
    db<RTC>(TRC) << "RTC::date(date= " << d << ")\n";

    reg(YEAR, d.year());
    reg(MONTH, d.month());
    reg(DAY, d.day());
    reg(HOURS, d.hour());
    reg(MINUTES, d.minute());
    reg(SECONDS, d.second());
}

__END_SYS
