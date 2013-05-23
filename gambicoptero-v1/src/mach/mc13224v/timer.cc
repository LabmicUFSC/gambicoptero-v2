#include <timer.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

__BEGIN_SYS

MC13224V_Timer::Handler* MC13224V_Timer::handlers[4];

__END_SYS

__USING_SYS

void MC13224V_Timer::handler_wrapper()
{
    db<MC13224V_IC>(TRC) << "MC13224V_Timer::handler_wrapper\n";
    Reg16 r;

    if((r = CPU::in16(IO::TIMER0_CSCTRL)) & 0x0010){
        CPU::out16(IO::TIMER0_CSCTRL, r & ~0x0010);
        handlers[TIMER0]();
        return;
    }
    if((r = CPU::in16(IO::TIMER1_CSCTRL)) & 0x0010){
        CPU::out16(IO::TIMER1_CSCTRL, r & ~0x0010);
        handlers[TIMER1]();
        return;
    }
/*
    if((r = CPU::in16(IO::TIMER2_CSCTRL)) & 0x0010){
        CPU::out16(IO::TIMER2_SCTRL, r & ~0x0010);
        handlers[TIMER2]();
        return;
    }
*/
    if((r = CPU::in16(IO::TIMER3_CSCTRL)) & 0x0010){
        CPU::out16(IO::TIMER3_CSCTRL, r & ~0x0010);
        handlers[TIMER3]();
        return;
    }
}
