// EPOS IC Mediator Common Package

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __ic_h
#define __ic_h

#include <system/config.h>

__BEGIN_SYS

class IC_Common
{
protected:
    IC_Common() {}

public:
    typedef unsigned int Interrupt_Id;
    typedef void (* Interrupt_Handler)(Interrupt_Id);
};

__END_SYS

#ifdef __IC_H
#include __IC_H
#endif

#endif
