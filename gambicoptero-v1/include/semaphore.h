// EPOS Semaphore Abstraction Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __semaphore_h
#define __semaphore_h

#include <utility/handler.h>
#include <synchronizer.h>

__BEGIN_SYS

class Semaphore: protected Synchronizer_Common
{
public:
    Semaphore(int v = 1) : _value(v) {
	db<Synchronizer>(TRC) << "Semaphore(value=" << _value << ") => "
			      << this << "\n";
    }

    ~Semaphore() {
	db<Synchronizer>(TRC) << "~Semaphore(this=" << this << ")\n";
    }

    void p() { 
	db<Synchronizer>(TRC) << "Semaphore::p(this=" << this 
			      << ",value=" << _value << ")\n";

	begin_atomic();
	if(fdec(_value) < 1)
	    sleep(); // implicit end_atomic()
	else
	    end_atomic();
    }

    void v() {
	db<Synchronizer>(TRC) << "Semaphore::v(this=" << this
			      << ",value=" << _value << ")\n";

	begin_atomic();
	if(finc(_value) < 0)
	    wakeup();  // implicit end_atomic()
	else
	    end_atomic();
    }

private:
    volatile int _value;
};


// An event handler that triggers a semaphore (see handler.h)
class Semaphore_Handler: public Handler
{
public:
    Semaphore_Handler(Semaphore * h) : _handler(h) {}
    ~Semaphore_Handler() {}

    void operator()() { _handler->v(); }
	
private:
    Semaphore * _handler;
};

__END_SYS

#endif
