// EPOS ATMega1281 Interrupt Controller Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega1281_ic_h
#define __atmega1281_ic_h

#include <system/memory_map.h>
#include <cpu.h>
#include "../avr_common/ic.h"

__BEGIN_SYS

class ATMega1281_IC: public IC_Common, private AVR_IC
{
public:
    static const unsigned int INTS = 57;
    typedef void (* Interrupt_Handler)();
private:
    typedef IO_Map<Machine> IO;

    // Interrupt IO Registers
    enum {	
	EIMSK   = IO::EIMSK, 
	EIFR    = IO::EIFR,
        PCMSK0  = IO::PCMSK0,
        WDTCSR  = IO::WDTCSR,
	TIMSK0  = IO::TIMSK0,
	TIMSK1  = IO::TIMSK1, 
	TIMSK2  = IO::TIMSK2,  
	TIMSK3  = IO::TIMSK3,
	TIMSK4  = IO::TIMSK4,
	TIMSK5  = IO::TIMSK5, 
	TIFR0   = IO::TIFR0,
	TIFR1   = IO::TIFR1,
	TIFR2   = IO::TIFR2,
	TIFR3   = IO::TIFR3,
	TIFR4   = IO::TIFR4,
	TIFR5   = IO::TIFR5,
	SPCR    = IO::SPCR,  
	SPSR    = IO::SPSR,
	UCSR0A  = IO::UCSR0A, 
	UCSR0B  = IO::UCSR0B,
	UCSR0C  = IO::UCSR0C,
	UCSR1A  = IO::UCSR1A,
	UCSR1B  = IO::UCSR1B,
	UCSR1C  = IO::UCSR1C,
	ADCSRA  = IO::ADCSRA, 
	ADCSRB  = IO::ADCSRB, 
	EECR    = IO::EECR,  
	ACSR    = IO::ACSR,
        TWCR    = IO::TWCR,
        SPMCSR  = IO::SPMCSR,
    };    
    
    // Interrupt Register Flags
    enum {
	// EIMSK
	INT7     = 0x80,
	INT6     = 0x40,
	INT5     = 0x20,
	INT4     = 0x10,
	INT3     = 0x08,
	INT2     = 0x04,
	INT1     = 0x02,
	INT0     = 0x01,
	// EIFR
	INTF7    = 0x80,
	INTF6    = 0x40,
	INTF5    = 0x20,
	INTF4    = 0x10,
	INTF3    = 0x08,
	INTF2    = 0x04,
	INTF1    = 0x02,
	INTF0    = 0x01,
        // PCMSK0
        PCINT2   = 0x04,
        PCINT1   = 0x02,
        PCINT0   = 0x01,
        // WDTCSR
        WDIE     = 0x40,
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
	TOIE2	 = 0x01,
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
	// TWCR
	TWINT	 = 0x80,
	TWIE	 = 0x01,
	// SPCR
	SPIE	 = 0x80,
	// SPSR
	SPIF	 = 0x80,
	// UCSRnA
	RXCn	 = 0x80,
	TXCn	 = 0x40,
	UDREn	 = 0x20,
	// UCSRnB
	RXCIEn	 = 0x80,
	TXCIEn	 = 0x40,
	UDRIEn	 = 0x20,
	//ADCSRA
	ADIF	 = 0x10,
	ADIE	 = 0x08,
	// EECR
	EERIE	 = 0x08,
	// ACSR
	ACI	 = 0x10,
	ACIE	 = 0x08,
        // SPMCSR
        SPMIE    = 0x80
    };

public:

