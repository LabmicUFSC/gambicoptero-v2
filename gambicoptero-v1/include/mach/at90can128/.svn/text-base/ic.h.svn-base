// EPOS AT90CAN128_IC Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __at90can128_ic_h
#define __at90can128_ic_h

#include <system/memory_map.h>
#include <cpu.h>
#include "../avr_common/ic.h"

__BEGIN_SYS

class AT90CAN128_IC: public IC_Common, private AVR_IC
{
public:
    static const unsigned int INTS = 37;
    typedef void (* Interrupt_Handler)();
private:
    typedef IO_Map<Machine> IO;

    // Interrupt IO Registers
    enum {	
	EIMSK   = IO::EIMSK,   
	EIFR    = IO::EIFR,
    TIMSK0  = IO::TIMSK0,
    TIMSK1  = IO::TIMSK1,
    TIMSK2  = IO::TIMSK2,
    TIMSK3  = IO::TIMSK3,
    TIFR0   = IO::TIFR0,
    TIFR1   = IO::TIFR1,
    TIFR2   = IO::TIFR2,
    TIFR3   = IO::TIFR3,
	//TIMSK   = IO::TIMSK,
	//TIFR    = IO::TIFR,
	//ETIMSK  = IO::ETIMSK,
	//ETIFR   = IO::ETIFR,
	SPCR    = IO::SPCR,
	SPSR    = IO::SPSR,
	UCSR0A  = IO::UCSR0A,
	UCSR0B  = IO::UCSR0B,
	UCSR1A  = IO::UCSR1A,
	UCSR1B  = IO::UCSR1B,
	ADCSRA  = IO::ADCSRA,
	EECR    = IO::EECR,
	ACSR    = IO::ACSR,
    TWCR    = IO::TWCR,
    SPMCSR  = IO::SPMCSR,
    };
 
    // Interrupt Register Flags
    enum {
	// EIMSK AT90CAN128 OK
	INT7     = 0x80,
	INT6     = 0x40,
	INT5     = 0x20,
	INT4     = 0x10,
	INT3     = 0x08,
	INT2     = 0x04,
	INT1     = 0x02,
	INT0     = 0x01,
	// EIFR AT90CAN128 OK
	INTF7    = 0x80,
	INTF6    = 0x40,
	INTF5    = 0x20,
	INTF4    = 0x10,
	INTF3    = 0x08,
	INTF2    = 0x04,
	INTF1    = 0x02,
	INTF0    = 0x01,
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
	// TWCR THESE TWO ARE OK, BUT THERE ARE MORE
	TWINT	 = 0x80,
	TWIE	 = 0x01,
	// SPCR THIS IS OK, BUT THERE ARE MORE
	SPIE	 = 0x80,
	// SPSR THIS IS OK, BUT THERE ARE MORE
	SPIF	 = 0x80,
	// UCSRnA THESE ARE OK, BUT THERE ARE MORE
	RXCn	 = 0x80,
	TXCn	 = 0x40,
	UDREn	 = 0x20,
	// UCSRnB THESE ARE OK, BUT THERE ARE MORE
	RXCIEn	 = 0x80,
	TXCIEn	 = 0x40,
	UDRIEn	 = 0x20,
	//ADCSRA ADIE IS NOW 0x08, ADIF OK, THERE ARE MORE
	ADIF	 = 0x10,
	ADIE	 = 0x80,
	// EECR OK
	EERIE	 = 0x08,
	// ACSR ACIE IS NOW 0x08, ACI OK, THERE ARE MORE
	ACI	 = 0x10,
	ACIE	 = 0x80,
    // SPMCSR
    SPMIE    = 0x80
    };

public:

