// EPOS Thread Abstraction Initialization

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

    if(Machine::cpu_id() == 0) {
        if(global_scheduler) {
            // The interrupt handler method must be set before the creation of
            // the first thread, because the thread constructor may call a reschedule
            IC::int_vector(IC::INT_RESCHEDULER, (IC::Interrupt_Handler) &ipi_reschedule);
            IC::enable(IC::INT_RESCHEDULER);
        }
    
        // The installation of the scheduler timer handler must precede the
        // creation of threads, since the constructor can induce a reeschedule
        // and this in turn can call timer->reset()
        // Letting reschedule() happen during thread creating is harmless, since
        // MAIN is created first and dispatch won't replace it nor by itself
        // neither by IDLE (that has a lower priority)
        if(active_scheduler)
            _timer = new (kmalloc(sizeof(Scheduler_Timer))) 
                     Scheduler_Timer(QUANTUM, &time_slicer);
    }

	if(Traits<FPU>::enabled)
		FPU::init();
    
    Machine::smp_barrier();

    Thread * first;
    if(Machine::cpu_id() == 0) {
        // Create the application's main thread
        // This must precede idle, thus avoiding implicit rescheduling
        first = new(kmalloc(sizeof(Thread))) Thread(entry, RUNNING, MAIN);
        new(kmalloc(sizeof(Thread))) Thread(&idle, READY, IDLE);
    } else {
        first = new(kmalloc(sizeof(Thread))) Thread(&idle, READY, IDLE);
        // to avoid a race condition, assume that CPU1 starts with the lowest priority
        if(global_scheduler && Machine::cpu_id() == 1) {
            _lowest_priority_thread = first;
            _lowest_priority_cpu = 1;
        }
    }

    Machine::smp_barrier();

    db<Init, Thread>(INF) << "Dispatching the first thread: " << first
                          << "\n";

    This_Thread::not_booting();

    first->_context->load();
}

__END_SYS
