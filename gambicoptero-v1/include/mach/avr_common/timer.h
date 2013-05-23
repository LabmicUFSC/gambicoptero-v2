// EPOS AVR Timer Common Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __avr_timer_h
#define __avr_timer_h

#include <timer.h>

__BEGIN_SYS

class AVR_Timer
{
protected:
    AVR_Timer() {}

private:
    // Imports
    typedef IO_Map<Machine> IO;
    typedef AVR8::Reg8 Reg8;
    typedef AVR8::Reg16 Reg16;

public:
    // The timer's counter
    typedef CPU::Reg16 Count;

// Bit Definitions
    enum {
#if defined (__AVR_AT90CAN128__)
    // TIMSK0
    OCIE0A   = 0x02,
    TOIE0    = 0x01,
    // TIMSK1
    ICIE1    = 0x20,
    OCIE1C   = 0x08,
    OCIE1B   = 0x04,
    OCIE1A   = 0x02,
    TOIE1    = 0x01,
    // TIMSK2
    OCIE2A   = 0x02,
    TOIE2    = 0x01,
    // TIMSK3
    ICIE3    = 0x20,
    OCIE3C   = 0x08,
    OCIE3B   = 0x04,
    OCIE3A   = 0x02,
    TOIE3    = 0x01,
    // TIFR0
    OCF0A    = 0x02,
    TOV0     = 0x01,
    // TIFR1
    ICF1     = 0x20,
    OCF1C    = 0x08,
    OCF1B    = 0x04,
    OCF1A    = 0x02,
    TOV1     = 0x01,
    // TIFR2
    OCF2A    = 0x02,
    TOV2     = 0x01,
    // TIFR3
    ICF3     = 0x20,
    OCF3C    = 0x08,
    OCF3B    = 0x04,
    OCF3A    = 0x02,
    TOV3     = 0x01,
    // GTCCR
    TSM      = 0x80,
    PSR2     = 0x02,
    PSR310   = 0x01,
    // ADCSRB
    ACME     = 0x40,
    // MCUCR
    PUD      = 0x10,
    // TCCR0A
    FOC0A   = 0x80,
    WGM00   = 0x40,
    COM0A1  = 0x20,
    COM0A0  = 0x10,
    WGM01   = 0x08,
    CS02    = 0x04,
    CS01    = 0x02,
    CS00    = 0x01,
    // ASSR
    EXCLK   = 0x10,
    AS2     = 0x08,
    TCN2UB  = 0x04,
    OCR2UB  = 0x02,
    TCR2UB  = 0x01,
    // TCCR1A, TCCR3A
    COMnA1 = 0x80,
    COMnA0 = 0x40,
    COMnB1 = 0x20,
    COMnB0 = 0x10,
    COMnC1 = 0x08,
    COMnC0 = 0x04,
    WGMn1  = 0x02,
    WGMn0  = 0x01,
    // TCCR1B, TCCR3B
    ICNCn  = 0x80,
    ICESn  = 0x40,
    WGMn3  = 0x10,
    WGMn2  = 0x08,
    CSn2   = 0x04,
    CSn1   = 0x02,
    CSn0   = 0x01,
    // TCCR1C, TCCR3C
    FOCnA  = 0x80,
    FOCnB  = 0x40,
    FOCnC  = 0x20,
    // TCCR2A
    FOC2A  = 0x80,
    WGM20  = 0x40,
    COM2A1 = 0x20,
    COM2A0 = 0x10,
    WGM21  = 0x08,
    CS22   = 0x04,
    CS21   = 0x02,
    CS20   = 0x01

#elif defined (__AVR_ATmega1281__)
        // TIMSK0
        OCIE0B   = 0x04,
        OCIE0A   = 0x02,
        TOIE0    = 0x01,
        // TIMSK1
        ICIE1    = 0x20,
        OCIE1C   = 0x08,
        OCIE1B   = 0x04,
        OCIE1A   = 0x02,
        TOIE1    = 0x01,
    // TIMSK2
        OCIE2B   = 0x04,
        OCIE2A   = 0x02,
    TOIE2    = 0x01,
        // TIMSK3
        ICIE3    = 0x20,
        OCIE3C   = 0x08,
        OCIE3B   = 0x04,
        OCIE3A   = 0x02,
        TOIE3    = 0x01,
        // TIMSK4
        ICIE4    = 0x20,
        OCIE4C   = 0x08,
        OCIE4B   = 0x04,
        OCIE4A   = 0x02,
        TOIE4    = 0x01,
        // TIMSK5
        ICIE5    = 0x20,
        OCIE5C   = 0x08,
        OCIE5B   = 0x04,
        OCIE5A   = 0x02,
        TOIE5    = 0x01,
        // TIFR0
        OCF0B    = 0x04,
        OCF0A    = 0x02,
        TOV0     = 0x01,
        // TIFR1
        ICF1     = 0x20,
        OCF1C    = 0x08,
        OCF1B    = 0x04,
        OCF1A    = 0x02,
        TOV1     = 0x01,
        // TIFR2
        OCF2B    = 0x04,
        OCF2A    = 0x02,
        TOV2     = 0x01,
        // TIFR3
        ICF3     = 0x20,
        OCF3C    = 0x08,
        OCF3B    = 0x04,
        OCF3A    = 0x02,
        TOV3     = 0x01,
        // TIFR4
        ICF4     = 0x20,
        OCF4C    = 0x08,
        OCF4B    = 0x04,
        OCF4A    = 0x02,
        TOV4     = 0x01,
        // TIFR5
        ICF5     = 0x20,
        OCF5C    = 0x08,
        OCF5B    = 0x04,
        OCF5A    = 0x02,
        TOV5     = 0x01,
        // GTCCR
    TSM     = 0x80,
    PSRASY  = 0x02,
    PSRSYNC = 0x01,
        // ADCSRB
    ACME   = 0x40,
        // MCUCR
    PUD    = 0x10,
    // TCCR0B 
    FOC0A  = 0x80,
    FOC0B  = 0x40,
    WGM02  = 0x08,
    CS02   = 0x04,
    CS01   = 0x02,
    CS00   = 0x01,
        // TCCR0A
    COM0A1 = 0x80,
    COM0A0 = 0x40,
    COM0B1 = 0x20,
    COM0B0 = 0x10,
    WGM01  = 0x02,
    WGM00  = 0x01,
    // ASSR
    AS2     = 0x20,
    TCN2UB  = 0x10,
    OCR2AUB = 0x08,
    OCR2BUB = 0x04,
    TCR2AUB = 0x02,
    TCR2BUB = 0x01,
    // TCCR1A, TCCR3A
    COMnA1 = 0x80,
    COMnA0 = 0x40,
    COMnB1 = 0x20,
    COMnB0 = 0x10,
    COMnC1 = 0x08,
    COMnC0 = 0x04,
    WGMn1  = 0x02,
    WGMn0  = 0x01,
    // TCCR1B, TCCR3B
    ICNCn  = 0x80,
    ICESn  = 0x40,
    WGMn3  = 0x10,
    WGMn2  = 0x08,
    CSn2   = 0x04,
    CSn1   = 0x02,
    CSn0   = 0x01,
    // TCCR1C, TCCR3C
    FOCnA  = 0x80,
    FOCnB  = 0x40,
    FOCnC  = 0x20,
    // TCCR2B
    FOC2A  = 0x80,
    FOC2B  = 0x40,
    WGM22  = 0x08,
    CS22   = 0x04,
    CS21   = 0x02,
    CS20   = 0x01,
    // TCCR2A
        COM2A1 = 0x80,
        COM2A0 = 0x40,
        COM2B1 = 0x20,
        COM2B0 = 0x10,
        WGM21  = 0x02,
        WGM20  = 0x01
#else
    // TIMSK
    OCIE2  = 0x80,
    TOIE2  = 0x40,
    TICIE1 = 0x20,
    OCIE1A = 0x10,
    OCIE1B = 0x08,
    TOIE1  = 0x04,
    OCIE0  = 0x02,
    TOIE0  = 0x01,
    // ETIMSK
    TICIE3 = 0x20,
    OCIE3A = 0x10,
    OCIE3B = 0x08,
    TOIE3  = 0x04,
    OCIE3C = 0x02,
    OCIE1C = 0x01,
    // TIFR
    OCF2   = 0x80,
    TOV2   = 0x40,
    ICF1   = 0x20,
    OCF1A  = 0x10,
    OCF1B  = 0x08,
    TOV1   = 0x04,
    OCF0   = 0x02,
    TOV0   = 0x01,
    // ETIFR
    ICF3   = 0x20,
    OCF3A  = 0x10,
    OCF3B  = 0x08,
    TOV3   = 0x04,
    OCF3C  = 0x02,
    OCF1C  = 0x01,
    // SFIOR
    TSM    = 0x80,
    ACME   = 0x08,
    PUD    = 0x04,
    PSR0   = 0x02,
    PSR321 = 0x01,
    // TCCR0 
    FOC0   = 0x80,
    WGM00  = 0x40,
    COM01  = 0x20,
    COM00  = 0x10,
    WGM01  = 0x08,
    CS02   = 0x04,
    CS01   = 0x02,
    CS00   = 0x01,
    // ASSR
    AS0    = 0x08,
    TCN0UB = 0x04,
    OCR0UB = 0x02,
    TCR0UB = 0x01,
    // TCCR1A, TCCR3A
    COMnA1 = 0x80,
    COMnA0 = 0x40,
    COMnB1 = 0x20,
    COMnB0 = 0x10,
    COMnC1 = 0x08,
    COMnC0 = 0x04,
    WGMn1  = 0x02,
    WGMn0  = 0x01,
    // TCCR1B, TCCR3B
    ICnC1  = 0x80,
    ICESn  = 0x40,
    WGMn3  = 0x10,
    WGMn2  = 0x08,
    CSn2   = 0x04,
    CSn1   = 0x02,
    CSn0   = 0x01,
    // TCCR1C, TCCR3C
    FOCnA  = 0x80,
    FOCnB  = 0x40,
    FOCnC  = 0x10,
    // TCCR2
    FOC2   = 0x80,
    WGM20  = 0x40,
    COM21  = 0x20,
    COM20  = 0x10,
    WGM21  = 0x08,
    CS22   = 0x04,
    CS21   = 0x02,
    CS20   = 0x01,
#endif
    };

