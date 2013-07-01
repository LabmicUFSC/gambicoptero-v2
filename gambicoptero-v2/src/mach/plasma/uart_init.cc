// EPOS PLASMA UART Mediator Initialization

#include <machine.h>

__BEGIN_SYS

void PLASMA_UART::init()
{
    db<PLASMA_UART>(TRC) << "PLASMA_UART::init()\n";

    //PLASMA_IC::enable(PLASMA_IC::IRQ_UART_READ);
    //PLASMA_IC::enable(PLASMA_IC::IRQ_UART_WRITE);
}

__END_SYS
