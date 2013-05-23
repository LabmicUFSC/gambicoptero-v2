// EPOS ATMega128 Interrupt Controller Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega128_ic_h
#define __atmega128_ic_h

#include <system/memory_map.h>
#include <cpu.h>
#include "../avr_common/ic.h"

__BEGIN_SYS

class ATMega128_IC: public IC_Common, private AVR_IC
{
public:
    static const unsigned int INTS = 35;
    typedef void (* Interrupt_Handler)();
private:
    typedef IO_Map<Machine> IO;

    // Interrupt IO Registers
    enum {	
	EIMSK   = IO::EIMSK,   // Manual Page 89
	EIFR    = IO::EIFR,
	TIMSK   = IO::TIMSK,   // Manual Page 138
	TIFR    = IO::TIFR,
	ETIMSK  = IO::ETIMSK,
	ETIFR   = IO::ETIFR,
	SPCR    = IO::SPCR,    // Manual Page 166
	SPSR    = IO::SPSR,
	UCSR0A  = IO::UCSR0A,  // Manual Page 189
	UCSR0B  = IO::UCSR0B,
	UCSR1A  = IO::UCSR1A,
	UCSR1B  = IO::UCSR1B,
	ADCSRA  = IO::ADCSRA,  // Manual Page 245
	EECR    = IO::EECR,    // Manual Page 20
	ACSR    = IO::ACSR     // Manual Page 228
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
	// TIMSK
	OCIE2	 = 0x80,
	TOIE2	 = 0x40,
	TICIE1	 = 0x20,
	OCIE1A	 = 0x10,
	OCIE1B	 = 0x08,
	TOIE1	 = 0x04,
	OCIE0	 = 0x02,
	TOIE0	 = 0x01,
	// TIFR
	OCF2	 = 0x80,
	TOV2	 = 0x40,
	ICF1	 = 0x20,
	OCF1A	 = 0x10,
	OCF1B	 = 0x08,
	TOV1	 = 0x04,
	OCF0	 = 0x02,
	TOV0	 = 0x01,
	// ETIMSK
	TICIE3	 = 0x20,
	OCIE3A	 = 0x10,
	OCIE3B	 = 0x08,
	TOIE3	 = 0x04,
	OCIE3C	 = 0x02,
	OCIE1C	 = 0x01,
	// ETIFR
	ICF3	 = 0x20,
	OCF3A	 = 0x10,
	OCF3B	 = 0x08,
	TOV3	 = 0x04,
	OCF3C	 = 0x02,
	OCF1C	 = 0x01,
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
	ADIE	 = 0x80,
	// EECR
	EERIE	 = 0x08,
	// ACSR
	ACI	 = 0x10,
	ACIE	 = 0x80
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
	IRQ_TIMER2_COMP  = 9,
	IRQ_TIMER2_OVF   = 10,
	IRQ_TIMER1_CAPT  = 11,
	IRQ_TIMER1_COMPA = 12,
	IRQ_TIMER1_COMPB = 13,
	IRQ_TIMER1_OVF   = 14,
	IRQ_TIMER0_COMP  = 15,
	IRQ_TIMER0_OVF   = 16,
	IRQ_SPI_STC      = 17,
	IRQ_USART0_RXC   = 18,
	IRQ_USART0_UDRE  = 19,
	IRQ_USART0_TXC   = 20,
	IRQ_ADC          = 21,
	IRQ_EE_RDY       = 22,
	IRQ_ANA_COMP     = 23,
	IRQ_TIMER1_COMPC = 24,
	IRQ_TIMER3_CAPT  = 25,
	IRQ_TIMER3_COMPA = 26,
	IRQ_TIMER3_COMPB = 27,
	IRQ_TIMER3_COMPC = 28,
	IRQ_TIMER3_OVF   = 29,
	IRQ_USART1_RXC   = 30,
	IRQ_USART1_UDRE  = 31,
	IRQ_USART1_TXC   = 32,
	IRQ_TWI          = 33,
	IRQ_SPM_RDY      = 34,
    	IRQ_TSC 	 = IRQ_TIMER1_OVF,	
	IRQ_TIMER 	 = IRQ_TIMER0_COMP
    };

public:
    ATMega128_IC() {};

    static Interrupt_Handler int_vector(Interrupt_Id i) {
    return (i < INTS) ? _int_vector[i] : 0;
    }

    static void int_vector(Interrupt_Id i, Interrupt_Handler h) {
    db<IC>(INF) << "ATMega128::int_vector(int=" << i << ",h="
            << (void *)h <<")\n";
    if(i < INTS) _int_vector[i] = h;
    }

