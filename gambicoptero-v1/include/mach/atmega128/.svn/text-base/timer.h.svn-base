// EPOS ATMega128 Timer Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega128_timer_h
#define __atmega128_timer_h

#include "../avr_common/timer.h"
#include <rtc.h>
#include <ic.h>

__BEGIN_SYS

class ATMega128_Timer_1: public Timer_Common, private AVR_Timer
{
public:
    static const unsigned int FREQUENCY = Traits<ATMega128_Timer_1>::FREQUENCY;
private:
    static const Hertz MACHINE_CLOCK = Traits<Machine>::CLOCK;
    static const unsigned int CLOCK = Traits<Machine>::CLOCK >> 10;

    typedef RTC::Microsecond Microsecond;

public:
    // Register Settings
    enum {
    TIMER_PRESCALE_1    = CSn0,
	TIMER_PRESCALE_8    = CSn1,
	TIMER_PRESCALE_32   = CSn1 | CSn0,
	TIMER_PRESCALE_64   = CSn2,
	TIMER_PRESCALE_128  = CSn2 | CSn0,
	TIMER_PRESCALE_256  = CSn2 | CSn1,
	TIMER_PRESCALE_1024 = CSn2 | CSn1 | CSn0
    };

public:
    ATMega128_Timer_1() {}

    ATMega128_Timer_1(const Hertz & f) {
	db<ATMega128_Timer_1>(TRC) << "ATMega128_Timer_1(f=" << f << ")\n";
	frequency(f);
    }

    ATMega128_Timer_1(const Microsecond & quantum,
                    const Handler * handler) {
    frequency(1000000 / quantum);
    IC::int_vector(IC::IRQ_TIMER, handler);
    IC::enable(IC::IRQ_TIMER);
    enable();
    }

    const void clock(const Hertz clock) {
        if(clock == MACHINE_CLOCK)
            tccr0(TIMER_PRESCALE_1|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 3))
            tccr0(TIMER_PRESCALE_8|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 6))
            tccr0(TIMER_PRESCALE_64|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 8))
            tccr0(TIMER_PRESCALE_256|WGMn2);
        else
            tccr0(TIMER_PRESCALE_1024|WGMn2);
    }

    Hertz frequency() const { return count2freq(ocr0()); }
    void frequency(const Hertz & f) {
	ocr0(freq2count(f));
    clock(CLOCK);
    };

    void reset() { tcnt0(0); }

    void enable(){ timsk(timsk() | OCIE0); }
    void disable(){ timsk(timsk() & ~OCIE0); }

    Tick read() { return tcnt0(); }

    static int init();

protected:
    static Hertz count2freq(const Count & c) { return CLOCK / c; }
    static Count freq2count(const Hertz & f) { return CLOCK / f; }
};


class ATMega128_Timer_2: public Timer_Common, private AVR_Timer
{
public:
    static const unsigned int FREQUENCY = Traits<ATMega128_Timer_2>::FREQUENCY;
private:
    static const Hertz MACHINE_CLOCK = Traits<Machine>::CLOCK;
    static const unsigned int CLOCK = Traits<Machine>::CLOCK >> 10;

    typedef RTC::Microsecond Microsecond;

public:
    ATMega128_Timer_2() {}

    ATMega128_Timer_2(const Hertz & f) {
	frequency(f);
    }

    ATMega128_Timer_2(const Handler * handler) {
    frequency(FREQUENCY);
    IC::int_vector(IC::IRQ_TIMER2_COMP, handler);
    IC::enable(IC::IRQ_TIMER2_COMP);
    enable();
    }

    Hertz frequency() const { return count2freq(ocr2()); }
    void frequency(const Hertz & f) {
	ocr2(freq2count(f));
	clock(CLOCK);
    };

    const void clock(const Hertz clock) {
        if(clock == MACHINE_CLOCK)
            tccr2(TIMER_PRESCALE_1|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 3))
            tccr2(TIMER_PRESCALE_8|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 6))
            tccr2(TIMER_PRESCALE_64|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 8))
            tccr2(TIMER_PRESCALE_256|WGMn2);
        else
            tccr2(TIMER_PRESCALE_1024|WGMn2);
    }

    void reset() { tcnt2(0); }

    void enable(){ timsk(timsk() | OCIE2); }
    void disable(){ timsk(timsk() & ~OCIE2); }

    Tick read() { return tcnt2(); }

    static int init();

protected:
    static Hertz count2freq(const Count & c) { return CLOCK / c; }
    static Count freq2count(const Hertz & f) { return CLOCK / f; }
};



class ATMega128_Timer_3: public Timer_Common, private AVR_Timer
{
public:
    static const unsigned int FREQUENCY = Traits<ATMega128_Timer_3>::FREQUENCY;
private:
    static const Hertz MACHINE_CLOCK = Traits<Machine>::CLOCK;
    static const unsigned int CLOCK = Traits<Machine>::CLOCK >> 10;

    typedef RTC::Microsecond Microsecond;

public:
    ATMega128_Timer_3() {}

    ATMega128_Timer_3(const Hertz & f) {
	frequency(f);
    }

    ATMega128_Timer_3(const Handler * handler) {
    frequency(FREQUENCY);
    IC::int_vector(IC::IRQ_TIMER3_COMPA, handler);
    IC::enable(IC::IRQ_TIMER3_COMPA);
    enable();
    }

    const void clock(const Hertz clock) {
        if(clock == MACHINE_CLOCK)
            tccr3b(TIMER_PRESCALE_1|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 3))
            tccr3b(TIMER_PRESCALE_8|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 6))
            tccr3b(TIMER_PRESCALE_64|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 8))
            tccr3b(TIMER_PRESCALE_256|WGMn2);
        else
            tccr3b(TIMER_PRESCALE_1024|WGMn2);
    }

    void enable(){ etimsk(etimsk() | OCIE3A); }
    void disable(){etimsk(etimsk() & ~OCIE3A);}

    Hertz frequency() const { return count2freq(ocr3a());}
    void frequency(const Hertz & f) {
	ocr3a(freq2count(f));
    clock(CLOCK);
    };

    void reset() { tcnt3(0); }
    Tick read() { return tcnt3(); }

    static int init();

protected:
    static Hertz count2freq(const Count & c) { return CLOCK / c; }
    static Count freq2count(const Hertz & f) { return CLOCK / f; }

};

__END_SYS

#endif
