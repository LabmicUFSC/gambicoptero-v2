// EPOS AVR8_TSC Implementation

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