    static void enable(Interrupt_Id irq) {

	if(irq <= IRQ_IRQ7) {
	    eimsk(eimsk() | 1 << (irq - 1));
	    return;
	}

	if(irq <= IRQ_TIMER0_OVF) {
	    timsk(timsk() | 1 << -(irq - 16)); 
	    return;
	}

	/* There is no linear relation between UART0, UART1 and Timer3
	   Interrupts and their respective register bits :-( */
	
	switch(irq) {
	    case IRQ_SPI_STC:         spcr(spcr()     | SPIE)  ; break;
	    case IRQ_USART0_RXC:      ucsr0b(ucsr0b() | RXCIEn); break;
	    case IRQ_USART0_UDRE:     ucsr0b(ucsr0b() | UDRIEn); break;
	    case IRQ_USART0_TXC:      ucsr0b(ucsr0b() | TXCIEn); break;
	    case IRQ_ADC:             adcsra(adcsra() | ADIE)  ; break;
	    case IRQ_EE_RDY:          eecr(eecr()     | EERIE) ; break;
	    case IRQ_ANA_COMP:        acsr(acsr()     | ACIE)  ; break;
	    case IRQ_TIMER1_COMPC:    etimsk(etimsk() | OCIE1C); break;
	    case IRQ_TIMER3_CAPT:     etimsk(etimsk() | TICIE3); break;
	    case IRQ_TIMER3_COMPA:    etimsk(etimsk() | OCIE3A); break;
	    case IRQ_TIMER3_COMPB:    etimsk(etimsk() | OCIE3B); break;
	    case IRQ_TIMER3_COMPC:    etimsk(etimsk() | OCIE3C); break;
	    case IRQ_TIMER3_OVF:      etimsk(etimsk() | TOIE3) ; break;
	    case IRQ_USART1_RXC:      ucsr1b(ucsr1b() | RXCIEn); break;
	    case IRQ_USART1_UDRE:     ucsr1b(ucsr1b() | UDRIEn); break;
	    case IRQ_USART1_TXC:      ucsr1b(ucsr1b() | TXCIEn); break;
	}

    }
    
    static void disable() {
        eimsk(0);
	timsk(0); 
	etimsk(0);
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

	if(irq <= IRQ_TIMER0_OVF) {
	    timsk(timsk() & ~( 1 << -(irq - 16))); 
	    return;
	}

	/* There is no linear relation between UART0, UART1 and Timer3
	   Interrupts and their respective register bits :-( */
	
	switch(irq) {
	    case IRQ_SPI_STC:         spcr(spcr()     & ~SPIE)  ; break;
	    case IRQ_USART0_RXC:      ucsr0b(ucsr0b() & ~RXCIEn); break;
	    case IRQ_USART0_UDRE:     ucsr0b(ucsr0b() & ~UDRIEn); break;
	    case IRQ_USART0_TXC:      ucsr0b(ucsr0b() & ~TXCIEn); break;
	    case IRQ_ADC:             adcsra(adcsra() & ~ADIE)  ; break;
	    case IRQ_EE_RDY:          eecr(eecr()     & ~EERIE) ; break;
	    case IRQ_ANA_COMP:        acsr(acsr()     & ~ACIE)  ; break;
	    case IRQ_TIMER1_COMPC:    etimsk(etimsk() & ~OCIE1C); break;
	    case IRQ_TIMER3_CAPT:     etimsk(etimsk() & ~TICIE3); break;
	    case IRQ_TIMER3_COMPA:    etimsk(etimsk() & ~OCIE3A); break;
	    case IRQ_TIMER3_COMPB:    etimsk(etimsk() & ~OCIE3B); break;
	    case IRQ_TIMER3_COMPC:    etimsk(etimsk() & ~OCIE3C); break;
	    case IRQ_TIMER3_OVF:      etimsk(etimsk() & ~TOIE3) ; break;
	    case IRQ_USART1_RXC:      ucsr1b(ucsr1b() & ~RXCIEn); break;
	    case IRQ_USART1_UDRE:     ucsr1b(ucsr1b() & ~UDRIEn); break;
	    case IRQ_USART1_TXC:      ucsr1b(ucsr1b() & ~TXCIEn); break; 
	}

    }
    
private:

    typedef AVR8::Reg8 Reg8;

    static Reg8 eimsk() { return AVR8::in8(EIMSK); }
    static void eimsk(Reg8 value) { AVR8::out8(EIMSK,value); } 
    static Reg8 eifr() { return AVR8::in8(EIFR); }
    static void eifr(Reg8 value) { AVR8::out8(EIFR,value); } 
    static Reg8 timsk() { return AVR8::in8(TIMSK); }
    static void timsk(Reg8 value) { AVR8::out8(TIMSK,value); } 
    static Reg8 tifr() { return AVR8::in8(TIFR); }
    static void tifr(Reg8 value) { AVR8::out8(TIFR,value); } 
    static Reg8 etimsk() { return AVR8::in8(ETIMSK); }
    static void etimsk(Reg8 value) { AVR8::out8(ETIMSK,value); } 
    static Reg8 etifr() { return AVR8::in8(ETIFR); }
    static void etifr(Reg8 value) { AVR8::out8(ETIFR,value); } 
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

private:
    static Interrupt_Handler _int_vector[INTS]; 
};

typedef ATMega128_IC IC;

__END_SYS

#endif
