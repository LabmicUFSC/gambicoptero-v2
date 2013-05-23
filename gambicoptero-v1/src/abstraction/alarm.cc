// EPOS Alarm Abstraction Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <semaphore.h>
#include <alarm.h>

__BEGIN_SYS

// Class attributes
Spin Alarm::_lock;
Alarm_Timer * Alarm::_timer;
volatile Alarm::Tick Alarm::_elapsed;
Alarm::Queue Alarm::_requests;

// Methods
Alarm::Alarm(const Microsecond & time, Handler * handler, int times):
_ticks(ticks(time)), _handler(handler),
_times(times), _link(this, _ticks)
{
    lock();

    db<Alarm>(TRC) << "Alarm(t=" << time
    << ",tk=" << _ticks
    << ",h=" << (void *)handler
    << ",x=" << times << ") => " << this << "\n";

    if(_ticks) {
	_requests.insert(&_link);
	unlock();
    } else {
	unlock();
	(*handler)();
    }
}

Alarm::~Alarm()
{
    lock();

    db<Alarm>(TRC) << "~Alarm()\n";

    _requests.remove(this);

    unlock();
}

// Class methods
void Alarm::delay(const Microsecond & time)
{
    db<Alarm>(TRC) << "Alarm::delay(time=" << time << ")\n";

    if(idle_waiting) {

	Semaphore semaphore(0);
	Semaphore_Handler handler(&semaphore);
	Alarm alarm(time, &handler, 1); // if time < tick trigger v()
	semaphore.p();

    } else {

	Tick t = _elapsed + ticks(time);
	while(_elapsed < t);

    }
}

void Alarm::handler()
{
    CPU::int_disable();
    // lock(); this handler is meant to be called obly by CPU[0]

    _elapsed++;

    Alarm * alarm = 0;

    if(!_requests.empty()) {
	// rank can be negative whenever multiple handlers get created for the same time tick
	if(_requests.head()->promote() <= 0) {

	    Queue::Element * e = _requests.remove();
	    alarm = e->object();

	    if(alarm->_times != INFINITE)
		alarm->_times--;
	    if(alarm->_times) {
		e->rank(alarm->_ticks);
		_requests.insert(e);
	    }
	}
    }

    // unlock();
    CPU::int_enable();

    if(alarm) {
	db<Alarm>(TRC) << "Alarm::handler(h=" << reinterpret_cast<void*>(alarm->handler) << ")\n";
	(*alarm->_handler)();
    }
}

__END_SYS
