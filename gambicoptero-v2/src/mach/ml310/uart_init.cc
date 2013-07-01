// EPOS ML310 UART Mediator Initialization

#include <machine.h>

__BEGIN_SYS

void ML310_UART::init()
{
    db<ML310_UART>(TRC) << "ML310_UART::init()\n";

    //Enable interrupt...
    ML310_IC::enable(ML310_IC::INT_UART_INTR);
}

__END_SYS
