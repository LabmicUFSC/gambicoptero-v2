// EPOS Thread Abstraction Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <system.h>
#include <thread.h>
#include <alarm.h>

__BEGIN_SYS

void Thread::init()
{
    int (* entry)() =
	reinterpret_cast<int (*)()>(System::info()->lmm.app_entry);

    db<Init, Thread>(TRC) << "Thread::init(entry=" << (void *)entry << ")\n";

    Machine::smp_barrier();

    Thread * first;
    if(Machine::cpu_id() == 0) {
	// Create the application's main thread
	// This must precede idle, thus avoiding implicit rescheduling
	first = new(kmalloc(sizeof(Thread))) Thread(entry, RUNNING, MAIN);
	new(kmalloc(sizeof(Thread))) Thread(&idle, READY, IDLE);
    } else 
	first = new(kmalloc(sizeof(Thread))) Thread(&idle, READY, IDLE);

    Machine::smp_barrier();

    if(Machine::cpu_id() == 0) {
	if(active_scheduler)
	    _timer = new (kmalloc(sizeof(Scheduler_Timer))) 
		Scheduler_Timer(QUANTUM, &time_slicer);
    } 

    db<Init, Thread>(INF) << "Dispatching the first thread: " << first
			  << "\n";

    This_Thread::not_booting();

    first->_context->load();
}

__END_SYS
