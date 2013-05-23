// EPOS Spin Lock Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __spin_h
#define __spin_h

#include <cpu.h>

__BEGIN_SYS

// Forwarder to the running thread id
class This_Thread
{
public:
    static unsigned int id();
    static void not_booting() { _not_booting = true; }

private:
    static bool _not_booting; 
};

// Recursive Spin Lock
class Spin
{
public:
    Spin(): _level(0), _owner(0) {}

    void acquire() {
	int me = This_Thread::id();

	while(CPU::cas(_owner, 0, me) != me);
	_level++;

	db<Spin>(TRC) << "Spin::acquire[SPIN=" << this
		      << ",ID=" << me
		      << "]() => {owner=" << _owner 
		      << ",level=" << _level << "}\n";
    }

    void release() {
    	if(--_level <= 0)
	    _owner = 0;

	db<Spin>(TRC) << "Spin::release[SPIN=" << this
		      << "]() => {owner=" << _owner 
		      << ",level=" << _level << "}\n";
    }

private:
    volatile unsigned int _level;
    volatile int _owner;
};

__END_SYS

#endif
