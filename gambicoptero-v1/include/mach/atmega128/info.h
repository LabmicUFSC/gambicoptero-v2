// EPOS ATMega128 Run-Time System Information

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega128_info_h
#define __atmega128_info_h

#include "../avr_common/info.h"

__BEGIN_SYS

template<>
struct System_Info<ATMega128>: public System_Info<AVR8> {};

__END_SYS

#endif
