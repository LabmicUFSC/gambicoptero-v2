// EPOS AVR8 TSC Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __avr8_tsc_h
#define __avr8_tsc_h

#include <system/memory_map.h>
#include <cpu.h>
#include <tsc.h>

__BEGIN_SYS

class AVR8_TSC: public TSC_Common
{
private:
    // Imports
    typedef IO_Map<Machine> IO;

    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;

    // Timer1 Registers
    enum {
#if defined (__AVR_AT90CAN128__)
    TCNT1L  = 0x64,
    TCNT1H  = 0x65,
    TCCR1B  = 0x61,
    TCCR1A  = 0x60,
    TIMSK1  = 0x4F
#elif defined (__AVR_ATmega1281__)
    TCNT1L  = 0x64,
    TCNT1H  = 0x65,
    TCCR1B  = 0x61,
    TCCR1A  = 0x60,
    TIMSK1  = 0x4F
#else 
    TCNT1L  = 0x2C,
    TCNT1H  = 0x2D,
    TCCR1B  = 0x2E,
    TCCR1A  = 0x2F,
    TIMSK   = 0x37,
#endif
    };

    // Timer1 Register Bits
    enum {
    // TCCR1B
    ICNC1  = 0x80,
    ICES1  = 0x40,
    WGM13  = 0x10,
    WGM12  = 0x08,
    CS12   = 0x04,
    CS11   = 0x02,
    CS10   = 0x01,
#if defined (__AVR_AT90CAN128__)
    // TCCR1A, TCCR3A
    COMnA1 = 0x80,
    COMnA0 = 0x40,
    COMnB1 = 0x20,
    COMnB0 = 0x10,
    COMnC1 = 0x08,
    COMnC0 = 0x04,
    WGMn1  = 0x02,
    WGMn0  = 0x01,
    // TCCR1C, TCCR3C
    FOCnA  = 0x80,
    FOCnB  = 0x40,
    FOCnC  = 0x20,
    // TIMSK1
    TOIE1  = 0x01
#elif defined (__AVR_ATmega1281__)
    // TCCR1A
    COM1A1 = 0x80,
    COM1A0 = 0x40,
    COM1B1 = 0x20,
    COM1B0 = 0x10,
    COM1C1 = 0x08,
    COM1C0 = 0x04,
    WGM11  = 0x02,
    WGM10  = 0x01,
    // TCCR1C
    FOC1A  = 0x80,
    FOC1B  = 0x40,
    FOC1C  = 0x20,
    // TIMSK1
    TOIE1  = 0x01
#else
    // TCCR1A
    COM1A1 = 0x80,
    COM1A0 = 0x40,
    COM1B1 = 0x20,
    COM1B0 = 0x10,
    FOC1A  = 0x08,
    FOC1B  = 0x04,
    WGM11  = 0x02,
    WGM10  = 0x01,
    // TIMSK
    TOIE1  = 0x04
#endif
    };

public:
    AVR8_TSC() { /* Actual timer initialization is up to init */ }
    
#if defined (__AVR_ATmega1281__)
    static Hertz frequency() { return Traits<Machine>::CLOCK >> 9; }
#else
    static Hertz frequency() { return Traits<Machine>::CLOCK / 8; }
#endif
    static Time_Stamp time_stamp() {  
    return tcnt1hl() | _ts << (sizeof(Reg16) * 8);
    }

    static void init();
    
private:

#if defined (__AVR_AT90CAN128__)
    static void enable() { timsk1(timsk1() | TOIE1); }
    static void disable() { timsk1(timsk1() & ~TOIE1); }
    static Reg8 timsk1() { return AVR8::in8(IO::TIMSK1); }
    static void timsk1(Reg8 value) { AVR8::out8(IO::TIMSK1,value); }
#elif defined (__AVR_ATmega1281__)
    static void enable() { timsk1(timsk1() | TOIE1); }
    static void disable() { timsk1(timsk1() & ~TOIE1); }
    static Reg8 timsk1() { return AVR8::in8(IO::TIMSK1); }
    static void timsk1(Reg8 value) { AVR8::out8(IO::TIMSK1,value); }
#else
    static void enable() { timsk(timsk() | TOIE1); }
    static void disable() { timsk(timsk() & ~TOIE1); }
    static Reg8 timsk() { return AVR8::in8(IO::TIMSK); }
    static void timsk(Reg8 value) { AVR8::out8(IO::TIMSK,value); }
#endif
    static Reg8 tccr1a() { return CPU::in8(TCCR1A); }
    static void tccr1a(Reg8 value) { CPU::out8(TCCR1A, value); }
    static Reg8 tccr1b() { return CPU::in8(TCCR1B); }
    static void tccr1b(Reg8 value) { CPU::out8(TCCR1B, value); }
    static Reg16 tcnt1hl() { return CPU::in16(TCNT1L); }
    static void tcnt1hl(Reg16 value) { return CPU::out16(TCNT1L, value); }


public:
    static volatile unsigned long _ts;
};

__END_SYS

#endif
