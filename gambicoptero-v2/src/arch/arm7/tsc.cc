// EPOS-- ARM7_TSC Implementation

#include <arch/arm7/tsc.h>

#include <machine.h>

__BEGIN_SYS

volatile ARM7_TSC::Time_Stamp ARM7_TSC::_ts = 0;

ARM7_TSC::Time_Stamp ARM7_TSC::time_stamp() {
    return cntr() | (_ts << 16);
}

void ARM7_TSC::tsc_timer_handler() {
    _ts++;
}

ARM7_TSC::ARM7_TSC() {}

void ARM7_TSC::ctrl(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_CTRL + offset, value); }
CPU::Reg16 ARM7_TSC::ctrl() { return CPU::in16(Machine::IO::TIMER0_CTRL + offset); }

void ARM7_TSC::sctrl(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_SCTRL + offset, value); }
CPU::Reg16 ARM7_TSC::sctrl() { return CPU::in16(Machine::IO::TIMER0_SCTRL + offset); }

void ARM7_TSC::comp1(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_COMP1 + offset, value); }
CPU::Reg16 ARM7_TSC::comp1() { return CPU::in16(Machine::IO::TIMER0_COMP1 + offset); }

void ARM7_TSC::comp2(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_COMP2 + offset, value); }
CPU::Reg16 ARM7_TSC::comp2() { return CPU::in16(Machine::IO::TIMER0_COMP2 + offset); }

void ARM7_TSC::capt(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_CAPT + offset, value); }
CPU::Reg16 ARM7_TSC::capt() { return CPU::in16(Machine::IO::TIMER0_CAPT + offset); }

void ARM7_TSC::load(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_LOAD + offset, value); }
CPU::Reg16 ARM7_TSC::load() { return CPU::in16(Machine::IO::TIMER0_LOAD + offset); }

void ARM7_TSC::hold(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_HOLD + offset, value); }
CPU::Reg16 ARM7_TSC::hold() { return CPU::in16(Machine::IO::TIMER0_HOLD + offset); }

void ARM7_TSC::cntr(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_CNTR + offset, value); }
CPU::Reg16 ARM7_TSC::cntr() { return CPU::in16(Machine::IO::TIMER0_CNTR + offset); }

void ARM7_TSC::cmpld1(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_CMPLD1 + offset, value); }
CPU::Reg16 ARM7_TSC::cmpld1() { return CPU::in16(Machine::IO::TIMER0_CMPLD1 + offset); }

void ARM7_TSC::cmpld2(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_CMPLD2 + offset, value); }
CPU::Reg16 ARM7_TSC::cmpld2() { return CPU::in16(Machine::IO::TIMER0_CMPLD2 + offset); }

void ARM7_TSC::csctrl(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_CSCTRL + offset, value); }
CPU::Reg16 ARM7_TSC::csctrl() { return CPU::in16(Machine::IO::TIMER0_CSCTRL + offset); }

void ARM7_TSC::enbl(CPU::Reg16 value){ CPU::out16(Machine::IO::TIMER0_ENBL, value); }
CPU::Reg16 ARM7_TSC::enbl() { return CPU::in16(Machine::IO::TIMER0_ENBL); }


__END_SYS
