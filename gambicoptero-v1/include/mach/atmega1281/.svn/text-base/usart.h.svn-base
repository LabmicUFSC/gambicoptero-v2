// EPOS ATMega1281 USART Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega1281_usart_h
#define __atmega1281_usart_h

#include <system/memory_map.h>
#include <usart.h>

__BEGIN_SYS

class ATMega1281_USART: public USART_Common
{
private:
	typedef IO_Map<Machine> IO;
	typedef AVR8::Reg8 Reg8;
		
public:
	ATMega1281_USART() {
		ubrr0l(0);
		ubrr0h(0);
		ddre(1<<2);
		ucsr0c((1<<7)|(1<<6)|(0<<2)|(1<<1)|(1<<0));
		ucsr0b((1<<4)|(1<<3));
		ubrr0l(3);
		ubrr0h(0);
	}

	~ATMega1281_USART() {
		ubrr0l(0);
		ubrr0h(0);
	}

	char get() {
		while(!(ucsr0a() & 1<<7));
		return udr0();
	}
    
	void put(char c) {
		while(!(ucsr0a() & 1<<5));
		udr0(c);
		while(!(ucsr0a() & 1<<6));
	}

private:
	Reg8 udr0() {
		return AVR8::in8(IO::UDR0);
	}

	void udr0(Reg8 value) { 
		AVR8::out8(IO::UDR0, value);
	}   

	Reg8 ubrr0h() {
		return AVR8::in8(IO::UBRR0H);
	}

	void ubrr0h(Reg8 value) { 
		AVR8::out8(IO::UBRR0H, value);
	}   

	Reg8 ubrr0l() {
		return AVR8::in8(IO::UBRR0L);
	}

	void ubrr0l(Reg8 value) { 
		AVR8::out8(IO::UBRR0L, value);
	}   

	Reg8 ucsr0c() {
		return AVR8::in8(IO::UCSR0C);
	}

	void ucsr0c(Reg8 value) { 
		AVR8::out8(IO::UCSR0C, value);
	}   

	Reg8 ucsr0b() {
		return AVR8::in8(IO::UCSR0B);
	}

	void ucsr0b(Reg8 value) { 
		AVR8::out8(IO::UCSR0B, value);
	}   

	Reg8 ucsr0a() {
		return AVR8::in8(IO::UCSR0A);
	}

	void ucsr0a(Reg8 value) { 
		AVR8::out8(IO::UCSR0A, value);
	}   

	Reg8 ddre() {
		return AVR8::in8(IO::DDRE);
	}

	void ddre(Reg8 value) { 
		AVR8::out8(IO::DDRE, value);
	}   
};

__END_SYS

#endif
