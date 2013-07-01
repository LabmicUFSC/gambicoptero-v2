// EPOS PLASMA UART Mediator Implementation

#include <mach/plasma/uart.h>

__BEGIN_SYS

void PLASMA_UART::int_handler(unsigned int interrupt){
    db<PLASMA_UART>(TRC) << "PLASMA_UART::int_handler(intr=" << interrupt << ")\n";
    //verify if data is ready or write has finished and warns specific thread waiting
}

__END_SYS
