// EPOS AVR8_TSC Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <arch/avr8/tsc.h>

__BEGIN_SYS

volatile unsigned long AVR8_TSC::_ts;

__END_SYS

__USING_SYS

#if defined (__AVR_ATmega1281__)

extern "C" void __vector_21(void) __attribute__ ((signal));

extern "C" void __vector_21(void) {

    TSC::_ts++;

}

#elif defined (__AVR_ATmega128__)

extern "C" void __vector_14(void) __attribute__ ((signal));

extern "C" void __vector_14(void) {

    TSC::_ts++;

}

#elif defined (__AVR_ATmega16__)

extern "C" void __vector_8(void) __attribute__ ((signal));

extern "C" void __vector_8(void) {

    TSC::_ts++;

}

#endif
