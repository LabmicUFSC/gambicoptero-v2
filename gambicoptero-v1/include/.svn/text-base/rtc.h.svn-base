// EPOS RTC Mediator Common Package

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __rtc_h
#define __rtc_h

#include <system/config.h>
#include <utility/debug.h>

__BEGIN_SYS

class RTC_Common
{
protected:
    RTC_Common() {}

public:
    // The time (as defined by God Chronos)
    typedef unsigned long Microsecond;
    typedef unsigned long Second;

    // Calendar date and time
    class Date {
    public:
	Date() {}
	Date(unsigned int Y, unsigned int M, unsigned int D,
	     unsigned int h, unsigned int m, unsigned int s) 
	    : _Y(Y), _M(M), _D(D), _h(h), _m(m), _s(s) {}
	Date(const Second & seconds, unsigned long epoch_days = 0);
 
	operator Second() const { return to_offset(); }
	Second to_offset(unsigned long epoch_days = 0) const;

	unsigned int year() const { return _Y; };
	unsigned int month() const { return _M; };
	unsigned int day() const { return _D; };
	unsigned int hour() const { return _h; };
	unsigned int minute() const { return _m; };
	unsigned int second() const { return _s; };

	void adjust_year(int y) { _Y += y; };

	friend Debug & operator << (Debug & db, const Date & d) {
	    db << "{" << d._Y << "/" << d._M << "/" << d._D << " "
	       << d._h << ":" << d._m << ":" << d._s << "}";
	    return db;
	}

    private:
	unsigned int _Y;
	unsigned int _M;
	unsigned int _D;
	unsigned int _h;
	unsigned int _m;
	unsigned int _s;
    };
};

__END_SYS

#ifdef __RTC_H
#include __RTC_H
#endif

#endif
