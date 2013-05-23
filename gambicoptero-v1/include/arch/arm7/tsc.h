// EPOS-- ARM7 Time-Stamp Counter Mediator Declarations

#ifndef __arm7_tsc_h
#define __arm7_tsc_h

#include <tsc.h>
#include <cpu.h>

__BEGIN_SYS

class ARM7_TSC: public TSC_Common
{
private:
    static const unsigned int CLOCK = Traits<Machine>::CLOCK;

public:
    ARM7_TSC();

    static Hertz frequency() {return CLOCK;}

    static void init();

    static Time_Stamp time_stamp();

private:
    static void tsc_timer_handler();

    static volatile Time_Stamp _ts;

    static const int instance = 3;
    static const int offset = instance * 0x20;

    static void ctrl(CPU::Reg16 value);
    static CPU::Reg16 ctrl();

    static void sctrl(CPU::Reg16 value);
    static CPU::Reg16 sctrl();

    static void comp1(CPU::Reg16 value);
    static CPU::Reg16 comp1();

    static void comp2(CPU::Reg16 value);
    static CPU::Reg16 comp2();

    static void capt(CPU::Reg16 value);
    static CPU::Reg16 capt();

    static void load(CPU::Reg16 value);
    static CPU::Reg16 load();

    static void hold(CPU::Reg16 value);
    static CPU::Reg16 hold();

    static void cntr(CPU::Reg16 value);
    static CPU::Reg16 cntr();

    static void cmpld1(CPU::Reg16 value);
    static CPU::Reg16 cmpld1();

    static void cmpld2(CPU::Reg16 value);
    static CPU::Reg16 cmpld2();

    static void csctrl(CPU::Reg16 value);
    static CPU::Reg16 csctrl();

    static void enbl(CPU::Reg16 value);
    static CPU::Reg16 enbl();
};

__END_SYS

#endif
