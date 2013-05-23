// EPOS ATMega128 Memory Map

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega128_memory_map_h
#define __atmega128_memory_map_h

#include <system/memory_map.h>

__BEGIN_SYS

template <>
struct Memory_Map<ATMega128>
{
    enum {
	MEM_BASE =	0,
	MEM_SIZE =	4096,
    };

    enum {
	BASE =		0x000000,
	TOP =		0x001000,
	APP_LO =	0x000000,
	APP_CODE =	0x000000,
	APP_DATA =	0x000100,
	APP_HI =	0x00ffff,
	PHY_MEM =	0x000100,
	IO_MEM =	0x000020,
	SYS =		0x000000,
	INT_VEC =	0x000000,
	SYS_INFO =	0x001000,
	SYS_CODE =	0x000000,
	SYS_DATA =	0x000150,
	SYS_STACK =	0x0010ff
    };
};

template <class Imp> struct IO_Map;
template <> struct IO_Map<ATMega128>
{
    enum {
	PINF	= 0x00,
	PINE	= 0x01,
	DDRE	= 0x02,
	PORTE	= 0x03,
	ADCL	= 0x04,
	ADCH	= 0x05,
	ADCSRA	= 0x06,
	ADMUX	= 0x07,
	ACSR	= 0x08,   
	UBRR0L	= 0x09,
	UCSR0B	= 0x0A,
	UCSR0A	= 0x0B,
	UDR0	= 0x0C,
	SPCR	= 0x0D,
	SPSR	= 0x0E,
	SPDR	= 0x0F,
	PIND	= 0x10,
	DDRD	= 0x11,
	PORTD	= 0x12,
	PINC	= 0x13,
	DDRC	= 0x14,
	PORTC	= 0x15,
	PINB	= 0x16,
	DDRB	= 0x17,
	PORTB	= 0x18,
	PINA	= 0x19, 
	DDRA	= 0x1A,
	PORTA	= 0x1B, 
	EECR	= 0x1C, 
	EEDR	= 0x1D,
	EEARL	= 0x1E,
	EEARH	= 0x1F,
	SFIOR	= 0x20,
	WDTCR	= 0x21,
	OCDR	= 0x22,
	OCR2	= 0x23,
	TCNT2	= 0x24,
	TCCR2	= 0x25,
	ICR1L	= 0x26,
	ICR1H	= 0x27,
	OCR1BL	= 0x28,
	OCR1BH	= 0x29,
	OCR1AL	= 0x2A,
	OCR1AH	= 0x2B,
	TCNT1L	= 0x2C,
	TCNT1H	= 0x2D,
	TCCR1B	= 0x2E,
	TCCR1A	= 0x2F,
	ASSR	= 0x30,
	OCR0	= 0x31,
	TCNT0	= 0x32,
	TCCR0	= 0x33,
	MCUCSR	= 0x34,
	MCUCR	= 0x35,
	TIFR	= 0x36,
	TIMSK	= 0x37,
	EIFR	= 0x38,
	EIMSK	= 0x39,
	EICRB	= 0x3A,
	RAMPZ	= 0x3B,
	XDIV	= 0x3C, 
	SPL	= 0x3D,
	SPH	= 0x3E,
	SREG	= 0x3F,
	DDRF    = 0x41,
	PORTF   = 0x42,
	PING    = 0x43,
	DDRG    = 0x44,
	PORTG   = 0x45,
	SPMCSR  = 0x48,
	EICRA   = 0x4A,
	XMCRB   = 0x4C,
	XMCRA   = 0x4D,
	OSCCAL  = 0x4F,
	TWBR    = 0x50,
	TWSR    = 0x51,
	TWAR    = 0x52,
	TWDR    = 0x53,
	TWCR    = 0x54,
	OCR1CL  = 0x58,
	OCR1CH  = 0x59,
	TCCR1C  = 0x5A,
	ETIFR   = 0x5C,
	ETIMSK  = 0x5D,
	ICR3L   = 0x60,
	ICR3H   = 0x61,
	OCR3CL  = 0x62,
	OCR3CH  = 0x63,
	OCR3BL  = 0x64,
	OCR3BH  = 0x65,
	OCR3AL  = 0x66,
	OCR3AH  = 0x67,
	TCNT3L  = 0x68,
	TCNT3H  = 0x69,
	TCCR3B  = 0x6A,
	TCCR3A  = 0x6B,
	TCCR3C  = 0x6C,
	UBRR0H  = 0x70,
	UCSR0C  = 0x75,
	UBRR1H  = 0x78,
	UBRR1L  = 0x79,
	UCSR1B  = 0x7A,
	UCSR1A  = 0x7B,
	UDR1    = 0x7C,
	UCSR1C  = 0x7D
    };
    
    typedef volatile unsigned char IO_Port;  
    struct IO_Ports {
	IO_Port pinf;		// [0x20]
	IO_Port pine;		// [0x21]
	IO_Port ddre;		// [0x22]
	IO_Port porte;		// [0x23]
	IO_Port adcl;		// [0x24]
	IO_Port adch;		// [0x25]
	IO_Port adcsra;	 	// [0x26]
	IO_Port admux;		// [0x27]
	IO_Port acsr;		// [0x28]   
	IO_Port ubrr0l;		// [0x29]
	IO_Port ucsr0b;		// [0x2A]
	IO_Port ucsr0a;		// [0x2B]
	IO_Port udr0;		// [0x2C]
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
	IO_Port sfior;		// [0x40]
	IO_Port wdtcr;		// [0x41]
	IO_Port ocdr;		// [0x42]
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
	IO_Port assr;		// [0x50]
	IO_Port ocr0;		// [0x51]
	IO_Port tcnt0;		// [0x52]
	IO_Port tccr0;		// [0x53]
	IO_Port mcucsr;	 	// [0x54]
	IO_Port mcucr;		// [0x55]
	IO_Port tifr;		// [0x56]
	IO_Port timsk;		// [0x57]
	IO_Port eifr;		// [0x58]
	IO_Port eimsk;		// [0x59]
	IO_Port eicrb;		// [0x5A]
	IO_Port rampz;	 	// [0x5B]
	IO_Port xdiv;		// [0x5C] 
	IO_Port spl;		// [0x5D]
	IO_Port sph;		// [0x5E]
	IO_Port sreg;		// [0x5F]
	IO_Port reserved0;	// [0x60]
	IO_Port ddrf;		// [0x61]
	IO_Port portf;	       	// [0x62]
	IO_Port ping;	    	// [0x63]
	IO_Port ddrg;       	// [0x64]
	IO_Port portg;      	// [0x65]
	IO_Port reserved1;  	// [0x66]
	IO_Port reserved2;  	// [0x67]
	IO_Port spmcsr;     	// [0x68]
	IO_Port reserved3;  	// [0x69]
	IO_Port eicra;      	// [0x6A]
	IO_Port reserved4;  	// [0x6B]
	IO_Port xmcrb;      	// [0x6C]
	IO_Port xmcra;      	// [0x6D]
	IO_Port reserved5;  	// [0x6E]
	IO_Port osccal;     	// [0x6F]
	IO_Port twbr;       	// [0x70]
	IO_Port twsr;       	// [0x71]
	IO_Port twar;       	// [0x72]
	IO_Port twdr;       	// [0x73]
	IO_Port twcr;       	// [0x74]
	IO_Port reserved6;  	// [0x75]
	IO_Port reserved7;  	// [0x76]
	IO_Port reserved8;  	// [0x77]
	IO_Port ocr1cl;     	// [0x78]
	IO_Port ocr1ch;     	// [0x79]
	IO_Port tccr1c;     	// [0x7A]
	IO_Port reserved9;  	// [0x7B]
	IO_Port etifr;      	// [0x7C]
	IO_Port etimsk;     	// [0x7D]
	IO_Port reserved10; 	// [0x7E]
	IO_Port reserved11; 	// [0x7F]
	IO_Port icr3l;      	// [0x80]
	IO_Port icr3h;      	// [0x81]
	IO_Port ocr3cl;     	// [0x82]
	IO_Port ocr3ch;     	// [0x83]
	IO_Port ocr3bl;     	// [0x84]
	IO_Port ocr3bh;     	// [0x85]
	IO_Port ocr3al;     	// [0x86]
	IO_Port ocr3ah;     	// [0x87]
	IO_Port tcnt3l;     	// [0x88]
	IO_Port tcnt3h;     	// [0x89]
	IO_Port tccr3b;     	// [0x8A]
	IO_Port tccr3a;     	// [0x8B]
	IO_Port tccr3c;     	// [0x8C]
	IO_Port reserved12; 	// [0x8D]
	IO_Port reserved13; 	// [0x8E]
	IO_Port reserved14; 	// [0x8F]
	IO_Port ubrr0h;     	// [0x90]
	IO_Port reserved15; 	// [0x91]
	IO_Port reserved16; 	// [0x92]
	IO_Port reserved17; 	// [0x93]
	IO_Port reserved18; 	// [0x94]
	IO_Port ucsr0c;     	// [0x95]
	IO_Port reserved19; 	// [0x96]
	IO_Port reserved20; 	// [0x97]
	IO_Port ubrr1h;     	// [0x98]
	IO_Port ubrr1l;     	// [0x99]
	IO_Port ucsr1b;     	// [0x9A]
	IO_Port ucsr1a;     	// [0x9B]
	IO_Port udr1;       	// [0x9C]
	IO_Port ucsr1c;     	// [0x9D]
	IO_Port reserved21; 	// [0x9E]
	IO_Port reserved22; 	// [0x9F]
    };
};