    // IRQs - 37
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
	IRQ_TIMER2_COMP  = 9,
	IRQ_TIMER2_OVF   = 10,
	IRQ_TIMER1_CAPT  = 11,
	IRQ_TIMER1_COMPA = 12,
	IRQ_TIMER1_COMPB = 13,
    IRQ_TIMER1_COMPC = 14,
	IRQ_TIMER1_OVF   = 15,
	IRQ_TIMER0_COMP  = 16,
	IRQ_TIMER0_OVF   = 17,
    IRQ_CANIT        = 18,
    IRQ_OVRIT        = 19,
    IRQ_SPI_STC      = 20,	
	IRQ_USART0_RXC   = 21,
	IRQ_USART0_UDRE  = 22,
	IRQ_USART0_TXC   = 23,
    IRQ_ANA_COMP     = 24,
    IRQ_ADC          = 25,
	IRQ_EE_RDY       = 26,
    IRQ_TIMER3_CAPT  = 27,
    IRQ_TIMER3_COMPA = 28,
    IRQ_TIMER3_COMPB = 29,
    IRQ_TIMER3_COMPC = 30,
	IRQ_TIMER3_OVF   = 31,
	IRQ_USART1_RXC   = 32,
    IRQ_USART1_UDRE  = 33,
    IRQ_USART1_TXC   = 34,
    IRQ_TWI          = 35,
    IRQ_SPM_RDY      = 36,
    IRQ_TSC      = IRQ_TIMER1_OVF,
    IRQ_TIMER    = IRQ_TIMER0_COMP
    };

