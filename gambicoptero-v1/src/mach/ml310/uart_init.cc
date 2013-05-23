// EPOS ML310 UART Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>

__BEGIN_SYS

void ML310_UART::init()
{
    db<ML310_UART>(TRC) << "ML310_UART::init()\n";

    //Enable interrupt...
    ML310_IC::enable(ML310_IC::INT_UART_INTR);
}

__END_SYS