    enum {
	IRQ_RESET        = 0,
	IRQ_IRQ0         = 1,
	IRQ_IRQ1         = 2,
	IRQ_IRQ2         = 3,
	IRQ_IRQ3         = 4,
	IRQ_IRQ4         = 5,
	IRQ_IRQ5         = 6,
	IRQ_IRQ6         = 7,
	IRQ_IRQ7         = 8,
        IRQ_PCINT0       = 9,
        IRQ_PCINT1       = 10,
        IRQ_PCINT2       = 11,
        IRQ_WDT          = 12,
        IRQ_TIMER2_COMPA = 13,
        IRQ_TIMER2_COMPB = 14,
	IRQ_TIMER2_OVF   = 15,
	IRQ_TIMER1_CAPT  = 16,
	IRQ_TIMER1_COMPA = 17,
	IRQ_TIMER1_COMPB = 18,
	IRQ_TIMER1_COMPC = 19,
	IRQ_TIMER1_OVF   = 20,
        IRQ_TIMER0_COMPA = 21,
        IRQ_TIMER0_COMPB = 22,
	IRQ_TIMER0_OVF   = 23,
	IRQ_SPI_STC      = 24,
	IRQ_USART0_RXC   = 25,
	IRQ_USART0_UDRE  = 26,
	IRQ_USART0_TXC   = 27,
	IRQ_ANA_COMP     = 28,
	IRQ_ADC          = 29,
	IRQ_EE_RDY       = 30,
	IRQ_TIMER3_CAPT  = 31,
	IRQ_TIMER3_COMPA = 32,
	IRQ_TIMER3_COMPB = 33,
	IRQ_TIMER3_COMPC = 34,
	IRQ_TIMER3_OVF   = 35,
	IRQ_USART1_RXC   = 36,
	IRQ_USART1_UDRE  = 37,
	IRQ_USART1_TXC   = 38,
	IRQ_TWI          = 39,
	IRQ_SPM_RDY      = 40,
	IRQ_TIMER4_CAPT  = 41,
	IRQ_TIMER4_COMPA = 42,
	IRQ_TIMER4_COMPB = 43,
	IRQ_TIMER4_COMPC = 44,
	IRQ_TIMER4_OVF   = 45,
	IRQ_TIMER5_CAPT  = 46,
	IRQ_TIMER5_COMPA = 47,
	IRQ_TIMER5_COMPB = 48,
	IRQ_TIMER5_COMPC = 49,
	IRQ_TIMER5_OVF   = 50,
	IRQ_USART2_RXC   = 51,
	IRQ_USART2_UDRE  = 52,
	IRQ_USART2_TXC   = 53,
	IRQ_USART3_RXC   = 54,
	IRQ_USART3_UDRE  = 55,
	IRQ_USART3_TXC   = 56,
    };

public:
    ATMega1281_IC() {};

    static Interrupt_Handler int_vector(Interrupt_Id i) {
    return (i < INTS) ? _int_vector[i] : 0;
    }

    static void int_vector(Interrupt_Id i, Interrupt_Handler h) {
    db<IC>(INF) << "PC_IC::int_vector(int=" << i << ",h=" 
            << (void *)h <<")\n";
    if(i < INTS) _int_vector[i] = h;
    }

