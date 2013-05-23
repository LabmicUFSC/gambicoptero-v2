// EPOS Thread Abstraction Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <machine.h>
#include <thread.h>
#include <scheduler.h>
#include <alarm.h>

__BEGIN_SYS

// Class attributes
Spin Thread::_lock;
unsigned int Thread::_thread_count;
Scheduler<Thread> Thread::_scheduler;
Scheduler_Timer * Thread::_timer;

// This_Thread class attributes
bool This_Thread::_not_booting;

// Methods
void Thread::common_constructor(Log_Addr entry, unsigned int stack_size) 
{
    db<Thread>(TRC) << "Thread(entry=" << (void *)entry 
		    << ",state=" << _state
		    << ",rank=" << _link.rank()
		    << ",stack={b=" << _stack
		    << ",s=" << stack_size
		    << "},context={b=" << _context
		    << "," << *_context << "}) => " << this << "\n";

    _thread_count++;

    _scheduler.insert(this);
    if((_state != READY) && (_state != RUNNING))
	_scheduler.suspend(this);

    reschedule();
}

Thread::~Thread()
{
    lock();

    db<Thread>(TRC) << "~Thread(this=" << this 
		    << ",state=" << _state
		    << ",rank=" << _link.rank()
		    << ",stack={b=" << _stack
		    << ",context={b=" << _context
		    << "," << *_context << "})\n";

    switch(_state) {
    case BEGINNING:
	_scheduler.resume(this);
	_thread_count--;
	break;
    case RUNNING:  // Self deleted itself!
	exit(-1); 
	break;
    case READY:
	_thread_count--;
	break;
    case SUSPENDED: 
	_scheduler.resume(this);
	_thread_count--;
	break;
    case WAITING:
	_waiting->remove(this);
	_scheduler.resume(this);
	_thread_count--;
	break;
    case FINISHING: // Already called exit()
	break;
    }
    
    _scheduler.remove(this);
    
    unlock();

    kfree(_stack);
}

void Thread::priority(const Priority & p)
{
    lock();

    db<Thread>(TRC) << "Thread::priority(this=" << this
		    << ",prio=" << p << ")\n";

    _scheduler.remove(this);
    _link.rank(int(p));
    _scheduler.insert(this);

    reschedule();
}

int Thread::join()
{
    lock();

    db<Thread>(TRC) << "Thread::join(this=" << this
		    << ",state=" << _state << ")\n";

    if(_state != FINISHING) {
	_joining = running();
	_joining->suspend(true);
    } else
	unlock();

    return *static_cast<int *>(_stack);
}

void Thread::pass()
{
    lock();

    db<Thread>(TRC) << "Thread::pass(this=" << this << ")\n";

    Thread * prev = running();
    Thread * next = _scheduler.choose(this);

    if(next)
	dispatch(prev, next, false);
    else {
 	db<Thread>(WRN) << "Thread::pass => thread (" << this 
 			<< ") not ready\n";
	unlock();
    }
}

void Thread::suspend(bool locked)
{
    if(!locked)
	lock();

    db<Thread>(TRC) << "Thread::suspend(this=" << this << ")\n";

    Thread * prev = running();

    _scheduler.suspend(this);
    _state = SUSPENDED;

    Thread * next = running();

    dispatch(prev, next);

}	    

void Thread::resume()
{
    lock();

    db<Thread>(TRC) << "Thread::resume(this=" << this << ")\n";

    if(_state == SUSPENDED) {
	_state = READY;
	_scheduler.resume(this);
    } else
	db<Thread>(WRN) << "Resume called for unsuspended object!\n";

    reschedule();
}


// Class methods

void Thread::yield()
{
    lock();

    db<Thread>(TRC) << "Thread::yield(running=" << running() << ")\n";
	
    Thread * prev = running();
    Thread * next = _scheduler.choose_another();

    dispatch(prev, next);
}

void Thread::exit(int status)
{
    lock();

    db<Thread>(TRC) << "Thread::exit(running=" << running() 
		    <<",status=" << status << ")\n";

    Thread * thr = running();
    _scheduler.remove(thr);
    *static_cast<int *>(thr->_stack) = status;
    thr->_state = FINISHING;

    _thread_count--;

    if(thr->_joining) {
	thr->_joining->_state = READY;
	_scheduler.resume(thr->_joining);
	thr->_joining = 0;
    }

    dispatch(thr, _scheduler.choose());
}

void Thread::sleep(Queue * q)
{
    db<Thread>(TRC) << "Thread::sleep(running=" << running()
		    << ",q=" << q << ")\n";

    Thread * thr = running();

    _scheduler.suspend(thr);
    thr->_state = WAITING;
    q->insert(&thr->_link);
    thr->_waiting = q;

    dispatch(thr, _scheduler.chosen());
}

void Thread::wakeup(Queue * q) 
{
    db<Thread>(TRC) << "Thread::wakeup(running=" << running()
		    << ",q=" << q << ")\n";

    if(!q->empty()) {
	Thread * t = q->remove()->object();
	t->_state = READY;
	t->_waiting = 0;
	_scheduler.resume(t);
    }

    reschedule();
}

void Thread::wakeup_all(Queue * q) 
{
    db<Thread>(TRC) << "Thread::wakeup_all(running=" << running()
		    << ",q=" << q << ")\n";

    while(!q->empty()) {
	Thread * t = q->remove()->object();
	t->_state = READY;
	t->_waiting = 0;
	_scheduler.resume(t);
    }

    reschedule();
}

void Thread::reschedule(bool preempt)
{
    if(preempt) {
	db<Thread>(TRC) << "Thread::reschedule()\n";
    
	Thread * prev = running();
	Thread * next = _scheduler.choose();
	
	dispatch(prev, next);
    } else 
	unlock();
}

void Thread::time_slicer() 
{
    lock();

    reschedule(true);
}

void Thread::implicit_exit() 
{
    exit(CPU::fr()); 
}

int Thread::idle()
{
    while(true) {
	if(Traits<Thread>::trace_idle)
	    db<Thread>(TRC) << "Thread::idle()\n";

	if(_thread_count <= Machine::n_cpus()) {
	    CPU::int_disable();
	    if(Machine::cpu_id() == 0) {
		db<Thread>(WRN) << "The last thread has exited!\n";
		db<Thread>(WRN) << "Rebooting the machine ...\n";
		Machine::reboot();
	    } else
		CPU::halt();
	}

	if(energy_aware)
	    if((_scheduler.schedulables() > 1) /* && enough_energy()*/)
		yield();
	    else
		CPU::halt();
	else {
	    CPU::halt();
	    if(_scheduler.schedulables() > 1)
		yield();
	}
    }

    return 0;
}

// Id forwarder to the spin lock
unsigned int This_Thread::id() 
{ 
    return _not_booting ?
	reinterpret_cast<unsigned int>(Thread::self()) :
	Machine::cpu_id() + 1;
}

__END_SYS
