// EPOS AT90CAN128_IC Initialization

#include <mach/at90can128/ic.h>

__BEGIN_SYS

AT90CAN128_IC::Interrupt_Handler  AT90CAN128_IC::_int_vector[IC::INTS];

__END_SYS


__USING_SYS

extern "C" void __epos_call_handler(char offset);

extern "C" void __epos_call_handler(char offset) {
    IC::Interrupt_Handler handler = IC::int_vector(offset);

    if (handler != 0)
    handler();
}
