// EPOS Active Object Abstraction Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __active_h
#define __active_h

#include <thread.h>

__BEGIN_SYS

class Active: public Thread
{
public:
    Active() : Thread(&entry, this, Thread::SUSPENDED) {}
    virtual ~Active() {}

    virtual int run() = 0;

    void start() { resume(); }

private:
    static int entry(Active * runnable) { return runnable->run(); }
};

__END_SYS

#endif