    static void enable(Interrupt_Id irq) {

	if (irq <= IRQ_IRQ7) {
	    eimsk(eimsk() | 1 << (irq - 1));
	    return;
	}
	
	switch (irq) {
            case IRQ_PCINT0:          pcmsk0(pcmsk0() | PCINT0); break;
            case IRQ_PCINT1:          pcmsk0(pcmsk0() | PCINT1); break;
            case IRQ_PCINT2:          pcmsk0(pcmsk0() | PCINT2); break;
            case IRQ_WDT:             wdtcsr(wdtcsr() | WDIE)  ; break;
            case IRQ_TIMER2_COMPA:    timsk2(timsk2() | OCIE2A); break;
            case IRQ_TIMER2_COMPB:    timsk2(timsk2() | OCIE2B); break;
            case IRQ_TIMER2_OVF:      timsk2(timsk2() | TOIE2) ; break;
            case IRQ_TIMER1_CAPT:     timsk1(timsk1() | ICIE1) ; break;
            case IRQ_TIMER1_COMPA:    timsk1(timsk1() | OCIE1A); break;
            case IRQ_TIMER1_COMPB:    timsk1(timsk1() | OCIE1B); break;
            case IRQ_TIMER1_COMPC:    timsk1(timsk1() | OCIE1C); break;
            case IRQ_TIMER1_OVF:      timsk1(timsk1() | TOIE1) ; break;
            case IRQ_TIMER0_COMPA:    timsk0(timsk0() | OCIE0A); break;
            case IRQ_TIMER0_COMPB:    timsk0(timsk0() | OCIE0B); break;
            case IRQ_TIMER0_OVF:      timsk0(timsk0() | TOIE0) ; break;
	    case IRQ_SPI_STC:         spcr(spcr()     | SPIE)  ; break;
	    case IRQ_USART0_RXC:      ucsr0b(ucsr0b() | RXCIEn); break;
	    case IRQ_USART0_UDRE:     ucsr0b(ucsr0b() | UDRIEn); break;
	    case IRQ_USART0_TXC:      ucsr0b(ucsr0b() | TXCIEn); break;
	    case IRQ_ANA_COMP:        acsr(acsr()     | ACIE)  ; break;
	    case IRQ_ADC:             adcsra(adcsra() | ADIE)  ; break;
	    case IRQ_EE_RDY:          eecr(eecr()     | EERIE) ; break;
	    case IRQ_TIMER3_CAPT:     timsk3(timsk3() | ICIE3) ; break;
	    case IRQ_TIMER3_COMPA:    timsk3(timsk3() | OCIE3A); break;
	    case IRQ_TIMER3_COMPB:    timsk3(timsk3() | OCIE3B); break;
	    case IRQ_TIMER3_COMPC:    timsk3(timsk3() | OCIE3C); break;
	    case IRQ_TIMER3_OVF:      timsk3(timsk3() | TOIE3) ; break;
	    case IRQ_USART1_RXC:      ucsr1b(ucsr1b() | RXCIEn); break;
	    case IRQ_USART1_UDRE:     ucsr1b(ucsr1b() | UDRIEn); break;
	    case IRQ_USART1_TXC:      ucsr1b(ucsr1b() | TXCIEn); break;
            case IRQ_TWI:             twcr(twcr()     | TWIE)  ; break;
            case IRQ_SPM_RDY:         spmcsr(spmcsr() | SPMIE) ; break;
	    case IRQ_TIMER4_CAPT:     timsk4(timsk4() | ICIE4) ; break;
	    case IRQ_TIMER4_COMPA:    timsk4(timsk4() | OCIE4A); break;
	    case IRQ_TIMER4_COMPB:    timsk4(timsk4() | OCIE4B); break;
	    case IRQ_TIMER4_COMPC:    timsk4(timsk4() | OCIE4C); break;
	    case IRQ_TIMER4_OVF:      timsk4(timsk4() | TOIE4) ; break;
	    case IRQ_TIMER5_CAPT:     timsk5(timsk5() | ICIE5) ; break;
	    case IRQ_TIMER5_COMPA:    timsk5(timsk5() | OCIE5A); break;
	    case IRQ_TIMER5_COMPB:    timsk5(timsk5() | OCIE5B); break;
	    case IRQ_TIMER5_COMPC:    timsk5(timsk5() | OCIE5C); break;
	    case IRQ_TIMER5_OVF:      timsk5(timsk5() | TOIE5) ; break;
	}
    }
    
    static void disable() {
        eimsk(0);
	timsk0(0); 
	timsk1(0); 
	timsk2(0); 
	timsk3(0); 
	timsk4(0); 
	timsk5(0); 
        spcr(spcr() & ~SPIE);
        ucsr0b(ucsr0b() & ~(RXCIEn | UDRIEn | TXCIEn)); 
        ucsr1b(ucsr1b() & ~(RXCIEn | UDRIEn | TXCIEn));
        adcsra(adcsra() & ~ADIE);
        acsr(acsr() & ~ACIE);
        eecr(eecr() & ~EERIE);
    }

