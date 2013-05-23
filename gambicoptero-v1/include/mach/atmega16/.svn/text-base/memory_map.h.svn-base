// EPOS ATMega16 Memory Map

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __memory_map_atmega16_h
#define __memory_map_atmega16_h

__BEGIN_SYS

template <>
struct Memory_Map<ATMega16>
{
    enum {
	MEM_BASE =	0x60,
	MEM_SIZE =	1024
    };


    enum {
	BASE =		0x000000,
	TOP =		0x000400,
	APP_LO =	0x000000,
	APP_CODE =	0x000000,
	APP_DATA =	0x800110,
	APP_HI =	0x00ffff,
	PHY_MEM =	0x800060,
	IO_MEM =	0x800020,
	SYS =		0x000000,
	INT_VEC =	0x000000,
	SYS_PT =	TOP,
	SYS_PD =	TOP,
	SYS_INFO =	0x000060,
	SYS_CODE =	0x000000,
	SYS_DATA =	0x800110,
	SYS_STACK =	0x80045f,
	MACH1 =		0x000400,
	MACH2 =		0x000000,
	MACH3 =		TOP,
    };

};

template <class Imp> struct IO_Map;
template <> struct IO_Map<ATMega16>
{
    enum {
	TWBR		= 0x00,
	TWSR		= 0x01,
	TWAR		= 0x02,
	TWDR		= 0x03,
	ADCL		= 0x04,
	ADCH		= 0x05,
	ADCSRA		= 0x06,
	ADMUX		= 0x07,
	ACSR		= 0x08,   
	UBRR0L		= 0x09,
	UCSR0B		= 0x0A,
	UCSR0A		= 0x0B,
	UDR0    	= 0x0C,
	SPCR		= 0x0D,
	SPSR		= 0x0E,
	SPDR		= 0x0F,
	PIND		= 0x10,
	DDRD		= 0x11,
	PORTD		= 0x12,
	PINC		= 0x13,
	DDRC		= 0x14,
	PORTC		= 0x15,
	PINB		= 0x16,
	DDRB		= 0x17,
	PORTB		= 0x18,
	PINA		= 0x19, 
	DDRA		= 0x1A,
	PORTA		= 0x1B, 
	EECR		= 0x1C, 
	EEDR		= 0x1D,
	EEARL		= 0x1E,
	EEARH		= 0x1F,
	UBRR0H		= 0x20,
	UCSR0C		= 0x20,
	WDTCR		= 0x21,
	ASSR		= 0x22,
	OCR2		= 0x23,
	TCNT2		= 0x24,
	TCCR2		= 0x25,
	ICR1L		= 0x26,
	ICR1H		= 0x27,
	OCR1BL		= 0x28,
	OCR1BH		= 0x29,
	OCR1AL		= 0x2A,
	OCR1AH		= 0x2B,
	TCNT1L		= 0x2C,
	TCNT1H		= 0x2D,
	TCCR1B		= 0x2E,
	TCCR1A		= 0x2F,
	SFIOR		= 0x30,
	OSCCAL		= 0x31,
	OCDR		= 0x31,
	TCNT0		= 0x32,
	TCCR0		= 0x33,
	MCUCSR		= 0x34,
	MCUCR		= 0x35,
	TWCR		= 0x36,
	SPMCR		= 0x37,
	TIFR		= 0x38,
	TIMSK		= 0x39,
	GIFR		= 0x3A,
	GICR		= 0x3B,
	OCR0		= 0x3C, 
	SPL		= 0x3D,
	SPH		= 0x3E,
	SREG		= 0x3F,
	UCSR1A  	= 0xff,
	UCSR1B  	= 0xff,
	UCSR1C  	= 0xff,
	UDR1    	= 0xff,
	UBRR1H  	= 0xff,
	UBRR1L  	= 0xff,
	ETIMSK  	= 0xff,
	ETIFR   	= 0xff,
	TCCR1C  	= 0xff,
	OCR1CH  	= 0xff,
	OCR1CL  	= 0xff,
	TCCR3A  	= 0xff,
	TCCR3B  	= 0xff,
	TCCR3C  	= 0xff,
	TCNT3H  	= 0xff,
	TCNT3L  	= 0xff,
	OCR3AH  	= 0xff,
	OCR3AL  	= 0xff,
	OCR3BH  	= 0xff,
	OCR3BL  	= 0xff,
	OCR3CH  	= 0xff,
	OCR3CL  	= 0xff,
	ICR3H   	= 0xff,
	ICR3L   	= 0xff
    };
    
    typedef volatile unsigned char IO_Port;  
    struct IO_Ports {
	IO_Port twbr;		// [0x20]
	IO_Port twsr;		// [0x21]
	IO_Port twar;		// [0x22]
	IO_Port twdr;		// [0x23]
	IO_Port adcl;		// [0x24]
	IO_Port adch;		// [0x25]
	IO_Port adcsra;	 	// [0x26]
	IO_Port admux;		// [0x27]
	IO_Port acsr;		// [0x28]   
	IO_Port ubrrl;		// [0x29]
	IO_Port ucsrb;		// [0x2A]
	IO_Port ucsra;		// [0x2B]
	IO_Port udr;		// [0x2C]
	IO_Port spcr;		// [0x2D]
	IO_Port spsr;		// [0x2E]
	IO_Port spdr;		// [0x2F]
	IO_Port pind;		// [0x30]
	IO_Port ddrd;		// [0x31]
	IO_Port portd;		// [0x32]
	IO_Port pinc;		// [0x33]
	IO_Port ddrc;		// [0x34]
	IO_Port portc;		// [0x35]
	IO_Port pinb;		// [0x36]
	IO_Port ddrb;		// [0x37]
	IO_Port portb;		// [0x38]
	IO_Port pina;		// [0x39] 
	IO_Port ddra;		// [0x3A]
	IO_Port porta;		// [0x3B] 
	IO_Port eecr;		// [0x3C] 
	IO_Port eedr;		// [0x3D]
	IO_Port eearl;		// [0x3E]
	IO_Port eearh;		// [0x3F]
	union
	{
	    IO_Port ubrrh;	// [0x40]
	    IO_Port ucsrc;	// [0x40]
	};
	IO_Port wdtcr;		// [0x41]
	IO_Port assr;		// [0x42]
	IO_Port ocr2;		// [0x43]
	IO_Port tcnt2;		// [0x44]
	IO_Port tccr2;		// [0x45]
	IO_Port icr1l;		// [0x46]
	IO_Port icr1h;		// [0x47]
	IO_Port ocr1bl;	 	// [0x48]
	IO_Port ocr1bh;	 	// [0x49]
	IO_Port ocr1al;	 	// [0x4A]
	IO_Port ocr1ah;	 	// [0x4B]
	IO_Port tcnt1l;	 	// [0x4C]
	IO_Port tcnt1h;	 	// [0x4D]
	IO_Port tccr1b;	 	// [0x4E]
	IO_Port tccr1a;	 	// [0x4F]
	IO_Port sfior;		// [0x50]
	union
	{
	    IO_Port osccal;	// [0x51]
	    IO_Port ocdr;	// [0x51]
	};
	IO_Port tcnt0;		// [0x52]
	IO_Port tccr0;		// [0x53]
	IO_Port mcucsr;	 	// [0x54]
	IO_Port mcucr;		// [0x55]
	IO_Port twcr;		// [0x56]
	IO_Port spmcr;		// [0x57]
	IO_Port tifr;		// [0x58]
	IO_Port timsk;		// [0x59]
	IO_Port gifr;		// [0x5A]
	IO_Port gicr;	 	// [0x5B]
	IO_Port ocr0;		// [0x5C] 
	IO_Port spl;		// [0x5D]
	IO_Port sph;		// [0x5E]
	IO_Port sreg;		// [0x5F]
    };
};

__END_SYS

#endif
