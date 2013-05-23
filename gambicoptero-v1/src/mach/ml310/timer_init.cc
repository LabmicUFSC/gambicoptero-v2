// EPOS ML310 Timer Mediator

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

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
