// EPOS AXI4LITE UART Mediator Implementation

#include <uart.h>

__BEGIN_SYS

AXI4LITE_UART::MM_Reg * AXI4LITE_UART::_regs = reinterpret_cast<AXI4LITE_UART::MM_Reg *>(AXI4LITE_UART::BASE_ADDRESS);

void AXI4LITE_UART::int_handler(unsigned int interrupt){
    db<AXI4LITE_UART>(TRC) << "AXI4LITE_UART::int_handler(intr=" << interrupt << ")\n";
	// To be implemented.
	
}

__END_SYS
