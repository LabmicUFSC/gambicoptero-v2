// EPOS AVR UART Mediator Common Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __avr_uart_h
#define __avr_uart_h

#include <system/memory_map.h>
#include <cpu.h>
#include <uart.h>

__BEGIN_SYS

class AVR_UART
{
protected:
    AVR_UART() {}

private:
    typedef IO_Map<Machine> IO;
    typedef AVR8::Reg8 Reg8;
    typedef AVR8::Reg16 Reg16;
    typedef Traits<UART> _Traits;

    static const unsigned int BASE_CLOCK = Traits<Machine>::CLOCK / 512;

public:
    //UART IO Register Bit Offsets
    enum {
        //UCSRA
        RXC	= 7,
        TXC 	= 6,
        UDRE	= 5,
        FE	= 4,
        DOR	= 3,
        UPE	= 2,
        U2X	= 1,
        MPCM	= 0,
        //UCSRB
        RXCIE	= 7,
        TXCIE	= 6,
        UDRIE	= 5,
        RXEN	= 4,
        TXEN	= 3,
        UCS2Z	= 2,
        RXB8	= 1,
        TXB8	= 0,
        //UCSRC
        URSEL	= 7,
        UMSEL	= 6,
        UPM1	= 5,
        UPM0	= 4,
        USBS	= 3,
        UCSZ1	= 2,
        UCSZ0	= 1,
        UCPOL	= 0,
    };

public:  
    AVR_UART(unsigned int unit = 0) : _unit(unit)
    {
		config(9600,8,0,1);
		power(_Traits::FULL);
    }
    AVR_UART(unsigned int baud, unsigned int data_bits, unsigned int parity,
	     unsigned int stop_bits, unsigned int unit = 0) : _unit(unit)
    {
    	config(baud,data_bits,parity,stop_bits);
		power(_Traits::FULL);
    }
    ~AVR_UART(){ 
		ubrrhl(0);
		ucsrb(0);
    }

    void config(unsigned int baud, unsigned int data_bits,
		unsigned int parity, unsigned int stop_bits)
    {
		ubrrhl((BASE_CLOCK / (baud>>5)) - 1);

		unsigned char cfg =
			((data_bits - 5) << UCSZ0) | ((stop_bits - 1) << USBS  ) | (1 << URSEL);
		if (parity) cfg |= (parity + 1) << UPM0;
		ucsrc(cfg);
	
		ucsra(0);
	}

    void config(unsigned int * baud, unsigned int * data_bits,
		unsigned int * parity, unsigned int * stop_bits)
    {

		*baud = (BASE_CLOCK / (ubrrhl() + 1))<<5;

		unsigned char rc = ucsrc();

		*data_bits = ((rc >> UCSZ0) + 5) & 0x0F;
		*stop_bits = ((rc >> USBS) + 1) & 0x03;

		*parity = (rc >> USBS) & 0x03;
		if(*parity) *parity -= 1;

    }

    Reg8 rxd() { return udr(); }
    void txd(Reg8 c) { udr(c); }

    void reset()
    {
		unsigned int b, db, p, sb;
		config(&b, &db, &p, &sb);
		config(b, db, p, sb);
    }

    void loopback(bool flag) { }

    void int_enable(bool receive = true, bool send = true,
		    bool line = true, bool modem = true)
    {
		ucsrb( (1 << TXEN) | (1 << RXEN) |
			   (receive << RXCIE) | (send << UDRIE) );
    }
    void int_disable() { ucsrb(1 << TXEN | 1 << RXEN); }

    bool rxd_full() { return (ucsra() & (1 << RXC)); }
    bool txd_empty() { return (ucsra() & (1 << UDRE)); }

    void dtr();
    void rts();
    bool cts() { return true; }
    bool dsr() { return true; }
    bool dcd() { return true; }
    bool ri()  { return false; }

    bool overrun_error() { return (ucsra() & (1 << RXC)) ; }
    bool parity_error()  { return (ucsra() & (1 << UPE)) ; }
    bool framing_error() { return (ucsra() & (1 << FE)) ; }

    void power(Traits<UART>::Power_Modes mode) {
        switch(mode)
        {
        case _Traits::FULL:
			ucsrb(ucsrb() | (1 << TXEN) | (1 << RXEN));
			break;
		case _Traits::LIGHT:
			ucsrb(ucsrb() & ~(1 << RXEN));
			//When using the FIFO buffer, it has to be flushed at this
			//point to avoid loss of data.
			ucsrb(ucsrb() | (1 << TXEN));
			break;
		case _Traits::STANDBY:
			ucsrb(ucsrb() & ~(1 << TXEN));
			ucsrb(ucsrb() | (1 << RXEN));
			break;
		case _Traits::OFF:
			ucsrb(ucsrb() & ~((1 << TXEN) | (1 << RXEN)));
			break;
        }
    }

private:
    Reg8 udr(){ return AVR8::in8((_unit == 0) ? IO::UDR0 : IO::UDR1); }
    void udr(Reg8 value){ AVR8::out8(((_unit == 0) ? IO::UDR0 : IO::UDR1),value); }   
    Reg8 ucsra(){ return AVR8::in8((_unit == 0) ? IO::UCSR0A : IO::UCSR1A); }
    void ucsra(Reg8 value){ AVR8::out8(((_unit == 0) ? IO::UCSR0A : IO::UCSR1A),value); } 
    Reg8 ucsrb(){ return AVR8::in8((_unit == 0) ? IO::UCSR0B : IO::UCSR1B); }
    void ucsrb(Reg8 value){ AVR8::out8(((_unit == 0) ? IO::UCSR0B : IO::UCSR1B),value); } 
    Reg8 ubrrl(){ return AVR8::in8((_unit == 0) ? IO::UBRR0L : IO::UBRR1L); }
    void ubrrl(Reg8 value){ AVR8::out8(((_unit == 0) ? IO::UBRR0L : IO::UBRR1L),value); } 
    Reg8 ubrrh(){ return AVR8::in8((_unit == 0) ? IO::UBRR0H : IO::UBRR1H); }
    void ubrrh(Reg8 value){ AVR8::out8(((_unit == 0) ? IO::UBRR0H : IO::UBRR1H),value); } 
    Reg8 ucsrc(){ return (_unit == 0) ? _ucsr0c : AVR8::in8(IO::UCSR1C); }
    void ucsrc(Reg8 value)
    {
		_ucsr0c = value;
		AVR8::out8(((_unit == 0) ? IO::UCSR0C : IO::UCSR1C),(value | 1 << URSEL));
    } 

    Reg16 ubrrhl()
    {
		Reg16 value = ubrrl();
		value |= ((Reg16)ubrrh())<<8;
		return value;
    }
    void ubrrhl(Reg16 value)
    {
		ubrrh((Reg8)(value>>8));
		ubrrl((Reg8)value);
    }

private:
    int _unit;
    Reg8 _ucsr0c;
};

__END_SYS

#endif
