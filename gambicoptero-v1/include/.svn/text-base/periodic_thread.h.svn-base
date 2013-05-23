// EPOS Periodic Thread Abstraction Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __periodic_thread_h
#define __periodic_thread_h

#include <utility/handler.h>
#include <thread.h>
#include <alarm.h>

__BEGIN_SYS

// Aperiodic Thread
typedef Thread Aperiodic_Thread;

// Periodic Thread
class Periodic_Thread: public Thread
{
protected:
    typedef RTC::Microsecond Microsecond;

public:
    Periodic_Thread(int (* entry)(), 
		    const Microsecond & period,
		    int times = Alarm::INFINITE,
		    const State & state = READY,
		    unsigned int stack_size = STACK_SIZE)
	: Thread(entry, BEGINNING, period, stack_size),
	  _semaphore(0),
	  _handler(&_semaphore),
	  _alarm(period, &_handler, times)
    {
	if((state == READY) || (state == RUNNING)) {
	    _state = SUSPENDED;
	    resume();
	} else
	    _state = state;
    }

    template<class T1>
    Periodic_Thread(int (* entry)(T1 a1), T1 a1,
		    const Microsecond & period,
		    int times = Alarm::INFINITE,
		    const State & state = READY,
		    unsigned int stack_size = STACK_SIZE)
	: Thread(entry, a1, BEGINNING, period, stack_size),
	  _semaphore(0),
	  _handler(&_semaphore),
	  _alarm(period, &_handler, times) 
    {
	if((state == READY) || (state == RUNNING)) {
	    _state = SUSPENDED;
	    resume();
	} else
	    _state = state;
    }

    template<class T1, class T2>
    Periodic_Thread(int (* entry)(T1 a1, T2 a2), T1 a1, T2 a2, 
		    const Microsecond & period,
		    int times = Alarm::INFINITE,
		    const State & state = READY,
		    unsigned int stack_size = STACK_SIZE)
	: Thread(entry, a1, a2, BEGINNING, period, stack_size),
	  _semaphore(0),
	  _handler(&_semaphore),
	  _alarm(period, &_handler, times)

    {
	if((state == READY) || (state == RUNNING)) {
	    _state = SUSPENDED;
	    resume();
	} else
	    _state = state;
    }

    template<class T1, class T2, class T3>
    Periodic_Thread(int (* entry)(T1 a1, T2 a2, T3 a3), T1 a1, T2 a2, T3 a3,
		    const Microsecond & period,
		    int times = Alarm::INFINITE,
		    const State & state = READY,
		    unsigned int stack_size = STACK_SIZE)
	: Thread(entry, a1, a2, a3, BEGINNING, period, stack_size),
	  _semaphore(0),
	  _handler(&_semaphore),
	  _alarm(period, &_handler, times)
    {
	if((state == READY) || (state == RUNNING)) {
	    _state = SUSPENDED;
	    resume();
	} else
	    _state = state;
    }

    static void wait_next() {
	reinterpret_cast<Periodic_Thread *>(self())->_semaphore.p();
    }

private:
    Semaphore _semaphore;
    Semaphore_Handler _handler;
    Alarm _alarm;
};

__END_SYS

#endif