public:

    AT90CAN128_IC()  {};

    static Interrupt_Handler int_vector(Interrupt_Id i) {
    return (i < INTS) ? _int_vector[i] : 0;
    }

    static void int_vector(Interrupt_Id i, Interrupt_Handler h) {
    db<IC>(INF) << "AT90CAN128_IC::int_vector(int=" << i << ",h="
            << (void *)h <<")\n";
    if(i < INTS) _int_vector[i] = h;
    }

    static void enable(Interrupt_Id irq) {

        if (irq <= IRQ_IRQ7) {
            eimsk(eimsk() | 1 << (irq - 1));
            return;
        }

        switch (irq) {
            case IRQ_TIMER2_COMP:       timsk2(timsk2() | OCIE2A); break;
            case IRQ_TIMER2_OVF:        timsk2(timsk2() | TOIE2) ; break;
            case IRQ_TIMER1_CAPT:       timsk1(timsk1() | ICIE1) ; break;
            case IRQ_TIMER1_COMPA:      timsk1(timsk1() | OCIE1A); break;
            case IRQ_TIMER1_COMPB:      timsk1(timsk1() | OCIE1B); break;
            case IRQ_TIMER1_COMPC:      timsk1(timsk1() | OCIE1C); break;
            case IRQ_TIMER1_OVF:        timsk1(timsk1() | TOIE1) ; break;
            case IRQ_TIMER0_COMP:       timsk0(timsk0() | OCIE0A); break;
            case IRQ_TIMER0_OVF:        timsk0(timsk0() | TOIE0); break;
            case IRQ_CANIT:             break;
            case IRQ_OVRIT:             break;
            case IRQ_SPI_STC:           spcr(spcr() | SPIE);        break;
            case IRQ_USART0_RXC:        ucsr0b(ucsr0b() | RXCIEn);  break;
            case IRQ_USART0_UDRE:       ucsr0b(ucsr0b() | UDRIEn);  break;
            case IRQ_USART0_TXC:        ucsr0b(ucsr0b() | TXCIEn);  break;
            case IRQ_ANA_COMP:          acsr(acsr()     | ACIE)  ; break;
            case IRQ_ADC:               adcsra(adcsra() | ADIE)  ; break;
            case IRQ_EE_RDY:            eecr(eecr()     | EERIE) ; break;
            case IRQ_TIMER3_CAPT:       timsk3(timsk3() | ICIE3);  break;
            case IRQ_TIMER3_COMPA:      timsk3(timsk3() | OCIE3A);  break;
            case IRQ_TIMER3_COMPB:      timsk3(timsk3() | OCIE3B);  break;
            case IRQ_TIMER3_COMPC:      timsk3(timsk3() | OCIE3C);  break;
            case IRQ_TIMER3_OVF:        timsk3(timsk3() | TOIE3);   break;
            case IRQ_USART1_RXC:        ucsr1b(ucsr1b() | RXCIEn);  break;
            case IRQ_USART1_UDRE:       ucsr1b(ucsr1b() | UDRIEn);  break;
            case IRQ_USART1_TXC:        ucsr1b(ucsr1b() | TXCIEn);  break;
            case IRQ_TWI:               twcr(twcr()     | TWIE)  ; break;
            case IRQ_SPM_RDY:           spmcsr(spmcsr() | SPMIE) ; break;
        }
    }

    static void disable() {
        eimsk(0);
        timsk0(0); 
        timsk1(0); 
        timsk2(0); 
        timsk3(0);
        spcr(spcr() & ~SPIE);
        ucsr0b(ucsr0b() & ~(RXCIEn | UDRIEn | TXCIEn));
        ucsr1b(ucsr1b() & ~(RXCIEn | UDRIEn | TXCIEn));
        adcsra(adcsra() & ~ADIE);
        acsr(acsr() & ~ACIE);
        eecr(eecr() & ~EERIE);
    }

    static void disable(Interrupt_Id irq) {

        if(irq <= IRQ_IRQ7) {
            eimsk(eimsk() & ~(1 << (irq - 1)));
            return;
        }

        switch (irq) {
            case IRQ_TIMER2_COMP:       timsk2(timsk2() & ~OCIE2A); break;
            case IRQ_TIMER2_OVF:        timsk2(timsk2() & ~TOIE2) ; break;
            case IRQ_TIMER1_CAPT:       timsk1(timsk1() & ~ICIE1) ; break;
            case IRQ_TIMER1_COMPA:      timsk1(timsk1() & ~OCIE1A); break;
            case IRQ_TIMER1_COMPB:      timsk1(timsk1() & ~OCIE1B); break;
            case IRQ_TIMER1_COMPC:      timsk1(timsk1() & ~OCIE1C); break;
            case IRQ_TIMER1_OVF:        timsk1(timsk1() & ~TOIE1) ; break;
            case IRQ_TIMER0_COMP:       timsk0(timsk0() & ~OCIE0A); break;
            case IRQ_TIMER0_OVF:        timsk0(timsk0() & ~TOIE0); break;
            case IRQ_CANIT:             break;
            case IRQ_OVRIT:             break;
            case IRQ_SPI_STC:           spcr(spcr() & ~SPIE);        break;
            case IRQ_USART0_RXC:        ucsr0b(ucsr0b() & ~RXCIEn);  break;
            case IRQ_USART0_UDRE:       ucsr0b(ucsr0b() & ~UDRIEn);  break;
            case IRQ_USART0_TXC:        ucsr0b(ucsr0b() & ~TXCIEn);  break;
            case IRQ_ANA_COMP:          acsr(acsr()     & ~ACIE)  ; break;
            case IRQ_ADC:               adcsra(adcsra() & ~ADIE)  ; break;
            case IRQ_EE_RDY:            eecr(eecr()     & ~EERIE) ; break;
            case IRQ_TIMER3_CAPT:       timsk3(timsk3() & ~ICIE3);  break;
            case IRQ_TIMER3_COMPA:      timsk3(timsk3() & ~OCIE3A);  break;
            case IRQ_TIMER3_COMPB:      timsk3(timsk3() & ~OCIE3B);  break;
            case IRQ_TIMER3_COMPC:      timsk3(timsk3() & ~OCIE3C);  break;
            case IRQ_TIMER3_OVF:        timsk3(timsk3() & ~TOIE3);   break;
            case IRQ_USART1_RXC:        ucsr1b(ucsr1b() & ~RXCIEn);  break;
            case IRQ_USART1_UDRE:       ucsr1b(ucsr1b() & ~UDRIEn);  break;
            case IRQ_USART1_TXC:        ucsr1b(ucsr1b() & ~TXCIEn);  break;
            case IRQ_TWI:               twcr(twcr()     & ~TWIE)  ; break;
            case IRQ_SPM_RDY:           spmcsr(spmcsr() & ~SPMIE) ; break;
        }
    }

