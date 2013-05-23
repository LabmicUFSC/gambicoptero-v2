// EPOS Task Abstraction Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __task_h
#define __task_h

#include <utility/malloc.h>
#include <address_space.h>
#include <segment.h>
#include <thread.h>

__BEGIN_SYS

class Task
{
private:
    typedef CPU::Log_Addr Log_Addr;
    typedef CPU::Context Context;
    typedef class Queue<Thread> Queue;

    static const unsigned int STACK_SIZE = 
	Traits<Machine>::APPLICATION_STACK_SIZE;

public:
    Task(const Segment & c, const Segment & d) : _code(c), _data(d) {
	db<Task>(TRC) << "Task()\n";
	_as.attach(c); 
	_as.attach(d); 
    }
    ~Task() {
	db<Task>(TRC) << "~Task()\n";
	while(!_threads.empty())
	    free(_threads.remove()->object());
    }

    Address_Space * as() { return &_as; }
    const Segment & code() const { return _code; }
    const Segment & data() const { return _data; }

    Thread * create_thread(int (* entry)(), 
        const Thread::State & state = Thread::READY,
	const Thread::Criterion & criterion = Thread::NORMAL,
	unsigned int stack_size = STACK_SIZE) {
	_as.activate();
	Thread * t = new(kmalloc(sizeof(Thread)))
	    Thread(entry, state, criterion);
	Queue::Element * e = new(kmalloc(sizeof(Queue::Element))) Queue::Element(t);
	_threads.insert(e);
	return t;
    }
    void destroy_thread(Thread * t) {
	Queue::Element * e = _threads.remove(t);
	if(e)
	    delete e->object();
    }

//    static Thread * volatile  & self() { return _running; }

    static void init();

private:
    Address_Space _as;
    const Segment & _code;
    const Segment & _data;
    Queue _threads;

//    static Task * volatile _running;
};

__END_SYS

#endif
