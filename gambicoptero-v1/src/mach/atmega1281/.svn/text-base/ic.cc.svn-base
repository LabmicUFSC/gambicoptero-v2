// EPOS ATMega1281 Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/atmega1281/ic.h>

__BEGIN_SYS

ATMega1281_IC::Interrupt_Handler  ATMega1281_IC::_int_vector[INTS];

__END_SYS


__USING_SYS

extern "C" void __epos_call_handler(char offset);

extern "C" void __epos_call_handler(char offset) {
    IC::Interrupt_Handler handler = IC::int_vector(offset);

    if (handler != 0)
    handler();
}