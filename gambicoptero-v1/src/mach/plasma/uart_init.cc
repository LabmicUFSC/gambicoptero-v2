// EPOS PLASMA UART Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>

__BEGIN_SYS

void PLASMA_UART::init()
{
    db<PLASMA_UART>(TRC) << "PLASMA_UART::init()\n";

    //PLASMA_IC::enable(PLASMA_IC::IRQ_UART_READ);
    //PLASMA_IC::enable(PLASMA_IC::IRQ_UART_WRITE);
}

__END_SYS
