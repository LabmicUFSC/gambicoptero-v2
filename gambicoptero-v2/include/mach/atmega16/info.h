// EPOS ATMega16 Run-Time System Information

#ifndef __atmega16_info_h
#define __atmega16_info_h

#include "../avr_common/info.h"

__BEGIN_SYS

template<>
struct System_Info<ATMega16>: public System_Info<AVR8> {};

__END_SYS

#endif
