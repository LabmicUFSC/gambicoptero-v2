// EPOS ATMega1281 Memory Map

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega1281_memory_map_h
#define __atmega1281_memory_map_h

#include <system/memory_map.h>

__BEGIN_SYS

template <>
struct Memory_Map<ATMega1281>
{
    enum {
	MEM_BASE = 0,
	MEM_SIZE = 8192,
    };

    enum {
	BASE      = 0x000000,
	TOP       = 0x002000,
	APP_LO    = 0x000000,
	APP_CODE  = 0x000000,
	APP_DATA  = 0x000200,
	APP_HI    = 0x00FFFF,
	PHY_MEM   = 0x000200,
	IO_MEM    = 0x000020,
	SYS       = 0x000000,
	INT_VEC   = 0x000000,
	SYS_INFO  = 0x002100,
	SYS_CODE  = 0x000000,
	SYS_DATA  = 0x000250,
	SYS_STACK = 0x0021FF
    };
};

template <class Imp> struct IO_Map;
template <> struct IO_Map<ATMega1281>
{
    enum {
	PINA	= 0x00,
	DDRA	= 0x01,
	PORTA	= 0x02,
	PINB	= 0x03,
	DDRB	= 0x04,
	PORTB	= 0x05,
	PINC	= 0x06,
	DDRC	= 0x07,
	PORTC	= 0x08,   
	PIND	= 0x09,
	DDRD	= 0x0A,
	PORTD	= 0x0B,
	PINE	= 0x0C,
	DDRE	= 0x0D,
	PORTE	= 0x0E,
	PINF	= 0x0F,
	DDRF	= 0x10,
	PORTF	= 0x11,
	PING	= 0x12,
	DDRG	= 0x13,
	PORTG	= 0x14,
	TIFR0	= 0x15,
	TIFR1	= 0x16,
	TIFR2	= 0x17,
	TIFR3	= 0x18,
	TIFR4	= 0x19, 
	TIFR5	= 0x1A,
	PCIFR	= 0x1B, 
	EIFR	= 0x1C, 
	EIMSK	= 0x1D,
	GPIOR0	= 0x1E,
	EECR	= 0x1F,
	EEDR	= 0x20,
	EEAR	= 0x21,
	EEARL	= 0x21,
	EEARH	= 0x22,
	GTCCR	= 0x23,
	TCCR0A	= 0x24,
	TCCR0B	= 0x25,
	TCNT0	= 0x26,
	OCR0A	= 0x27,
	OCR0B	= 0x28,
        /* Reserved [0x29] */
	GPIOR1	= 0x2A,
	GPIOR2	= 0x2B,
	SPCR	= 0x2C,
	SPSR	= 0x2D,
	SPDR	= 0x2E,
        /* Reserved [0x2F] */
	ACSR    = 0x30,
	MONDR   = 0x31,
        OCDR    = 0x31,
        /* Reserved [0x32] */
	SMCR	= 0x33,
	MCUSR	= 0x34,
	MCUCR	= 0x35,
        /* Reserved [0x36] */
	SPMCSR	= 0x37,
        /* Reserved [0x38..0x3A] */
	RAMPZ	= 0x3B,
	EIND	= 0x3C, 
        SPL     = 0x3D,
        SPH     = 0x3E,
        SREG    = 0x3F,
	WDTCSR  = 0x40,
	CLKPR   = 0x41,
        /* Reserved [0x42..0x43] */
	PRR0    = 0x44,
	PRR1    = 0x45,
	OSCCAL  = 0x46,
        /* Reserved [0x47] */
	PCICR   = 0x48,
	EICRA   = 0x49,
	EICRB   = 0x4A,
	PCMSK0  = 0x4B,
	PCMSK1  = 0x4C,
	PCMSK2  = 0x4D,
	TIMSK0  = 0x4E,
	TIMSK1  = 0x4F,
	TIMSK2  = 0x50,
	TIMSK3  = 0x51,
	TIMSK4  = 0x52,
	TIMSK5  = 0x53,
	XMCRA   = 0x54,
        XMCRB   = 0x55,
        /* Reserved [0x56..0x57] */
        ADC     = 0x58,
        ADCW    = 0x58,
        ADCL    = 0x58, 
        ADCH    = 0x59,
        ADCSRA  = 0x5A,
        ADCSRB  = 0x5B,
        ADMUX   = 0x5C,
        DIDR2   = 0x5D,
        DIDR0   = 0x5E,
        DIDR1   = 0x5F,
        TCCR1A  = 0x60,
        TCCR1B  = 0x61,
        TCCR1C  = 0x62,
        /* Reserved [0x63] */
        TCNT1   = 0x64,
        TCNT1L  = 0x64,
        TCNT1H  = 0x65,
        ICR1    = 0x66,
        ICR1L   = 0x66,
        ICR1H   = 0x67,
        OCR1A   = 0x68,
        OCR1AL  = 0x68,
        OCR1AH  = 0x69,
        OCR1B   = 0x6A,
        OCR1BL  = 0x6A,
        OCR1BH  = 0x6B,
        OCR1C   = 0x6C,
        OCR1CL  = 0x6C,
        OCR1CH  = 0x6D,
        /* Reserved [0x6E..0x6F] */
        TCCR3A  = 0x70,
        TCCR3B  = 0x71,
        TCCR3C  = 0x72,
        /* Reserved [0x73] */
        TCNT3   = 0x74,
        TCNT3L  = 0x74,
        TCNT3H  = 0x75,
        ICR3    = 0x76,
        ICR3L   = 0x76,
        ICR3H   = 0x77,
        OCR3A   = 0x78,
        OCR3AL  = 0x78,
        OCR3AH  = 0x79,
        OCR3B   = 0x7A,
        OCR3BL  = 0x7A,
        OCR3BH  = 0x7B,
        OCR3C   = 0x7C,
        OCR3CL  = 0x7C,
        OCR3CH  = 0x7D,
        /* Reserved [0x7E..0x7F] */
        TCCR4A  = 0x80,
        TCCR4B  = 0x81,
        TCCR4C  = 0x82,
        /* Reserved [0x83] */
        TCNT4   = 0x84,
        TCNT4L  = 0x84,
        TCNT4H  = 0x85,
        ICR4    = 0x86,
        ICR4L   = 0x86,
        ICR4H   = 0x87,
        OCR4A   = 0x88,
        OCR4AL  = 0x88,
        OCR4AH  = 0x89,
        OCR4B   = 0x8A,
        OCR4BL  = 0x8A,
        OCR4BH  = 0x8B,
        OCR4C   = 0x8C,
        OCR4CL  = 0x8C,
        OCR4CH  = 0x8D,
        /* Reserved [0x8E..0x8F] */
        TCCR2A  = 0x90,
        TCCR2B  = 0x91,
        TCNT2   = 0x92,
        OCR2A   = 0x93,
        OCR2B   = 0x94,
        /* Reserved [0x95] */
        ASSR    = 0x96,
        /* Reserved [0x97] */
        TWBR    = 0x98,
        TWSR    = 0x99,
        TWAR    = 0x9A,
        TWDR    = 0x9B,
        TWCR    = 0x9C,
        TWAMR   = 0x9D,
        /* Reserved [0x9E..0x9F] */
        UCSR0A  = 0xA0,
        UCSR0B  = 0xA1,
        UCSR0C  = 0xA2,
        /* Reserved [0xA3] */
        UBRR0   = 0xA4,
        UBRR0L  = 0xA4,
        UBRR0H  = 0xA5,
        UDR0    = 0xA6,
        /* Reserved [0xA7] */
        UCSR1A  = 0xA8,
        UCSR1B  = 0xA9,
        UCSR1C  = 0xAA,
        /* Reserved [0xAB] */
        UBRR1   = 0xAC,
        UBRR1L  = 0xAC,
        UBRR1H  = 0xAD,
        UDR1    = 0xAE,
        /* Reserved [0xAF] */
        TCCR5A  = 0x100,
        TCCR5B  = 0x101,
        TCCR5C  = 0x102,
        /* Reserved [0x103] */
        TCNT5   = 0x104,
        TCNT5L  = 0x104,
        TCNT5H  = 0x105,
        ICR5    = 0x106,
        ICR5L   = 0x106,
        ICR5H   = 0x107,
        OCR5A   = 0x108,
        OCR5AL  = 0x108,
        OCR5AH  = 0x109,
        OCR5B   = 0x10A,
        OCR5BL  = 0x10A,
        OCR5BH  = 0x10B,
        OCR5C   = 0x10C,
        OCR5CL  = 0x10C,
        OCR5CH  = 0x10D
    };
    
    typedef volatile unsigned char IO_Port;  
    struct IO_Ports {
	IO_Port pina;		// [0x20]
	IO_Port ddra;		// [0x21]
	IO_Port porta;		// [0x22]
	IO_Port pinb;		// [0x23]
	IO_Port ddrb;		// [0x24]
	IO_Port portb;		// [0x25]
	IO_Port pinc;	 	// [0x26]
	IO_Port ddrc;		// [0x27]
	IO_Port portc;		// [0x28]   
	IO_Port pind;		// [0x29]
	IO_Port ddrd;		// [0x2A]
	IO_Port portd;		// [0x2B]
	IO_Port pine;		// [0x2C]
	IO_Port ddre;		// [0x2D]
	IO_Port porte;		// [0x2E]
	IO_Port pinf;		// [0x2F]
	IO_Port ddrf;		// [0x30]
	IO_Port portf;		// [0x31]
	IO_Port ping;		// [0x32]
	IO_Port ddrg;		// [0x33]
	IO_Port portg;		// [0x34]
	IO_Port tifr0;		// [0x35]
	IO_Port tifr1;		// [0x36]
	IO_Port tifr2;		// [0x37]
	IO_Port tifr3;		// [0x38]
	IO_Port tifr4;		// [0x39] 
	IO_Port tifr5;		// [0x3A]
	IO_Port pcifr;		// [0x3B] 
	IO_Port eifr;		// [0x3C] 
	IO_Port eimsk;		// [0x3D]
	IO_Port gpior0;		// [0x3E]
	IO_Port eecr;		// [0x3F]
	IO_Port eedr;		// [0x40]
	IO_Port eearl;		// [0x41]
	IO_Port eearh;		// [0x42]
	IO_Port gtccr;		// [0x43]
	IO_Port tccr0a;		// [0x44]
	IO_Port tccr0b;		// [0x45]
	IO_Port tcnt0;		// [0x46]
	IO_Port ocr0a;		// [0x47]
	IO_Port ocr0b;	 	// [0x48]
	IO_Port reserved0; 	// [0x49]
	IO_Port gpior1;	 	// [0x4A]
	IO_Port gpior2;	 	// [0x4B]
	IO_Port spcr;	 	// [0x4C]
	IO_Port spsr;	 	// [0x4D]
	IO_Port spdr;	 	// [0x4E]
	IO_Port reserved1; 	// [0x4F]
	IO_Port acsr;		// [0x50]
	IO_Port ocdr;		// [0x51]
	IO_Port reserved2;	// [0x52]
	IO_Port smcr;		// [0x53]
	IO_Port mcusr;	 	// [0x54]
	IO_Port mcucr;		// [0x55]
	IO_Port reserved3;	// [0x56]
	IO_Port spmcsr;		// [0x57]
	IO_Port reserved4;	// [0x58]
	IO_Port reserved5;	// [0x59]
	IO_Port reserved6;	// [0x5A]
	IO_Port rampz;		// [0x5B]
	IO_Port eind;		// [0x5C]
	IO_Port spl;	 	// [0x5D]
	IO_Port sph;		// [0x5E] 
	IO_Port sreg;		// [0x5F]
	IO_Port wdtcsr;		// [0x60]
	IO_Port clkpr;		// [0x61]
	IO_Port reserved7;	// [0x62]
	IO_Port reserved8;	// [0x63]
	IO_Port prr0;	       	// [0x64]
	IO_Port prr1;	    	// [0x65]
	IO_Port osccal;       	// [0x66]
	IO_Port reserved9;     	// [0x67]
	IO_Port pcicr;  	// [0x68]
	IO_Port eicra;  	// [0x69]
	IO_Port eicrb;     	// [0x6A]
	IO_Port pcmsk0;  	// [0x6B]
	IO_Port pcmsk1;      	// [0x6C]
	IO_Port pcmsk2;  	// [0x6D]
	IO_Port timsk0;      	// [0x6E]
	IO_Port timsk1;      	// [0x6F]
	IO_Port timsk2;  	// [0x70]
	IO_Port timsk3;     	// [0x71]
	IO_Port timsk4;       	// [0x72]
	IO_Port timsk5;       	// [0x73]
	IO_Port xmcra;       	// [0x74]
	IO_Port xmcrb;       	// [0x75]
	IO_Port reserved10;    	// [0x76]
	IO_Port reserved11;  	// [0x77]
	IO_Port adcl;     	// [0x78]
	IO_Port adch;     	// [0x79]
	IO_Port adcsra;     	// [0x7A]
	IO_Port adcsrb;  	// [0x7B]
	IO_Port admux;      	// [0x7C]
	IO_Port didr2;     	// [0x7D]
	IO_Port didr0;  	// [0x7E]
	IO_Port didr1;  	// [0x7F]
	IO_Port tccr1a;      	// [0x80]
	IO_Port tccr1b;      	// [0x81]
	IO_Port tccr1c;     	// [0x82]
	IO_Port reserved12;    	// [0x83]
	IO_Port tcnt1l;     	// [0x84]
	IO_Port tcnt1h;     	// [0x85]
	IO_Port icr1l;     	// [0x86]
	IO_Port icr1h;     	// [0x87]
	IO_Port ocr1al;     	// [0x88]
	IO_Port ocr1ah;     	// [0x89]
	IO_Port ocr1bl;     	// [0x8A]
	IO_Port ocr1bh; 	// [0x8B]
        IO_Port ocr1cl;         // [0x8C]
        IO_Port ocr1ch;         // [0x8D]
	IO_Port reserved13; 	// [0x8E]
	IO_Port reserved14; 	// [0x8F]
	IO_Port tccr3a;     	// [0x90]
	IO_Port tccr3b; 	// [0x91]
	IO_Port tccr3c; 	// [0x92]
	IO_Port reserved15; 	// [0x93]
	IO_Port tcnt3l; 	// [0x94]
	IO_Port tcnt3h;     	// [0x95]
	IO_Port icr3l;  	// [0x96]
	IO_Port icr3h;   	// [0x97]
	IO_Port ocr3al;     	// [0x98]
	IO_Port ocr3ah;     	// [0x99]
	IO_Port ocr3bl;     	// [0x9A]
	IO_Port ocr3bh;     	// [0x9B]
	IO_Port ocr3cl;       	// [0x9C]
	IO_Port ocr3ch;     	// [0x9D]
	IO_Port reserved16; 	// [0x9E]
	IO_Port reserved17; 	// [0x9F]
        IO_Port tccr4a;         // [0xA0]
        IO_Port tccr4b;         // [0xA1]
        IO_Port tccr4c;         // [0xA2]
        IO_Port reserved18;     // [0xA3]
        IO_Port tcnt4l;         // [0xA4]
        IO_Port tcnt4h;         // [0xA5]
        IO_Port icr4l;          // [0xA6]
        IO_Port icr4h;          // [0xA7]
        IO_Port ocr4al;         // [0xA8]
        IO_Port ocr4ah;         // [0xA9]
        IO_Port ocr4bl;         // [0xAA]
        IO_Port ocr4bh;         // [0xAB]
        IO_Port ocr4cl;         // [0xAC]
        IO_Port ocr4ch;         // [0xAD]
        IO_Port reserved19;     // [0xAE]
        IO_Port reserved20;     // [0xAF]
        IO_Port tccr2a;         // [0xB0]
        IO_Port tccr2b;         // [0xB1]
        IO_Port tcnt2;          // [0xB2]
        IO_Port ocr2a;          // [0xB3]
        IO_Port ocr2b;          // [0xB4]
        IO_Port reserved21;     // [0xB5]
        IO_Port assr;           // [0xB6]
        IO_Port reserved22;     // [0xB7]
        IO_Port twbr;           // [0xB8]
        IO_Port twsr;           // [0xB9]
        IO_Port twar;           // [0xBA]
        IO_Port twdr;           // [0xBB]
        IO_Port twcr;           // [0xBC]
        IO_Port twamr;          // [0xBD]
        IO_Port reserved23;     // [0xBE]
        IO_Port reserved24;     // [0xBF]
        IO_Port ucsr0a;         // [0xC0]
        IO_Port ucsr0b;         // [0xC1]
        IO_Port ucsr0c;         // [0xC2]
        IO_Port reserved25;     // [0xC3]
        IO_Port ubrr0l;         // [0xC4]
        IO_Port ubrr0h;         // [0xC5]
        IO_Port udr0;           // [0xC6]
        IO_Port reserved26;     // [0xC7]
        IO_Port ucsr1a;         // [0xC8]
        IO_Port ucsr1b;         // [0xC9]
        IO_Port ucsr1c;         // [0xCA]
        IO_Port reserved27;     // [0xCB]
        IO_Port ubrr1l;         // [0xCC]
        IO_Port ubrr1h;         // [0xCD]
        IO_Port udr1;           // [0xCE]
        IO_Port reserved28;     // [0xCF]
        IO_Port tccr5a;         // [0x120]
        IO_Port tccr5b;         // [0x121]
        IO_Port tccr5c;         // [0x122]
        IO_Port reserved29;     // [0x123]
        IO_Port tcnt5l;         // [0x124]
        IO_Port tcnt5h;         // [0x125]
        IO_Port icr5l;          // [0x126]
        IO_Port icr5h;          // [0x127]
        IO_Port ocr5al;         // [0x128]
        IO_Port ocr5ah;         // [0x129]
        IO_Port ocr5bl;         // [0x12A]
        IO_Port ocr5bh;         // [0x12B]
        IO_Port ocr5cl;         // [0x12C]
        IO_Port ocr5ch;         // [0x12D]
    };
};

__END_SYS

#endif