    static void disable(Interrupt_Id irq) {

	if (irq <= IRQ_IRQ7) {
	    eimsk(eimsk() & ~(1 << (irq - 1)));
	    return;
	}

	switch (irq) {
            case IRQ_PCINT0:          pcmsk0(pcmsk0() & ~PCINT0); break;
            case IRQ_PCINT1:          pcmsk0(pcmsk0() & ~PCINT1); break;
            case IRQ_PCINT2:          pcmsk0(pcmsk0() & ~PCINT2); break;
            case IRQ_WDT:             wdtcsr(wdtcsr() & ~WDIE)  ; break;
            case IRQ_TIMER2_COMPA:    timsk2(timsk2() & ~OCIE2A); break;
            case IRQ_TIMER2_COMPB:    timsk2(timsk2() & ~OCIE2B); break;
            case IRQ_TIMER2_OVF:      timsk2(timsk2() & ~TOIE2) ; break;
            case IRQ_TIMER1_CAPT:     timsk1(timsk1() & ~ICIE1) ; break;
            case IRQ_TIMER1_COMPA:    timsk1(timsk1() & ~OCIE1A); break;
            case IRQ_TIMER1_COMPB:    timsk1(timsk1() & ~OCIE1B); break;
            case IRQ_TIMER1_COMPC:    timsk1(timsk1() & ~OCIE1C); break;
            case IRQ_TIMER1_OVF:      timsk1(timsk1() & ~TOIE1) ; break;
            case IRQ_TIMER0_COMPA:    timsk0(timsk0() & ~OCIE0A); break;
            case IRQ_TIMER0_COMPB:    timsk0(timsk0() & ~OCIE0B); break;
            case IRQ_TIMER0_OVF:      timsk0(timsk0() & ~TOIE0) ; break;
	    case IRQ_SPI_STC:         spcr(spcr()     & ~SPIE)  ; break;
	    case IRQ_USART0_RXC:      ucsr0b(ucsr0b() & ~RXCIEn); break;
	    case IRQ_USART0_UDRE:     ucsr0b(ucsr0b() & ~UDRIEn); break;
	    case IRQ_USART0_TXC:      ucsr0b(ucsr0b() & ~TXCIEn); break;
	    case IRQ_ANA_COMP:        acsr(acsr()     & ~ACIE)  ; break;
	    case IRQ_ADC:             adcsra(adcsra() & ~ADIE)  ; break;
	    case IRQ_EE_RDY:          eecr(eecr()     & ~EERIE) ; break;
	    case IRQ_TIMER3_CAPT:     timsk3(timsk3() & ~ICIE3) ; break;
	    case IRQ_TIMER3_COMPA:    timsk3(timsk3() & ~OCIE3A); break;
	    case IRQ_TIMER3_COMPB:    timsk3(timsk3() & ~OCIE3B); break;
	    case IRQ_TIMER3_COMPC:    timsk3(timsk3() & ~OCIE3C); break;
	    case IRQ_TIMER3_OVF:      timsk3(timsk3() & ~TOIE3) ; break;
	    case IRQ_USART1_RXC:      ucsr1b(ucsr1b() & ~RXCIEn); break;
	    case IRQ_USART1_UDRE:     ucsr1b(ucsr1b() & ~UDRIEn); break;
	    case IRQ_USART1_TXC:      ucsr1b(ucsr1b() & ~TXCIEn); break; 
            case IRQ_TWI:             twcr(twcr()     & ~TWIE)  ; break;
            case IRQ_SPM_RDY:         spmcsr(spmcsr() & ~SPMIE) ; break;
	    case IRQ_TIMER4_CAPT:     timsk4(timsk4() & ~ICIE4) ; break;
	    case IRQ_TIMER4_COMPA:    timsk4(timsk4() & ~OCIE4A); break;
	    case IRQ_TIMER4_COMPB:    timsk4(timsk4() & ~OCIE4B); break;
	    case IRQ_TIMER4_COMPC:    timsk4(timsk4() & ~OCIE4C); break;
	    case IRQ_TIMER4_OVF:      timsk4(timsk4() & ~TOIE4) ; break;
	    case IRQ_TIMER5_CAPT:     timsk5(timsk5() & ~ICIE5) ; break;
	    case IRQ_TIMER5_COMPA:    timsk5(timsk5() & ~OCIE5A); break;
	    case IRQ_TIMER5_COMPB:    timsk5(timsk5() & ~OCIE5B); break;
	    case IRQ_TIMER5_COMPC:    timsk5(timsk5() & ~OCIE5C); break;
	    case IRQ_TIMER5_OVF:      timsk5(timsk5() & ~TOIE5) ; break;
	}
    }
    
private:

    typedef AVR8::Reg8 Reg8;

