// EPOS AXI4LITE Mediator Initialization

#include <machine.h>
#include <system.h>
#include <uart.h>

extern "C" { void __epos_library_app_entry(void); }

__BEGIN_SYS

void AXI4LITE::init()
{
    db<AXI4LITE>(TRC) << "AXI4LITE::init()\n";

    if(Traits<AXI4LITE_IC>::enabled)
	AXI4LITE_IC::init();
    if(Traits<AXI4LITE_Timer>::enabled)
	AXI4LITE_Timer::init();
    if(Traits<AXI4LITE_UART>::enabled)
	AXI4LITE_UART::init();
    if(Traits<AXI4LITE_NIC>::enabled)
	AXI4LITE_NIC::init();
}

__END_SYS
