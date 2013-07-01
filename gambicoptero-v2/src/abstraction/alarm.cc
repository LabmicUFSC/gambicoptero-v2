// EPOS Alarm Abstraction Implementation

#include <system/kmalloc.h>
#include <semaphore.h>
#include <alarm.h>

__BEGIN_SYS

// Class attributes
Spin Alarm_Base::_lock;
Alarm_Timer * Single_Core_Alarm::_timer;
volatile Single_Core_Alarm::Tick Single_Core_Alarm::_elapsed;
Single_Core_Alarm::Queue Single_Core_Alarm::_requests;

// Methods
Single_Core_Alarm::Single_Core_Alarm(const Microsecond & time, Handler * handler, int times):
Alarm_Base(time, handler, times), _link(this, _ticks)
{
    lock();

    db<Single_Core_Alarm>(TRC) << "Alarm(t=" << time
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

Single_Core_Alarm::~Single_Core_Alarm()
{
    lock();

    db<Single_Core_Alarm>(TRC) << "~Single_Core_Alarm()\n";

    _requests.remove(this);

    unlock();
}

// Class methods
void Single_Core_Alarm::delay(const Microsecond & time)
{
    db<Single_Core_Alarm>(TRC) << "Single_Core_Alarm::delay(time=" << time << ")\n";

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

void Single_Core_Alarm::handler()
{
    CPU::int_disable();
    // lock(); this handler is meant to be called obly by CPU[0]

    _elapsed++;

    Single_Core_Alarm * alarm = 0;

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
	db<Single_Core_Alarm>(TRC) << "Single_Core_Alarm::handler(h=" << reinterpret_cast<void*>(alarm->handler) << ")\n";
	(*alarm->_handler)();
    }
}

// Class attributes
Alarm_Timer * SMP_Alarm::_timer;
volatile SMP_Alarm::Tick SMP_Alarm::_elapsed[MAX_CPUS];
SMP_Alarm::Queue SMP_Alarm::_requests[MAX_CPUS];
int SMP_Alarm::_lowest_priority_queue = 0;

// Methods
SMP_Alarm::SMP_Alarm(const Microsecond & time, Handler * handler, int times):
Alarm_Base(time, handler, times), _link(this, _ticks)
{
    lock();
    
    db<SMP_Alarm>(TRC) << "SMP_Alarm(t=" << time
    << ",tk=" << _ticks
    << ",h=" << (void *)handler
    << ",x=" << times << ") => " << this << "\n";

    if(_ticks) {
    _requests[(_lowest_priority_queue++ % MAX_CPUS)].insert(&_link);
    unlock();
    } else {
    unlock();
    (*handler)();
    }
}

SMP_Alarm::~SMP_Alarm()
{
    lock();
    
    db<SMP_Alarm>(TRC) << "~SMP_Alarm()\n";

    _requests[Machine::cpu_id()].remove(this);
    
    unlock();
}

// Class methods
void SMP_Alarm::delay(const Microsecond & time)
{
    db<SMP_Alarm>(TRC) << "SMP_Alarm::delay(time=" << time << ")\n";

    if(idle_waiting) {

    Semaphore semaphore(0);
    Semaphore_Handler handler(&semaphore);
    SMP_Alarm alarm(time, &handler, 1); // if time < tick trigger v()
    semaphore.p();

    } else {

    Tick t = _elapsed[Machine::cpu_id()] + ticks(time);
    while(_elapsed[Machine::cpu_id()] < t);

    }
}

void SMP_Alarm::handler()
{
    CPU::int_disable();
    lock();
    
    _elapsed[Machine::cpu_id()]++;
    
    SMP_Alarm * alarms[MAX_ALARMS];
    alarms[0] = 0;
    int i = 0;

    if(!_requests[Machine::cpu_id()].empty()) {
      _requests[Machine::cpu_id()].head()->promote();
      //release all tasks that have reached 0 ticks
      while(_requests[Machine::cpu_id()].head()->rank() <= 0) {
        Queue::Element * e = _requests[Machine::cpu_id()].remove();
        alarms[i] = e->object();
        if(alarms[i]->_times != INFINITE)
          alarms[i]->_times--;
        if(alarms[i]->_times) {
          e->rank(alarms[i]->_ticks);
          _requests[Machine::cpu_id()].insert(e);
        }
        i++;
        if(_requests[Machine::cpu_id()].empty())
          break;
      }
    }
    
    unlock();
    CPU::int_enable();

    while(i-- > 0) {
      (*alarms[i]->_handler)();
    }
}

__END_SYS
