// EPOS PC_RTC Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <display.h>
#include <rtc.h>

const unsigned int TEST_DURATION = 20; // s

__USING_SYS

int main()
{
    OStream cout;

    cout << "PC_RTC test\n";

    RTC rtc;
    RTC::Second t0 = rtc.seconds_since_epoch();

    cout << "It's now " << t0 << " seconds since epoch.\n";

    RTC::Date last_date = rtc.date();
    while(rtc.seconds_since_epoch() < t0 + TEST_DURATION) {
	if(last_date.second() != rtc.date().second()) {
	    last_date = rtc.date();
	    Display::position(20, 30);
	    cout << last_date.day() << '/'
		 << last_date.month() << '/'
		 << last_date.year() << ' ';
	    Display::position(20, 40);
	    cout << last_date.hour() << ':'
		 << last_date.minute() << ':'
		 << last_date.second() << "    ";
	}
    }

    cout << "\n\nSetting the time to its current value: ";
    rtc.date(rtc.date());
    cout << "done!\n";

    cout << "The End!\n";

    return 0;
}
