// EPOS ML310 Timer Mediator

#include <timer.h>

__BEGIN_SYS

void ML310_Timer::init()
{
    db<ML310_Timer>(TRC) << "ML310_Timer::init()\n";

    CPU::_mtspr(SPR_PIT, 0); //PIT = 0
    CPU::_mtspr(SPR_TSR, 0xFFFFFFFF); //Disable all status (TSR)
    CPU::_mtspr(SPR_TCR, 0x04400000); //Enable Timer with PIT AUTO RELOAD ENABLED. (TCR)
}

__END_SYS
