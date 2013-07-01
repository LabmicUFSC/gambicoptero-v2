// EPOS ATMega1281 Run-Time System Information

#ifndef __atmega1281_info_h
#define __atmega1281_info_h

#include "../avr_common/info.h"

__BEGIN_SYS

template<>
struct System_Info<ATMega1281>: public System_Info<AVR8> {};

__END_SYS

#endif