private:

    typedef AVR8::Reg8 Reg8;

    static Reg8 eimsk() { return AVR8::in8(EIMSK); }
    static void eimsk(Reg8 value) { AVR8::out8(EIMSK,value); } 
    static Reg8 eifr() { return AVR8::in8(EIFR); }
    static void eifr(Reg8 value) { AVR8::out8(EIFR,value); } 
    static Reg8 timsk0() { return AVR8::in8(TIMSK0); }
    static void timsk0(Reg8 value) { AVR8::out8(TIMSK0,value); } 
    static Reg8 timsk1() { return AVR8::in8(TIMSK1); }
    static void timsk1(Reg8 value) { AVR8::out8(TIMSK1,value); } 
    static Reg8 timsk2() { return AVR8::in8(TIMSK2); }
    static void timsk2(Reg8 value) { AVR8::out8(TIMSK2,value); } 
    static Reg8 timsk3() { return AVR8::in8(TIMSK3); }
    static void timsk3(Reg8 value) { AVR8::out8(TIMSK3,value); } 
    static Reg8 tifr0() { return AVR8::in8(TIFR0); }
    static void tifr0(Reg8 value) { AVR8::out8(TIFR0,value); } 
    static Reg8 tifr1() { return AVR8::in8(TIFR1); }
    static void tifr1(Reg8 value) { AVR8::out8(TIFR1,value); } 
    static Reg8 tifr2() { return AVR8::in8(TIFR2); }
    static void tifr2(Reg8 value) { AVR8::out8(TIFR2,value); } 
    static Reg8 tifr3() { return AVR8::in8(TIFR3); }
    static void tifr3(Reg8 value) { AVR8::out8(TIFR3,value); } 
    //static Reg8 etimsk() { return AVR8::in8(ETIMSK);; }
    //static void etimsk(Reg8 value) { AVR8::out8(ETIMSK,value); }
    //static Reg8 etifr() { return AVR8::in8(ETIFR);; }
    //static void etifr(Reg8 value) { AVR8::out8(ETIFR,value); }
    static Reg8 spcr() { return AVR8::in8(SPCR); }
    static void spcr(Reg8 value) { AVR8::out8(SPCR,value); } 
    static Reg8 spsr() { return AVR8::in8(SPSR); }
    static void spsr(Reg8 value) { AVR8::out8(SPSR,value); } 
    static Reg8 ucsr0a() { return AVR8::in8(UCSR0A); }
    static void ucsr0a(Reg8 value) { AVR8::out8(UCSR0A,value); } 
    static Reg8 ucsr0b() { return AVR8::in8(UCSR0B); }
    static void ucsr0b(Reg8 value) { AVR8::out8(UCSR0B,value); } 
    static Reg8 adcsra() { return AVR8::in8(ADCSRA); }
    static void adcsra(Reg8 value) { AVR8::out8(ADCSRA,value); } 
    static Reg8 eecr() { return AVR8::in8(EECR); }
    static void eecr(Reg8 value) { AVR8::out8(EECR,value); } 
    static Reg8 acsr() { return AVR8::in8(ACSR); }
    static void acsr(Reg8 value) { AVR8::out8(ACSR,value); } 
    static Reg8 ucsr1a() { return AVR8::in8(UCSR1A); }
    static void ucsr1a(Reg8 value) { AVR8::out8(UCSR1A,value); } 
    static Reg8 ucsr1b() { return AVR8::in8(UCSR1B); }
    static void ucsr1b(Reg8 value) { AVR8::out8(UCSR1B,value); }
    static Reg8 twcr() { return AVR8::in8(TWCR); }
    static void twcr(Reg8 value) { AVR8::out8(TWCR,value); }
    static Reg8 spmcsr() { return AVR8::in8(SPMCSR); }
    static void spmcsr(Reg8 value) { AVR8::out8(SPMCSR,value); }

private:
    static Interrupt_Handler _int_vector[INTS]; 
};

typedef AT90CAN128_IC IC;

__END_SYS

#endif
