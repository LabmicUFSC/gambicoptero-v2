// EPOS IC Mediator Common Package

#ifndef __ic_h
#define __ic_h

#include <system/config.h>

__BEGIN_SYS

class IC_Common
{
protected:
    IC_Common() {}
public:
	enum {
	// it is used by the thread reschedule method when an IPI is necessary in global schedulers (SMP)
    INT_RESCHEDULER 
    };
public:
    typedef unsigned int Interrupt_Id;
    typedef void (* Interrupt_Handler)(Interrupt_Id);

	// it is used by the thread reschedule method when an IPI is necessary in global schedulers (SMP)
	static void ipi_send(int dest, int interrupt) { }
};

__END_SYS

#ifdef __IC_H
#include __IC_H
#endif

#endif
