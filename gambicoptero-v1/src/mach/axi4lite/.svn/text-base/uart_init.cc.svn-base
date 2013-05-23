// EPOS AXI4LITE UART Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>

__BEGIN_SYS

void AXI4LITE_UART::init()
{
    db<AXI4LITE_UART>(TRC) << "AXI4LITE_UART::init()\n";

	//Set configured BAUDRATE;
	_regs->clkdiv = CLOCK/BAUDRATE;

}

__END_SYS
