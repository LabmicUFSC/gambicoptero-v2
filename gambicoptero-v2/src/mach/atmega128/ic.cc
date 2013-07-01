// EPOS ATMega128 IC Mediator Implementation

#include <mach/atmega128/ic.h>

__BEGIN_SYS

ATMega128_IC::Interrupt_Handler  ATMega128_IC::_int_vector[INTS];

__END_SYS


__USING_SYS

extern "C" void __epos_call_handler(char offset);

extern "C" void __epos_call_handler(char offset) {
    IC::Interrupt_Handler handler = IC::int_vector(offset);

    if (handler != 0)
    handler();
}
