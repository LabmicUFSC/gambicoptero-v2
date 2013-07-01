// EPOS ATMega1281 Mediator Implementation

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
