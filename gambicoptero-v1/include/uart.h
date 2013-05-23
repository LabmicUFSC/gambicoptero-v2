// EPOS UART Mediator Common Package

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __uart_h
#define __uart_h

#include <system/config.h>

__BEGIN_SYS

class UART_Common
{
protected:
    UART_Common() {}
};

__END_SYS

#ifdef __UART_H
#include __UART_H
#endif

#endif
