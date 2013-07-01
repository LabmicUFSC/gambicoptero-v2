#include <mach/integratorcp/timer.h>

__BEGIN_SYS

Timer::Handler * IntegratorCP_Timer::_handler[3] = { 0, 0, 0};

void IntegratorCP_Timer::int_handler(IC::Interrupt_Id id)
{
    CPU::Reg32 clr = TIMER_CLEAR;
    switch (id)
    {
        case IC::TIMERINT0: id = 0; break;
        case IC::TIMERINT1: id = 1; clr += 0x100; break;
        case IC::TIMERINT2: id = 2; clr += 0x200; break;
        default:
            return;
    }
    //clear
    CPU::out32(clr, ~0);
    //call handler
    if (_handler[id]) _handler[id]();
}

__END_SYS



