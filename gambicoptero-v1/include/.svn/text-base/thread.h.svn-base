// EPOS Thread Abstraction Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __thread_h
#define __thread_h

#include <system/kmalloc.h>
#include <utility/queue.h>
#include <utility/handler.h>
#include <cpu.h>
#include <scheduler.h>

__BEGIN_SYS

class Thread
{
    friend class Scheduler<Thread>;
    friend class Synchronizer_Common;

protected:
    static const bool active_scheduler = Traits<Thread>::Criterion::timed;
    static const bool preemptive = Traits<Thread>::Criterion::preemptive;
    static const bool energy_aware = Traits<Thread>::Criterion::energy_aware;
    static const bool smp = Traits<Thread>::smp;

    static const unsigned int QUANTUM = Traits<Thread>::QUANTUM;
    static const unsigned int STACK_SIZE =
        Traits<Machine>::APPLICATION_STACK_SIZE;

    typedef CPU::Log_Addr Log_Addr;
    typedef CPU::Context Context;

public:
    // Thread State
    enum State {
        BEGINNING,
        READY,
        RUNNING,
        SUSPENDED,
        WAITING,
        FINISHING
    };

    // Thread Priority
    typedef Scheduling_Criteria::Priority Priority;
    
    // Thread Scheduling Criterion
    typedef Traits<Thread>::Criterion Criterion;
    enum {
        NORMAL = Criterion::NORMAL,
        MAIN = Criterion::MAIN,
        IDLE = Criterion::IDLE
    };

    // Thread Queue
    typedef Ordered_Queue<Thread, Criterion, false,
                          Scheduler<Thread>::Element> Queue;

public:
    Thread(int (* entry)(), 
           const State & state = READY,
           const Criterion & criterion = NORMAL,
           unsigned int stack_size = STACK_SIZE)
    : _state(state), _waiting(0), _joining(0), _link(this, criterion)
    {
        lock();

        _stack = kmalloc(stack_size);
        _context = CPU::init_stack(_stack, stack_size, &implicit_exit, entry);

        common_constructor(entry, stack_size);
    }

    template<typename T1>
    Thread(int (* entry)(T1 a1), T1 a1,
           const State & state = READY,
           const Criterion & criterion = NORMAL,
           unsigned int stack_size = STACK_SIZE)
    : _state(state), _waiting(0), _joining(0), _link(this, criterion)
    {
        lock();

        _stack = kmalloc(stack_size);
        _context = CPU::init_stack(_stack, stack_size, &implicit_exit, entry, 
                                   a1);

        common_constructor(entry, stack_size);
    }

    template<typename T1, typename T2>
    Thread(int (* entry)(T1 a1, T2 a2), T1 a1, T2 a2,
           const State & state = READY,
           const Criterion & criterion = NORMAL,
           unsigned int stack_size = STACK_SIZE)
    : _state(state), _waiting(0), _joining(0), _link(this, criterion)
    {
        lock();

        _stack = kmalloc(stack_size);
        _context = CPU::init_stack(_stack, stack_size, &implicit_exit, entry,
                                   a1, a2);

        common_constructor(entry, stack_size);
    }

    template<typename T1, typename T2, typename T3>
    Thread(int (* entry)(T1 a1, T2 a2, T3 a3), T1 a1, T2 a2, T3 a3,
           const State & state = READY,
           const Criterion & criterion = NORMAL,
           unsigned int stack_size = STACK_SIZE)
    : _state(state), _waiting(0), _joining(0), _link(this, criterion)
    {
        lock();

        _stack = kmalloc(stack_size);
        _context = CPU::init_stack(_stack, stack_size, &implicit_exit, entry, 
                                   a1, a2, a3);

        common_constructor(entry, stack_size);
    }

    ~Thread();

    const volatile State & state() const { return _state; }
    const volatile Criterion & criterion() const { return _link.rank(); }

    Priority  priority() const { return int(_link.rank()); }
    void priority(const Priority & p);    

    int join();
    void pass();
    void suspend() { suspend(false); }
    void resume();

    static Thread * self() { return running(); }
    static void yield();
    static void exit(int status = 0);

    static void init();

protected:
    void common_constructor(Log_Addr entry, unsigned int stack_size);

    static Thread * volatile running() { return _scheduler.chosen(); }

    Queue::Element * link() { return &_link; }

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

    void suspend(bool locked);
    static void sleep(Queue * q);
    static void wakeup(Queue * q);
    static void wakeup_all(Queue * q);

    static void reschedule(bool preempt = preemptive);

    static void time_slicer();

    static void implicit_exit();

    static void dispatch(Thread * prev, Thread * next, bool charge = true) {
//	if(charge) {
//  	    if(active_scheduler)
//  		_timer->reset();
//  	    if(energy_aware)
//  		account_energy();
//	}

        if(prev != next) {
            if(prev->_state == RUNNING)
                prev->_state = READY;
            next->_state = RUNNING;

            db<Thread>(TRC) << "Thread::dispatch(prev=" << prev
                            << ",next=" << next << ")\n";

            if(smp)
                _lock.release();
            CPU::switch_context(&prev->_context, next->_context);
        } else
            if(smp)
                _lock.release();

        CPU::int_enable();
    }

    static int idle();

protected:
    Log_Addr _stack;
    Context * volatile _context;
    volatile State _state; 
    Queue * _waiting;
    Thread * volatile _joining;
    Queue::Element _link;

    static Spin _lock;
    static Scheduler_Timer * _timer;
    static unsigned int _thread_count;
    static Scheduler<Thread> _scheduler;
};

// An event handler that triggers a thread (see handler.h)
class Thread_Handler : public Handler
{
public:
    Thread_Handler(Thread * h) : _handler(h) {}
    ~Thread_Handler() {}

    void operator()() { _handler->resume(); }

private:
    Thread * _handler;
};

__END_SYS

#endif