    static Reg8 eimsk() { return AVR8::in8(EIMSK); }
    static void eimsk(Reg8 value) { AVR8::out8(EIMSK,value); } 
    static Reg8 eifr() { return AVR8::in8(EIFR); }
    static void eifr(Reg8 value) { AVR8::out8(EIFR,value); } 
    static Reg8 pcmsk0() { return AVR8::in8(PCMSK0); }
    static void pcmsk0(Reg8 value) { AVR8::out8(PCMSK0,value); }
    static Reg8 wdtcsr() { return AVR8::in8(WDTCSR); }
    static void wdtcsr(Reg8 value) { AVR8::out8(WDTCSR,value); }
    static Reg8 timsk0() { return AVR8::in8(TIMSK0); }
    static void timsk0(Reg8 value) { AVR8::out8(TIMSK0,value); } 
    static Reg8 timsk1() { return AVR8::in8(TIMSK1); }
    static void timsk1(Reg8 value) { AVR8::out8(TIMSK1,value); } 
    static Reg8 timsk2() { return AVR8::in8(TIMSK2); }
    static void timsk2(Reg8 value) { AVR8::out8(TIMSK2,value); } 
    static Reg8 timsk3() { return AVR8::in8(TIMSK3); }
    static void timsk3(Reg8 value) { AVR8::out8(TIMSK3,value); } 
    static Reg8 timsk4() { return AVR8::in8(TIMSK4); }
    static void timsk4(Reg8 value) { AVR8::out8(TIMSK4,value); } 
    static Reg8 timsk5() { return AVR8::in8(TIMSK5); }
    static void timsk5(Reg8 value) { AVR8::out8(TIMSK5,value); } 
    static Reg8 tifr0() { return AVR8::in8(TIFR0); }
    static void tifr0(Reg8 value) { AVR8::out8(TIFR0,value); } 
    static Reg8 tifr1() { return AVR8::in8(TIFR1); }
    static void tifr1(Reg8 value) { AVR8::out8(TIFR1,value); } 
    static Reg8 tifr2() { return AVR8::in8(TIFR2); }
    static void tifr2(Reg8 value) { AVR8::out8(TIFR2,value); } 
    static Reg8 tifr3() { return AVR8::in8(TIFR3); }
    static void tifr3(Reg8 value) { AVR8::out8(TIFR3,value); } 
    static Reg8 tifr4() { return AVR8::in8(TIFR4); }
    static void tifr4(Reg8 value) { AVR8::out8(TIFR4,value); } 
    static Reg8 tifr5() { return AVR8::in8(TIFR5); }
    static void tifr5(Reg8 value) { AVR8::out8(TIFR5,value); } 
    static Reg8 spcr() { return AVR8::in8(SPCR); }
    static void spcr(Reg8 value) { AVR8::out8(SPCR,value); } 
    static Reg8 spsr() { return AVR8::in8(SPSR); }
    static void spsr(Reg8 value) { AVR8::out8(SPSR,value); } 
    static Reg8 ucsr0a() { return AVR8::in8(UCSR0A); }
    static void ucsr0a(Reg8 value) { AVR8::out8(UCSR0A,value); } 
    static Reg8 ucsr0b() { return AVR8::in8(UCSR0B); }
    static void ucsr0b(Reg8 value) { AVR8::out8(UCSR0B,value); } 
    static Reg8 ucsr0c() { return AVR8::in8(UCSR0C); }
    static void ucsr0c(Reg8 value) { AVR8::out8(UCSR0C,value); } 
    static Reg8 ucsr1a() { return AVR8::in8(UCSR1A); }
    static void ucsr1a(Reg8 value) { AVR8::out8(UCSR1A,value); } 
    static Reg8 ucsr1b() { return AVR8::in8(UCSR1B); }
    static void ucsr1b(Reg8 value) { AVR8::out8(UCSR1B,value); } 
    static Reg8 ucsr1c() { return AVR8::in8(UCSR1C); }
    static void ucsr1c(Reg8 value) { AVR8::out8(UCSR1C,value); } 
    static Reg8 adcsra() { return AVR8::in8(ADCSRA); }
    static void adcsra(Reg8 value) { AVR8::out8(ADCSRA,value); } 
    static Reg8 adcsrb() { return AVR8::in8(ADCSRB); }
    static void adcsrb(Reg8 value) { AVR8::out8(ADCSRB,value); } 
    static Reg8 eecr() { return AVR8::in8(EECR); }
    static void eecr(Reg8 value) { AVR8::out8(EECR,value); } 
    static Reg8 acsr() { return AVR8::in8(ACSR); }
    static void acsr(Reg8 value) { AVR8::out8(ACSR,value); } 
    static Reg8 twcr() { return AVR8::in8(TWCR); }
    static void twcr(Reg8 value) { AVR8::out8(TWCR,value); }
    static Reg8 spmcsr() { return AVR8::in8(SPMCSR); }
    static void spmcsr(Reg8 value) { AVR8::out8(SPMCSR,value); }
private:
    static Interrupt_Handler _int_vector[INTS]; 
};

typedef ATMega1281_IC IC;

__END_SYS

#endif

