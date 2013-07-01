// EPOS Alarm Abstraction Declarations

#ifndef __alarm_h
#define __alarm_h

#include <utility/queue.h>
#include <utility/handler.h>
#include <tsc.h>
#include <rtc.h>
#include <timer.h>
#include <semaphore.h>

__BEGIN_SYS

class Alarm_Base
{
public:
    typedef TSC::Time_Stamp Time_Stamp;
    typedef Timer::Tick Tick;  
    
    static const bool smp = Traits<Thread>::smp;
    
public:
    typedef TSC::Hertz Hertz;
    typedef RTC::Microsecond Microsecond;

    // Infinite times (for alarms)
    enum { INFINITE = -1 };
    
     static Hertz resolution() { return Alarm_Timer::FREQUENCY; }
     
public:
    Alarm_Base(const Microsecond & time, Handler * handler, int times) :
    _ticks(ticks(time)), _handler(handler), _times(times) { }
  
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
    
public:
    Tick _ticks;
    Handler * _handler;
    int _times; 
    static Spin _lock;
  
};

class Single_Core_Alarm : public Alarm_Base
{
private:
    static const bool idle_waiting = Traits<Single_Core_Alarm>::idle_waiting;

    typedef Relative_Queue<Single_Core_Alarm, Tick> Queue;

public:    
    Single_Core_Alarm(const Microsecond & time, Handler * handler, int times = 1);
    ~Single_Core_Alarm();

    static void delay(const Microsecond & time);

    static int init();

private:
    static void handler();

private:
    Queue::Element _link; 
  
    static Alarm_Timer * _timer;
    static volatile Tick _elapsed;
    static Queue _requests;
};

class SMP_Alarm : public Alarm_Base
{
private:
    static const bool idle_waiting = Traits<SMP_Alarm>::idle_waiting;
    static const unsigned int MAX_CPUS = Traits<Machine>::MAX_CPUS;
    static const unsigned int MAX_ELAPSED = 100000;
	static const unsigned int MAX_ALARMS = 10;
    
    typedef Relative_Queue<SMP_Alarm, Tick> Queue;  

public:
    SMP_Alarm(const Microsecond & time, Handler * handler, int times = 1);
    ~SMP_Alarm();

    static void delay(const Microsecond & time);

    static int init();

private:
    static void handler();
    static volatile Tick _elapsed[MAX_CPUS];
private:
    Queue::Element _link; 
  
    // only one timer is needed because the timer interrupt is shared among cores
    // and the "tick" couting is done by the SMP_Alarm::handler()
    static Alarm_Timer * _timer; 
    static Queue _requests[MAX_CPUS];
    static int _lowest_priority_queue;
};

//Define the Alarm that will be used by the rest of the system
typedef IF<Traits<Thread>::smp, 
          SMP_Alarm, 
          Single_Core_Alarm>::Result Alarm;

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

__END_SYS

#endif
