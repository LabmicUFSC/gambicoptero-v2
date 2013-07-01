#include <timer.h>

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
    if((r = CPU::in16(IO::TIMER2_CSCTRL)) & 0x0010){
        CPU::out16(IO::TIMER2_CSCTRL, r & ~0x0010);
        handlers[TIMER2]();
        return;
    }
    if((r = CPU::in16(IO::TIMER3_CSCTRL)) & 0x0010){
        CPU::out16(IO::TIMER3_CSCTRL, r & ~0x0010);
        handlers[TIMER3]();
        return;
    }
}
