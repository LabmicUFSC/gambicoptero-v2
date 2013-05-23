// EPOS Alarm Abstraction Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __alarm_h
#define __alarm_h

#include <utility/queue.h>
#include <utility/handler.h>
#include <tsc.h>
#include <rtc.h>
#include <timer.h>
#include <semaphore.h>

__BEGIN_SYS

class Alarm
{
private:
    typedef TSC::Time_Stamp Time_Stamp;
    typedef Timer::Tick Tick;

    static const bool smp = Traits<Thread>::smp;
    static const bool idle_waiting = Traits<Alarm>::idle_waiting;

    typedef Relative_Queue<Alarm, Tick> Queue;

public:
    typedef TSC::Hertz Hertz;
    typedef RTC::Microsecond Microsecond;

    // Infinite times (for alarms)
    enum { INFINITE = -1 };

    Alarm(const Microsecond & time, Handler * handler, int times = 1);
    ~Alarm();

    static Hertz resolution() { return Alarm_Timer::FREQUENCY; }

    static void delay(const Microsecond & time);

    static int init();

private:
    static Microsecond period() {
        return 1000000 / resolution();
    }

    static Tick ticks(const Microsecond & time) {
        return (time + period() / 2) / period();
    }

    static void lock() {
        CPU::int_disable();
        if(smp)
            _lock.acquire();
    }

    static void unlock() {
        if(smp)
            _lock.release();
        CPU::int_enable();
    }

    static void handler();

private:
    Tick _ticks;
    Handler * _handler;
    int _times;
    Queue::Element _link;

    static Spin _lock;
    static Alarm_Timer * _timer;
    static volatile Tick _elapsed;
    static Queue _requests;
};


class Delay
{
private:
    typedef RTC::Microsecond Microsecond;

public:
    Delay(const Microsecond & time): _time(time) { repeat(); }

    void repeat() { Alarm::delay(_time); }

private:
    Microsecond _time;
};


template<typename Timer>
class Private_Alarm: private Timer
{
public:
    typedef TSC::Hertz Hertz;
    typedef RTC::Microsecond Microsecond;

public:
    Private_Alarm(const Microsecond & time, 
                  Handler * handler, 
                  bool retrigger = true) 
    : Timer(1000000 / time, handler, retrigger)
    {
        db<Alarm>(TRC) << "Alarm(t=" << time
                       << ",h=" << (void *)handler
                       << ",r=" << retrigger << ") => " << this << "\n";
    }

    ~Private_Alarm() {
        db<Alarm>(TRC) << "~Alarm()\n";
    }

    static Hertz resolution() { return Timer::FREQUENCY; }
};

__END_SYS

#endif
