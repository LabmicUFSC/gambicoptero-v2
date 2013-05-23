// EPOS PLASMA Timer Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __plasma_timer_h
#define __plasma_timer_h

#include <cpu.h>
#include <ic.h>
#include <rtc.h>
#include <timer.h>

__BEGIN_SYS

class PLASMA_Timer:  public Timer_Common
{
protected:
    typedef CPU::Reg32 Count;

    // Plasma Timer private imports, types and constants
    static const unsigned int FREQUENCY = Traits<PLASMA_Timer>::FREQUENCY;
    static const unsigned int CHANNELS = 3;

public:
  typedef int Channel;
  enum {
    SCHEDULER,
    ALARM,
	TEST
  };
 
public:
    PLASMA_Timer(const Hertz & frequency,
                 const Handler * handler,
                 const Channel & channel):
        _initial(FREQUENCY / frequency), _current(_initial), 
	    _channel(channel), _handler(handler)
    {
      db<Timer>(TRC) << "Timer(f=" << frequency
        << ",h=" << reinterpret_cast<void*>(handler)
        << ",ch=" << channel 
        << ") => {count=" << _initial << "}\n";

      if(_initial && !_channels[channel]) 
         _channels[channel] = this;
      else
         db<Timer>(ERR) << "Timer not installed!\n";
    }

    ~PLASMA_Timer() {}

    void frequency(const Hertz & f) {
		_initial = FREQUENCY / f;
        reset();
        db<PLASMA_Timer>(TRC) << "PLASMA_Timer::frequency(freq=" << frequency()
		                             << ",cnt=" << (void*)_initial << ")\n";
    }

	Hertz frequency() {  return FREQUENCY / _initial; }

    void enable() {}

    void disable() {}

    void reset() { _current = _initial; }

    static void int_handler(unsigned int interrupt);

    static void init();

protected:
    // Plasma_Timer attributes
    Count _initial;
    volatile Count _current;
    unsigned int _channel;
    Handler * _handler;
    static PLASMA_Timer * _channels[CHANNELS];

};


// Timer used by Alarm
class Alarm_Timer: public PLASMA_Timer
{
  public:
    static const unsigned int FREQUENCY = Timer::FREQUENCY;

  public:
    Alarm_Timer(const Handler * handler):
      PLASMA_Timer(FREQUENCY, handler, ALARM) {}
};


// Timer used by Thread::Scheduler
class Scheduler_Timer: public PLASMA_Timer
{
  private:
    typedef RTC::Microsecond Microsecond;

  public:
    Scheduler_Timer(const Microsecond & quantum, const Handler * handler): 
      PLASMA_Timer(1000000 / quantum, handler, SCHEDULER) {}
};


__END_SYS

#endif
