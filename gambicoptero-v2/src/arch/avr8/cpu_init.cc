// EPOS AVR8 CPU Mediator Initialization

#include <cpu.h>
#include <mmu.h>
#include <tsc.h>

__BEGIN_SYS

void AVR8::init()
{
    db<Init, AVR8>(TRC) << "AVR8::init()\n";

    if(_Traits::power_management) {
        //Enable sleep modes
        out8(MCUCR, in8(MCUCR) | SE);
	db<AVR8>(INF) << "AVR8:: Power Management Enabled\n";
    }
    else {
        //Disable sleep modes
        out8(MCUCR, in8(MCUCR) & ~SE);
	db<AVR8>(INF) << "AVR8:: Power Management Disabled\n";
    }

    // Initialize the MMU
    if(Traits<AVR8_MMU>::enabled)
	AVR8_MMU::init();
    else
	db<Init, AVR8>(WRN) << "MMU is disabled!\n";

    if(Traits<AVR8_TSC>::enabled)
	AVR8_TSC::init();

}

__END_SYS
