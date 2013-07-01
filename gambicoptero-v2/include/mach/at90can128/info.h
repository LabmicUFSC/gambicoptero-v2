// EPOS ATMega128 Run-Time System Information

#ifndef __at90can128_info_h
#define __at90can128_info_h

#include "../avr_common/info.h"

__BEGIN_SYS

template<>
struct System_Info<AT90CAN128>: public System_Info<AVR8> {};

__END_SYS

#endif
