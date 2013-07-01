// EPOS Timer Mediator Common Package

#ifndef __timer_h
#define __timer_h

#include <utility/handler.h>
#include <tsc.h>

__BEGIN_SYS

class Timer_Common
{
protected:
    Timer_Common() {}

public:
    typedef TSC::Hertz Hertz;
    typedef TSC::Hertz Tick;
    typedef void (Handler)();
};

__END_SYS

#ifdef __TIMER_H
#include __TIMER_H
#endif

#endif
