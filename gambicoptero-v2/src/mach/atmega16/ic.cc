// EPOS ATMega16 IC Mediator Implementation

#include <mach/atmega16/ic.h>

__BEGIN_SYS

ATMega16_IC::Interrupt_Handler ATMega16_IC::_int_vector[INTS];

__END_SYS


__USING_SYS

extern "C" void __epos_call_handler(char offset);

extern "C" void __epos_call_handler(char offset) {
    IC::Interrupt_Handler handler = IC::int_vector(offset);

    if (handler != 0)
    handler();
}