    // Register Settings
    enum {
    	TIMER_PRESCALE_1    = CSn0,
	TIMER_PRESCALE_8    = CSn1,
	TIMER_PRESCALE_64   = CSn1 | CSn0,
	TIMER_PRESCALE_256  = CSn2,
	TIMER_PRESCALE_1024 = CSn2 | CSn0
    };

public:
#if defined (__AVR_AT90CAN128__)
    static Reg8 timsk0() { return AVR8::in8(IO::TIMSK0); }
    static void timsk0(Reg8 value) { AVR8::out8(IO::TIMSK0,value); } 
    static Reg8 timsk1() { return AVR8::in8(IO::TIMSK1); }
    static void timsk1(Reg8 value) { AVR8::out8(IO::TIMSK1,value); } 
    static Reg8 timsk2() { return AVR8::in8(IO::TIMSK2); }
    static void timsk2(Reg8 value) { AVR8::out8(IO::TIMSK2,value); } 
    static Reg8 timsk3() { return AVR8::in8(IO::TIMSK3); }
    static void timsk3(Reg8 value) { AVR8::out8(IO::TIMSK3,value); } 
    static Reg8 tifr0() { return AVR8::in8(IO::TIFR0); }
    static void tifr0(Reg8 value) { AVR8::out8(IO::TIFR0,value); } 
    static Reg8 tifr1() { return AVR8::in8(IO::TIFR1); }
    static void tifr1(Reg8 value) { AVR8::out8(IO::TIFR1,value); } 
    static Reg8 tifr2() { return AVR8::in8(IO::TIFR2); }
    static void tifr2(Reg8 value) { AVR8::out8(IO::TIFR2,value); } 
    static Reg8 tifr3() { return AVR8::in8(IO::TIFR3); }
    static void tifr3(Reg8 value) { AVR8::out8(IO::TIFR3,value); } 
    static Reg8 gtccr() { return AVR8::in8(IO::GTCCR); }
    static void gtccr(Reg8 value) { AVR8::out8(IO::GTCCR,value); }
    static Reg8 adcsrb() { return AVR8::in8(IO::ADCSRB); }
    static void adcsrb(Reg8 value) { AVR8::out8(IO::ADCSRB,value); }
    static Reg8 mcucr() { return AVR8::in8(IO::MCUCR); }
    static void mcucr(Reg8 value) { AVR8::out8(IO::MCUCR,value); }
    static Reg8 tccr0a() { return AVR8::in8(IO::TCCR0A); }
    static void tccr0a(Reg8 value) { AVR8::out8(IO::TCCR0A,value); }
    static Reg8 ocr0a() { return AVR8::in8(IO::OCR0A); }
    static void ocr0a(Reg8 value) { AVR8::out8(IO::OCR0A,value); }
    static Reg8 tccr2a() { return AVR8::in8(IO::TCCR2A); }
    static void tccr2a(Reg8 value) { AVR8::out8(IO::TCCR2A,value); }
    static Reg8 ocr2a() { return AVR8::in8(IO::OCR2A); }
    static void ocr2a(Reg8 value) { AVR8::out8(IO::OCR2A,value); }
#elif defined (__AVR_ATmega1281__)
    static Reg8 timsk0() { return AVR8::in8(IO::TIMSK0); }
    static void timsk0(Reg8 value) { AVR8::out8(IO::TIMSK0,value); } 
    static Reg8 timsk1() { return AVR8::in8(IO::TIMSK1); }
    static void timsk1(Reg8 value) { AVR8::out8(IO::TIMSK1,value); } 
    static Reg8 timsk2() { return AVR8::in8(IO::TIMSK2); }
    static void timsk2(Reg8 value) { AVR8::out8(IO::TIMSK2,value); } 
    static Reg8 timsk3() { return AVR8::in8(IO::TIMSK3); }
    static void timsk3(Reg8 value) { AVR8::out8(IO::TIMSK3,value); } 
    static Reg8 timsk4() { return AVR8::in8(IO::TIMSK4); }
    static void timsk4(Reg8 value) { AVR8::out8(IO::TIMSK4,value); } 
    static Reg8 timsk5() { return AVR8::in8(IO::TIMSK5); }
    static void timsk5(Reg8 value) { AVR8::out8(IO::TIMSK5,value); } 
    static Reg8 tifr0() { return AVR8::in8(IO::TIFR0); }
    static void tifr0(Reg8 value) { AVR8::out8(IO::TIFR0,value); } 
    static Reg8 tifr1() { return AVR8::in8(IO::TIFR1); }
    static void tifr1(Reg8 value) { AVR8::out8(IO::TIFR1,value); } 
    static Reg8 tifr2() { return AVR8::in8(IO::TIFR2); }
    static void tifr2(Reg8 value) { AVR8::out8(IO::TIFR2,value); } 
    static Reg8 tifr3() { return AVR8::in8(IO::TIFR3); }
    static void tifr3(Reg8 value) { AVR8::out8(IO::TIFR3,value); } 
    static Reg8 tifr4() { return AVR8::in8(IO::TIFR4); }
    static void tifr4(Reg8 value) { AVR8::out8(IO::TIFR4,value); } 
    static Reg8 tifr5() { return AVR8::in8(IO::TIFR5); }
    static void tifr5(Reg8 value) { AVR8::out8(IO::TIFR5,value); } 
    static Reg8 gtccr() { return AVR8::in8(IO::GTCCR); }
    static void gtccr(Reg8 value) { AVR8::out8(IO::GTCCR,value); }
    static Reg8 adcsrb() { return AVR8::in8(IO::ADCSRB); }
    static void adcsrb(Reg8 value) { AVR8::out8(IO::ADCSRB,value); }
    static Reg8 mcucr() { return AVR8::in8(IO::MCUCR); }
    static void mcucr(Reg8 value) { AVR8::out8(IO::MCUCR,value); }
    static Reg8 tccr0b() { return AVR8::in8(IO::TCCR0B); }
    static void tccr0b(Reg8 value) { AVR8::out8(IO::TCCR0B,value); }
    static Reg8 tccr0a() { return AVR8::in8(IO::TCCR0A); }
    static void tccr0a(Reg8 value) { AVR8::out8(IO::TCCR0A,value); }
    static Reg8 ocr0b() { return AVR8::in8(IO::OCR0B); }
    static void ocr0b(Reg8 value) { AVR8::out8(IO::OCR0B,value); }
    static Reg8 ocr0a() { return AVR8::in8(IO::OCR0A); }
    static void ocr0a(Reg8 value) { AVR8::out8(IO::OCR0A,value); }
    static Reg8 tccr2b() { return AVR8::in8(IO::TCCR2B); }
    static void tccr2b(Reg8 value) { AVR8::out8(IO::TCCR2B,value); }
    static Reg8 tccr2a() { return AVR8::in8(IO::TCCR2A); }
    static void tccr2a(Reg8 value) { AVR8::out8(IO::TCCR2A,value); }
    static Reg8 ocr2b() { return AVR8::in8(IO::OCR2B); }
    static void ocr2b(Reg8 value) { AVR8::out8(IO::OCR2B,value); }
    static Reg8 ocr2a() { return AVR8::in8(IO::OCR2A); }
    static void ocr2a(Reg8 value) { AVR8::out8(IO::OCR2A,value); }
#else
    static Reg8 timsk() { return AVR8::in8(IO::TIMSK); }
    static void timsk(Reg8 value) { AVR8::out8(IO::TIMSK,value); }
    static Reg8 etimsk() { return AVR8::in8(IO::ETIMSK); }
    static void etimsk(Reg8 value) { AVR8::out8(IO::ETIMSK,value); }
    static Reg8 tifr() { return AVR8::in8(IO::TIFR); }
    static void tifr(Reg8 value) { AVR8::out8(IO::TIFR,value); }
    static Reg8 etifr() { return AVR8::in8(IO::ETIFR); }
    static void etifr(Reg8 value) { AVR8::out8(IO::ETIFR,value); }
    static Reg8 sfior() { return AVR8::in8(IO::SFIOR); }
    static void sfior(Reg8 value) { AVR8::out8(IO::SFIOR,value); }
    static Reg8 tccr0() { return AVR8::in8(IO::TCCR0); }
    static void tccr0(Reg8 value) { AVR8::out8(IO::TCCR0,value); }
    static Reg8 ocr0() { return AVR8::in8(IO::OCR0); }
    static void ocr0(Reg8 value) { AVR8::out8(IO::OCR0,value); }
    static Reg8 tccr2() { return AVR8::in8(IO::TCCR2); }
    static void tccr2(Reg8 value) { AVR8::out8(IO::TCCR2,value); }
    static Reg8 ocr2() { return AVR8::in8(IO::OCR2); }
    static void ocr2(Reg8 value) { AVR8::out8(IO::OCR2,value); }
#endif
    static Reg8 tcnt0() { return AVR8::in8(IO::TCNT0); }
    static void tcnt0(Reg8 value) { AVR8::out8(IO::TCNT0,value); }
    static Reg8 tcnt2() { return AVR8::in8(IO::TCNT2); }
    static void tcnt2(Reg8 value) { AVR8::out8(IO::TCNT2,value); }
    static Reg8 assr() { return AVR8::in8(IO::ASSR); }
    static void assr(Reg8 value) { AVR8::out8(IO::ASSR,value); }
    static Reg8 tccr1a() { return AVR8::in8(IO::TCCR1A); }
    static void tccr1a(Reg8 value) { AVR8::out8(IO::TCCR1A,value); }
    static Reg8 tccr1b() { return AVR8::in8(IO::TCCR1B); }
    static void tccr1b(Reg8 value) { AVR8::out8(IO::TCCR1B,value); }
    static Reg8 tccr1c() { return AVR8::in8(IO::TCCR1C); }
    static void tccr1c(Reg8 value) { AVR8::out8(IO::TCCR1C,value); }
    static Reg16 tcnt1() { return AVR8::in16(IO::TCNT1L); }
    static void tcnt1(Reg16 value) { AVR8::out16(IO::TCNT1L,value); }
    static Reg8 tcnt1h() { return AVR8::in8(IO::TCNT1H); }
    static void tcnt1h(Reg8 value) { AVR8::out8(IO::TCNT1H,value); }
    static Reg8 tcnt1l() { return AVR8::in8(IO::TCNT1L); }
    static void tcnt1l(Reg8 value) { AVR8::out8(IO::TCNT1L,value); }
    static Reg8 ocr1ah() { return AVR8::in8(IO::OCR1AH); }
    static void ocr1ah(Reg8 value) { AVR8::out8(IO::OCR1AH,value); }
    static Reg16 ocr1a() { return AVR8::in16(IO::OCR1AL); }
    static void ocr1a(Reg16 value) { return AVR8::out16(IO::OCR1AL, value); }
    static Reg8 ocr1al() { return AVR8::in8(IO::OCR1AL); }
    static void ocr1al(Reg8 value) { AVR8::out8(IO::OCR1AL,value); }
    static Reg8 ocr1bh() { return AVR8::in8(IO::OCR1BH); }
    static void ocr1bh(Reg8 value) { AVR8::out8(IO::OCR1BH,value); }
    static Reg8 ocr1bl() { return AVR8::in8(IO::OCR1BL); }
    static void ocr1bl(Reg8 value) { AVR8::out8(IO::OCR1BL,value); }
    static Reg8 ocr1ch() { return AVR8::in8(IO::OCR1CH); }
    static void ocr1ch(Reg8 value) { AVR8::out8(IO::OCR1CH,value); }
    static Reg8 ocr1cl() { return AVR8::in8(IO::OCR1CL); }
    static void ocr1cl(Reg8 value) { AVR8::out8(IO::OCR1CL,value); }
    static Reg8 icr1h() { return AVR8::in8(IO::ICR1H); }
    static void icr1h(Reg8 value) { AVR8::out8(IO::ICR1H,value); }
    static Reg8 icr1l() { return AVR8::in8(IO::ICR1L); }
    static void icr1l(Reg8 value) { AVR8::out8(IO::ICR1L,value); }
    static Reg8 tccr3a() { return AVR8::in8(IO::TCCR3A); }
    static void tccr3a(Reg8 value) { AVR8::out8(IO::TCCR3A,value); }
    static Reg8 tccr3b() { return AVR8::in8(IO::TCCR3B); }
    static void tccr3b(Reg8 value) { AVR8::out8(IO::TCCR3B,value); }
    static Reg8 tccr3c() { return AVR8::in8(IO::TCCR3C); }
    static void tccr3c(Reg8 value) { AVR8::out8(IO::TCCR3C,value); }
    static Reg8 tcnt3h() { return AVR8::in8(IO::TCNT3H); }
    static void tcnt3h(Reg8 value) { AVR8::out8(IO::TCNT3H,value); }
    static Reg8 tcnt3l() { return AVR8::in8(IO::TCNT3L); }
    static void tcnt3l(Reg8 value) { AVR8::out8(IO::TCNT3L,value); }
    static Reg16 tcnt3() { return AVR8::in16(IO::TCNT3L); }
    static void tcnt3(Reg16 value) { AVR8::out16(IO::TCNT3L,value); }
    static Reg8 ocr3ah() { return AVR8::in8(IO::OCR3AH); }
    static void ocr3ah(Reg8 value) { AVR8::out8(IO::OCR3AH,value); }
    static Reg8 ocr3al() { return AVR8::in8(IO::OCR3AL); }
    static void ocr3al(Reg8 value) { AVR8::out8(IO::OCR3AL,value); }
    static Reg16 ocr3a (){ return AVR8::in16(IO::OCR3AL);}
    static void ocr3a(Reg16 value) { AVR8::out16(IO::OCR3AL,value);}
    static Reg8 ocr3bh() { return AVR8::in8(IO::OCR3BH); }
    static void ocr3bh(Reg8 value) { AVR8::out8(IO::OCR3BH,value); }
    static Reg8 ocr3bl() { return AVR8::in8(IO::OCR3BL); }
    static void ocr3bl(Reg8 value) { AVR8::out8(IO::OCR3BL,value); }
    static Reg16 ocr3b (){ return AVR8::in16(IO::OCR3BL);}
    static void ocr3b(Reg16 value) { AVR8::out16(IO::OCR3BL,value);}
    static Reg8 ocr3ch() { return AVR8::in8(IO::OCR3CH); }
    static void ocr3ch(Reg8 value) { AVR8::out8(IO::OCR3CH,value); }
    static Reg8 ocr3cl() { return AVR8::in8(IO::OCR3CL); }
    static void ocr3cl(Reg8 value) { AVR8::out8(IO::OCR3CL,value); }
    static Reg16 ocr3c (){ return AVR8::in16(IO::OCR3CL);}
    static void ocr3c(Reg16 value) { AVR8::out16(IO::OCR3CL,value);}
    static Reg8 icr3h() { return AVR8::in8(IO::ICR3H); }
    static void icr3h(Reg8 value) { AVR8::out8(IO::ICR3H,value); }
    static Reg8 icr3l() { return AVR8::in8(IO::ICR3L); }
    static void icr3l(Reg8 value) { AVR8::out8(IO::ICR3L,value); }
    static Reg16 icr3() { return AVR8::in16(IO::ICR3L); }
    static void icr3(Reg8 value) { AVR8::out16(IO::ICR3L,value); }
};

__END_SYS

#endif
