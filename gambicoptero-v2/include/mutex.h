// EPOS Mutex Abstraction Declarations

#ifndef __mutex_h
#define __mutex_h

#include <synchronizer.h>

__BEGIN_SYS

class Mutex: protected Synchronizer_Common
{
public:
    Mutex() : _locked(false) {
	db<Synchronizer>(TRC) << "Mutex() => " << this << "\n"; 
    }

    ~Mutex() {
	db<Synchronizer>(TRC) << "~Mutex(this=" << this << ")\n";
    }

    void lock() { 
	db<Synchronizer>(TRC) << "Mutex::lock(this=" << this << ")\n";

	begin_atomic();
	if(tsl(_locked))
	    sleep(); // implicit end_atomic()
	else
	    end_atomic();
    }

    void unlock() { 
	db<Synchronizer>(TRC) << "Mutex::unlock(this=" << this << ")\n";

	begin_atomic();
	_locked = false;
	wakeup(); // implicit end_atomic()
    }

private:
    volatile bool _locked;
};


// An event handler that triggers a mutex (see handler.h)
class Mutex_Handler: public Handler
{
public:
    Mutex_Handler(Mutex * h) : _handler(h) {}
    ~Mutex_Handler() {}

    void operator()() { _handler->unlock(); }
	
private:
    Mutex * _handler;
};

__END_SYS

#endif