//
// Memory Map for Atmega128
//
// Code Memory
//
// 0x0000 -+- - - - - - - - - - - - -+- 
//         | 			     |
//         : Fixed Interrupt Vector  :
//         |                         |
//        -+- - - - - - - - - - - - -+- 
//         | 			     |
//         : APP/SYS CODE            :
//         |                         |
// 0xFFFF -+- - - - - - - - - - - - -+-
//
// Data Memory
//
// 0x0000 -+- - - - - - - - - - - - -+- BASE
//         | 			     |
//         : GP Registers            :
//         |                         |
// 0x0020 -+- - - - - - - - - - - - -+- IO_MEM
//         | 			     |
//         : IO Registers            :
//         |                         |
// 0x0100 -+- - - - - - - - - - - - -+- PHY_MEM, APP_DATA, SYS_DATA
//         | 			     |
//         : APP/SYS DATA            :
//         |                         |
//         : Interrupt Vector        :  
//         |                         |
// 0x???? -+- - - - - - - - - - - - -+- bss_end, FREE
//         | 			     |
//         : APP/SYS                 :
//         |                         |
//         : HEAP/STACK              :
//         |                         |
// 0x1000 -+- - - - - - - - - - - - -+- TOP, SYS_INFO
//         | 			     |
//         : System_Info             :
//         |                         |
//         : GCC Stack               :
//         |                         |
// 0x10ff -+- - - - - - - - - - - - -+- RAM End
//
//
// The area between TOP and RAM End is freed in __epos_library_app_entry
//

__END_SYS

#endif
