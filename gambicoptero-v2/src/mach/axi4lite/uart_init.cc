// EPOS AXI4LITE UART Mediator Initialization

#include <machine.h>

__BEGIN_SYS

void AXI4LITE_UART::init()
{
    db<AXI4LITE_UART>(TRC) << "AXI4LITE_UART::init()\n";

	//Set configured BAUDRATE;
	_regs->clkdiv = CLOCK/BAUDRATE;

}

__END_SYS
