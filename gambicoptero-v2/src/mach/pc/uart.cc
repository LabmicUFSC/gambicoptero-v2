// EPOS PC UART Mediator Implementation

#include <mach/pc/uart.h>

__BEGIN_SYS

const PC_UART::IO_Port PC_UART::_ports[] = { Traits<PC_UART>::COM1, 
					     Traits<PC_UART>::COM2, 
					     Traits<PC_UART>::COM3,
					     Traits<PC_UART>::COM4 };

__END_SYS
