// EPOS Mica2_Timer Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __at90can128_timer_h
#define __at90can128_timer_h

#include "../avr_common/timer.h"
#include <rtc.h>
#include <ic.h>

__BEGIN_SYS

class AT90CAN128_Timer_1: public Timer_Common, private AVR_Timer
{
public:
    static const unsigned int FREQUENCY = Traits<AT90CAN128_Timer_1>::FREQUENCY;
private:
    static const Hertz MACHINE_CLOCK = Traits<Machine>::CLOCK;
    static const unsigned int CLOCK = Traits<Machine>::CLOCK >> 10;

    typedef RTC::Microsecond Microsecond;

public:

    // Register Settings
    enum {
    TIMER_PRESCALE_1    = CSn0,
    TIMER_PRESCALE_8    = CSn1,
    TIMER_PRESCALE_64   = CSn1 | CSn0,
    TIMER_PRESCALE_256  = CSn2,
    TIMER_PRESCALE_1024 = CSn2 | CSn0
    };

public:

    AT90CAN128_Timer_1() {}

    AT90CAN128_Timer_1(const Hertz & f) {
    db<AT90CAN128_Timer_1>(TRC) << "AT90CAN128_Timer_1(f=" << f << ")\n";
    frequency(f);
    }

    AT90CAN128_Timer_1(const Handler * handler) {
    frequency(1000000 / 10000);
    IC::int_vector(IC::IRQ_TIMER, handler);
    IC::enable(IC::IRQ_TIMER);
    enable();
    }

    AT90CAN128_Timer_1(const Microsecond & quantum,
                    const Handler * handler) {
    frequency(1000000 / quantum);
    IC::int_vector(IC::IRQ_TIMER, handler);
    IC::enable(IC::IRQ_TIMER);
    enable();
    }

    const void clock(const Hertz clock) {
        if(clock == MACHINE_CLOCK)
            tccr0a(TIMER_PRESCALE_1|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 3))
            tccr0a(TIMER_PRESCALE_8|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 6))
            tccr0a(TIMER_PRESCALE_64|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 8))
            tccr0a(TIMER_PRESCALE_256|WGMn2);
        else
            tccr0a(TIMER_PRESCALE_1024|WGMn2);
    }

    Hertz frequency() const { return count2freq(ocr0a()); }
    void frequency(const Hertz & f) {
    ocr0a(freq2count(f));
    clock(CLOCK);
    };

    void reset() { tcnt0(0); }

    void enable(){ timsk0(timsk0() | OCIE0A); }
    void disable(){ timsk0(timsk0() & ~OCIE0A); }

    Tick read() { return tcnt0(); }

    static int init();

protected:
    static Hertz count2freq(const Count & c) { return CLOCK / c; }
    static Count freq2count(const Hertz & f) { return CLOCK / f; }
};

class AT90CAN128_Timer_2: public Timer_Common, private AVR_Timer
{
public:
    static const unsigned int FREQUENCY = Traits<AT90CAN128_Timer_2>::FREQUENCY;
private:
    static const Hertz MACHINE_CLOCK = Traits<Machine>::CLOCK;
    static const unsigned int CLOCK = Traits<Machine>::CLOCK >> 10;

    typedef RTC::Microsecond Microsecond;

public:

    // Register Settings
    enum {
    TIMER_PRESCALE_1    = CSn0,
    TIMER_PRESCALE_8    = CSn1,
    TIMER_PRESCALE_64   = CSn1 | CSn0,
    TIMER_PRESCALE_256  = CSn2,
    TIMER_PRESCALE_1024 = CSn2 | CSn0
    };

public:

    AT90CAN128_Timer_2() {}

    AT90CAN128_Timer_2(const Hertz & f) {
    db<AT90CAN128_Timer_2>(TRC) << "AT90CAN128_Timer_2(f=" << f << ")\n";
    frequency(f);
    }

    AT90CAN128_Timer_2(const Handler * handler) {
    frequency(FREQUENCY);
    IC::int_vector(IC::IRQ_TIMER2_COMP, handler);
    IC::enable(IC::IRQ_TIMER2_COMP);
    enable();
    }

    AT90CAN128_Timer_2(const Microsecond & quantum,
                    const Handler * handler) {
    frequency(1000000 / quantum);
    IC::int_vector(IC::IRQ_TIMER2_COMP, handler);
    IC::enable(IC::IRQ_TIMER2_COMP);
    enable();
    }

    const void clock(const Hertz clock) {
        if(clock == MACHINE_CLOCK)
            tccr2a(TIMER_PRESCALE_1|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 3))
            tccr2a(TIMER_PRESCALE_8|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 6))
            tccr2a(TIMER_PRESCALE_64|WGMn2);
        else if (clock == (MACHINE_CLOCK >> 8))
            tccr2a(TIMER_PRESCALE_256|WGMn2);
        else
            tccr2a(TIMER_PRESCALE_1024|WGMn2);
    }

    Hertz frequency() const { return count2freq(ocr2a()); }
    void frequency(const Hertz & f) {
    ocr2a(freq2count(f));
    clock(CLOCK);
    };

    void reset() { tcnt2(0); }

    void enable(){ timsk2(timsk2() | OCIE2A); }
    void disable(){ timsk2(timsk2() & ~OCIE2A); }

    Tick read() { return tcnt2(); }

    static int init();

protected:
    static Hertz count2freq(const Count & c) { return CLOCK / c; }
    static Count freq2count(const Hertz & f) { return CLOCK / f; }
};


class AT90CAN128_Timer_3: public Timer_Common, private AVR_Timer
{
public:
    static const unsigned int FREQUENCY = Traits<AT90CAN128_Timer_3>::FREQUENCY;
private:
    static const Hertz MACHINE_CLOCK = Traits<Machine>::CLOCK;
    static const unsigned int CLOCK = Traits<Machine>::CLOCK >> 8;

    typedef RTC::Microsecond Microsecond;

public:

    // Register Settings
    enum {
    TIMER_PRESCALE_1    = CSn0,
    TIMER_PRESCALE_8    = CSn1,
    TIMER_PRESCALE_64   = CSn1 | CSn0,
    TIMER_PRESCALE_256  = CSn2,
    TIMER_PRESCALE_1024 = CSn2 | CSn0
    };

public:

    AT90CAN128_Timer_3() {}

    AT90CAN128_Timer_3(const Hertz & f) {
    db<AT90CAN128_Timer_3>(TRC) << "AT90CAN128_Timer_3(f=" << f << ")\n";
    frequency(f);
    }

    AT90CAN128_Timer_3(const Handler * handler) {
    frequency(FREQUENCY);
    IC::int_vector(IC::IRQ_TIMER3_COMPA, handler);
    IC::enable(IC::IRQ_TIMER3_COMPA);
    enable();
    }

    AT90CAN128_Timer_3(const Microsecond & quantum,
                    const Handler * handler) {
    frequency(1000000 / quantum);
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

    Hertz frequency() const { return count2freq(ocr3a()); }
    void frequency(const Hertz & f) {
    ocr3a(freq2count(f));
    clock(CLOCK);
    };

    void reset() { tcnt3(0); }

    void enable(){ timsk3(timsk3() | OCIE3A); }
    void disable(){ timsk3(timsk3() & ~OCIE3A); }

    Tick read() { return tcnt3(); }

    static int init();

protected:
    static Hertz count2freq(const Count & c) { return CLOCK / c; }
    static Count freq2count(const Hertz & f) { return CLOCK / f; }
};

__END_SYS

#endif
