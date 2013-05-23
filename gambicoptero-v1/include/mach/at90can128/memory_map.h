// EPOS  Memory Map for the AT90CAN128

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __at90can128_memory_map_h
#define __at90can128_memory_map_h

#include <system/memory_map.h>

__BEGIN_SYS

template <>
struct Memory_Map<AT90CAN128>
{
    enum {
	MEM_BASE =	0,
	MEM_SIZE =	4096,
    };

    enum {
    BASE =      0x000000,
    TOP =       0x001000,
    APP_LO =    0x000000,
    APP_CODE =  0x000000,
    APP_DATA =  0x000150,
    APP_HI =    0x00ffff,
    PHY_MEM =   0x000200,
    IO_MEM =    0x000020,
    SYS =       0x000000,
    INT_VEC =   0x000000,
    SYS_INFO =  0x001000,
    SYS_CODE =  0x000000,
    SYS_DATA =  0x000150,
    SYS_STACK = 0x0010ff
    };
};

template <class Imp> struct IO_Map;
template <> struct IO_Map<AT90CAN128>
{
    enum {
	PINA		= 0x00,
	DDRA		= 0x01,
	PORTA		= 0x02,
	PINB		= 0x03,
	DDRB		= 0x04,
	PORTB		= 0x05,
	PINC		= 0x06,
	DDRC		= 0x07,
	PORTC		= 0x08,   
	PIND		= 0x09,
	DDRD		= 0x0A,
	PORTD		= 0x0B,
	PINE		= 0x0C,
	DDRE		= 0x0D,
	PORTE		= 0x0E,
	PINF		= 0x0F,
	DDRF		= 0x10,
	PORTF		= 0x11,
	PING		= 0x12,
	DDRG		= 0x13,
	PORTG		= 0x14,
	TIFR0		= 0x15,
	TIFR1		= 0x16,
	TIFR2		= 0x17,
	TIFR3		= 0x18,

	// 0x19 through 0x1B are reserved
	
	EIFR		= 0x1C, 
	EIMSK		= 0x1D,
	GPIOR0		= 0x1E,
	EECR		= 0x1F,
	EEDR		= 0x20,
	EEARL		= 0x21,
	EEARH		= 0x22,
	GTCCR		= 0x23,
	TCCR0A		= 0x24,

	// 0x25 is reserved

	TCNT0		= 0x26,
	OCR0A		= 0x27,

	// 0x28, 0x29 are reserved

	GPIOR1		= 0x2A,
	GPIOR2		= 0x2B,
	SPCR		= 0x2C,
	SPSR		= 0x2D,
	SPDR		= 0x2E,

	// 0x2F is reserved

	ACSR		= 0x30,
	OCDR		= 0x31,

	// 0x32 is reserved

	SMCR		= 0x33,
	MCUSR		= 0x34,
	MCUCR		= 0x35,

	// 0x36 is reserved

	SPMCSR		= 0x37,

	// 0x38 through 0x3A are reserved

	RAMPZ		= 0x3B,

	// 0x3C is reserved

	SPL		= 0x3D,
	SPH		= 0x3E,
	SREG		= 0x3F,
	
	// beginning of LD/ST-only registers

	WDTCT		= 0x40,
	CLKPR		= 0x41,

	// 0x42 through 0x45 are reserved

	OSCCAL		= 0x46,

	// 0x47, 0x48 are reserved
	
	EICRA		= 0x49,
	EICRB		= 0x4A,

	// 0x4A through 0x4D are reserved

	TIMSK0		= 0x4E,
	TIMSK1		= 0x4F,
	TIMSK2		= 0x50,
	TIMSK3		= 0x51,

	// 0x52, 0x53 are reserved

	XMCRA		= 0x54,
	XMCRB		= 0x55,

	// 0x56, 0x57 are reserved

	ADCL		= 0x58,
	ADCH		= 0x59,
	ADCSRA		= 0x5A,
	ADCSRB		= 0x5B,
	ADMUX		= 0x5C,

	// 0x5D is reserved

	DIDR		= 0x5E,
	DIDR1		= 0x5F,
	TCCR1A		= 0x60,
	TCCR1B		= 0x61,
	TCCR1C		= 0x62,

	// 0x63 is reserved

	TCNT1L		= 0x64,
	TCNT1H		= 0x65,
	ICR1L		= 0x66,
	ICR1H		= 0x67,
	OCR1AL		= 0x68,
	OCR1AH		= 0x69,
	OCR1BL		= 0x6A,
	OCR1BH		= 0x6B,
	OCR1CL		= 0x6C,
	OCR1CH		= 0x6D,

	// 0x6E, 0x6F are reserved

	TCCR3A		= 0x70,
	TCCR3B		= 0x71,
	TCCR3C		= 0x72,
	
	// 0x73 is reserved

	TCNT3L		= 0x74,
	TCNT3H		= 0x75,
	ICR3L		= 0x76,
	ICR3H		= 0x77,
	OCR3AL		= 0x78,
	OCR3AH		= 0x79,
	OCR3BL		= 0x7A,
	OCR3BH		= 0x7B,
	OCR3CL		= 0x7C,
	OCR3CH		= 0x7D,

	// 0x7E through 0x9F are reserved

	TCCR2A		= 0x90,
	
	// 0x91 is reserved
	
	TCNT2		= 0x92,
	OCR2A		= 0x93,
	
	// 0x94, 0x95 are reserved
	
	ASSR		= 0x96,

	// 0x97 is reserved

	TWBR		= 0x98,
	TWSR		= 0x99,
	TWAR		= 0x9A,
	TWDR		= 0x9B,
	TWCR		= 0x9C,

	// 0x9D through 0x9F are reserved

	UCSR0A		= 0xA0,
	UCSR0B		= 0xA1,
	UCSR0C		= 0xA2,
	
	// 0xA3 is reserved

	UBRR0L		= 0xA4,
	UBRR0H		= 0xA5,
	UDR0		= 0xA6,

	// 0xA7 is reserved

	UCSR1A		= 0xA8,
	UCSR1B		= 0xA9,
	UCSR1C		= 0xAA,

	// 0xAB is reserved
	
	UBRR1L		= 0xAC,
	UBRR1H		= 0xAD,
	UDR1		= 0xAE,

	// 0xAF through 0xB7 are reserved

	CANGCON		= 0xB8,
	CANGSTA		= 0xB9,
	CANGIT		= 0xBA,
	CANGIE		= 0xBB,
	CANEN2		= 0xBC,
	CANEN1		= 0xBD,
	CANIE2		= 0xBE,
	CANIE1		= 0xBF,
	CANSIT2		= 0xC0,
	CANSIT1		= 0xC1,
	CANBT1		= 0xC2,
	CANBT2		= 0xC3,
	CANBT3		= 0xC4,
	CANTCON		= 0xC5,
	CANTIML		= 0xC6,
	CANTIMH		= 0xC7,
	CANTTCL		= 0xC8,
	CANTTCH		= 0xC9,
	CANTEC		= 0xCA,
	CANREC		= 0xCB,
	CANHPMOB	= 0xCC,
	CANPAGE		= 0xCD,
	CANSTMOB	= 0xCE,
	CANCDMOB	= 0xCF,
	CANIDT4		= 0xD0,
	CANIDT3		= 0xD1,
	CANIDT2		= 0xD2,
	CANIDT1		= 0xD3,
	CANIDM4		= 0xD4,
	CANIDM3		= 0xD5,
	CANIDM2		= 0xD6,
	CANIDM1		= 0xD7,
	CANSTML		= 0xD8,
	CANSTMH		= 0xD9,
	CANMSG		= 0xDA
	// 0xDB through 0xDF are reserved
    };

    typedef volatile unsigned char IO_Port;  
    struct IO_Ports {
	IO_Port pina;		// [0x00]
	IO_Port ddra;		// [0x01]
	IO_Port porta;		// [0x02]
	IO_Port pinb;		// [0x03]
	IO_Port ddrb;		// [0x04]
	IO_Port portb;		// [0x05]
	IO_Port pinc;		// [0x06]
	IO_Port ddrc;		// [0x07]
	IO_Port portc;		// [0x08]   
	IO_Port pind;		// [0x09]
	IO_Port ddrd;		// [0x0a]
	IO_Port portd;		// [0x0b]
	IO_Port pine;		// [0x0c]
	IO_Port ddre;		// [0x0d]
	IO_Port porte;		// [0x0e]
	IO_Port pinf;		// [0x0f]
	IO_Port ddrf;		// [0x10]
	IO_Port portf;		// [0x11]
	IO_Port ping;		// [0x12]
	IO_Port ddrg;		// [0x13]
	IO_Port portg;		// [0x14]
	IO_Port tifr0;		// [0x15]
	IO_Port tifr1;		// [0x16]
	IO_Port tifr2;		// [0x17]
	IO_Port tifr3;		// [0x18]

	IO_Port reserved1;	// [0x19]
	IO_Port reserved2;	// [0x1a]
	IO_Port reserved3;	// [0x1b]

	IO_Port eifr;		// [0x1c] 
	IO_Port eimsk;		// [0x1d]
	IO_Port gpior0;		// [0x1e]
	IO_Port eecr;		// [0x1f]
	IO_Port eedr;		// [0x20]
	IO_Port eearl;		// [0x21]
	IO_Port eearh;		// [0x22]
	IO_Port gtccr;	 	// [0x23]
	IO_Port tccr0a;		// [0x24]

	IO_Port reserved5;	// [0x25]

	IO_Port tcnt0;		// [0x26]
	IO_Port ocr0a;		// [0x27]

	IO_Port reserved6;	// [0x28]
	IO_Port reserved7;	// [0x29]

	IO_Port gpior1;		// [0x2a]
	IO_Port gpior2;		// [0x2b]
	IO_Port spcr;		// [0x2c]
	IO_Port spsr;		// [0x2d]
	IO_Port spdr;		// [0x2e]

	IO_Port reserved8;	// [0x2f]

	IO_Port acsr;		// [0x30]
	IO_Port ocdr;		// [0x31]

	IO_Port reserved9;	// [0x32]

	IO_Port smcr;		// [0x33]
	IO_Port mcusr;		// [0x34]
	IO_Port mcucr;	 	// [0x35]

	IO_Port reserved10;	// [0x36]

	IO_Port spmcsr;		// [0x37]

	IO_Port reserved11;	// [0x38]
	IO_Port reserved12;	// [0x39]
	IO_Port reserved13;	// [0x3a]

	IO_Port rampz;		// [0x3b]

	IO_Port reserved14;	// [0x3c]

	IO_Port spl;		// [0x3d]
	IO_Port sph;		// [0x3e]
	IO_Port sreg;		// [0x3f]
	
	IO_Port wdtct;		// [0x40]
	IO_Port clkpr;		// [0x41]

	IO_Port reserved15;	// [0x42]
	IO_Port reserved16;	// [0x43]
	IO_Port reserved17;	// [0x44]
	IO_Port reserved18;	// [0x45]

	IO_Port osccal;		// [0x46]
	
	IO_Port reserved19;	// [0x47]
	IO_Port reserved4;	// [0x48]
	
	IO_Port eicra ;		// [0x49]
	IO_Port eicrb ;		// [0x4a]

	IO_Port reserved20;	// [0x4b]
	IO_Port reserved21;	// [0x4c]
	IO_Port reserved22;	// [0x4d]

	IO_Port timsk0;		// [0x4e]
	IO_Port timsk1;		// [0x4f]
	IO_Port timsk2;		// [0x50]
	IO_Port timsk3;		// [0x51]

	IO_Port reserved23;	// [0x52]
	IO_Port reserved24;	// [0x53]

	IO_Port xmcra;		// [0x54]
	IO_Port xmcrb ;		// [0x55]

	IO_Port reserved25;	// [0x56]
	IO_Port reserved26;	// [0x57]

	IO_Port adcl;		// [0x58]
	IO_Port adch;		// [0x59]
	IO_Port adcsra;		// [0x5a]
	IO_Port adcsrb;		// [0x5b]
	IO_Port admux;		// [0x5c]

	IO_Port reserved27;	// [0x5d]

	IO_Port didr0;		// [0x5e]
	IO_Port didr1;		// [0x5f]
	IO_Port tccr1a;		// [0x60]
	IO_Port tccr1b;		// [0x61]
	IO_Port tccr1c;		// [0x62]

	IO_Port reserved28;	// [0x63]

	IO_Port tcnt1l;		// [0x64]
	IO_Port tcnt1h;		// [0x65]
	IO_Port icr1l;		// [0x66]
	IO_Port icr1h;		// [0x67]
	IO_Port ocr1al;		// [0x68]
	IO_Port ocr1ah;		// [0x69]
	IO_Port ocr1bl;		// [0x6a]
	IO_Port ocr1bh;		// [0x6b]
	IO_Port ocr1cl;		// [0x6c]
	IO_Port ocr1ch;		// [0x6d]

	IO_Port reserved29;	// [0x6e]
	IO_Port reserved30;	// [0x6f]

	IO_Port tccr3a;		// [0x70]
	IO_Port tccr3b;		// [0x71]
	IO_Port tccr3c;		// [0x72]
	
	IO_Port reserved31;	// [0x73]

	IO_Port tcnt3l;		// [0x74]
	IO_Port tcnt3h;		// [0x75]
	IO_Port icr3l;		// [0x76]
	IO_Port icr3h;		// [0x77]
	IO_Port ocr3al;		// [0x78]
	IO_Port ocr3ah;		// [0x79]
	IO_Port ocr3bl;		// [0x7a]
	IO_Port ocr3bh;		// [0x7b]
	IO_Port ocr3cl;		// [0x7c]
	IO_Port ocr3ch;		// [0x7d]

	IO_Port reserved32;	// [0x7e]
	IO_Port reserved33;	// [0x7f]
	IO_Port reserved34;	// [0x80]
	IO_Port reserved35;	// [0x81]
	IO_Port reserved36;	// [0x82]
	IO_Port reserved37;	// [0x83]
	IO_Port reserved38;	// [0x84]
	IO_Port reserved39;	// [0x85]
	IO_Port reserved40;	// [0x86]
	IO_Port reserved41;	// [0x87]
	IO_Port reserved42;	// [0x88]
	IO_Port reserved43;	// [0x89]
	IO_Port reserved44;	// [0x8a]
	IO_Port reserved45;	// [0x8b]
	IO_Port reserved46;	// [0x8c]
	IO_Port reserved47;	// [0x8d]
	IO_Port reserved48;	// [0x8e]
	IO_Port reserved49;	// [0x8f]

	IO_Port tccr2a;		// [0x90]
	
	IO_Port reserved50;	// [0x91]

	IO_Port tcnt2;		// [0x92]
	IO_Port ocr2a;		// [0x93]
	
	IO_Port reserved51;	// [0x94]
	IO_Port reserved52;	// [0x95]
	
	IO_Port assr;		// [0x96]

	IO_Port reserved53;	// [0x97]

	IO_Port twbr;		// [0x98]
	IO_Port twsr;		// [0x99]
	IO_Port twar;		// [0x9a]
	IO_Port twdr;		// [0x9b]
	IO_Port twcr;		// [0x9c]

	IO_Port reserved54;	// [0x9d]
	IO_Port reserved55;	// [0x9e]
	IO_Port reserved56;	// [0x9f]
	IO_Port ucsr0a;		// [0xa0]
	IO_Port ucsr0b;		// [0xa1]
	IO_Port ucsr0c;		// [0xa2]
	
	IO_Port reserved57;	// [0xa3]

	IO_Port ubrr0l;		// [0xa4]
	IO_Port ubbr0h;		// [0xa5]
	IO_Port udr0;		// [0xa6]

	IO_Port reserved58;	// [0xa7]

	IO_Port ucsr1a;		// [0xa8]
	IO_Port ucsr1b;		// [0xa9]
	IO_Port ucsr1c;		// [0xaa]

	IO_Port reserved59;	// [0xab]
	
	IO_Port ubrr1l;		// [0xac]
	IO_Port ubrr1h;		// [0xad]
	IO_Port udr1;		// [0xae]

	IO_Port reserved60;	// [0xaf]
	IO_Port reserved61;	// [0xb0]
	IO_Port reserved62;	// [0xb1]
	IO_Port reserved63;	// [0xb2]
	IO_Port reserved64;	// [0xb3]
	IO_Port reserved65;	// [0xb4]
	IO_Port reserved66;	// [0xb5]
	IO_Port reserved67;	// [0xb6]
	IO_Port reserved68;	// [0xb7]

	IO_Port cangcon;	// [0xb8]
	IO_Port cangsta;	// [0xb9]
	IO_Port cangit;		// [0xba]
	IO_Port cangie;		// [0xbb]
	IO_Port canen2;		// [0xbc]
	IO_Port canen1;		// [0xbd]
	IO_Port canie2;		// [0xbe]
	IO_Port canie1;		// [0xbf]
	IO_Port cansit2;	// [0xc0]
	IO_Port cansit1;	// [0xc1]
	IO_Port canbt1;		// [0xc2]
	IO_Port canbt2;		// [0xc3]
	IO_Port canbt3;		// [0xc4]
	IO_Port cantcon;	// [0xc5]
	IO_Port cantiml;	// [0xc6]
	IO_Port cantimh;	// [0xc7]
	IO_Port canttcl;	// [0xc8]
	IO_Port canttch;	// [0xc9]
	IO_Port cantec;		// [0xca]
	IO_Port canrec;		// [0xcb]
	IO_Port canhpmob;	// [0xcc]
	IO_Port canpage;	// [0xcd]
	IO_Port canstmob;	// [0xce]
	IO_Port cancdmob;	// [0xcf]
	IO_Port canidt4;	// [0xd0]
	IO_Port canidt3;	// [0xd1]
	IO_Port canidt2;	// [0xd2]
	IO_Port canidt1;	// [0xd3]
	IO_Port canidm4;	// [0xd4]
	IO_Port canidm3;	// [0xd5]
	IO_Port canidm2;	// [0xd6]
	IO_Port canidm1;	// [0xd7]
	IO_Port canstml;	// [0xd8]
	IO_Port canstmh;	// [0xd9]
	IO_Port canmsg;		// [0xda]

	IO_Port reserved69;	// [0xdb]
	IO_Port reserved70;	// [0xdc]
	IO_Port reserved71;	// [0xdd]
	IO_Port reserved72;	// [0xde]
	IO_Port reserved73;	// [0xdf]

    };
};

//
// Memory Map for A90CAN128
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
// 0x0000 -+- - - - - - - - - - - - -+- BA1
//         | 			     |
//         : GP Registers            :
//         |                         |
// 0x0020 -+- - - - - - - - - - - - -+- IO_MEM
//         | 			     |
//         : IO Registers            :
//         |                         |
// 0x0100 -+- - - - - - - - - - - - -+- PHY_MEM, SYS_INFO
//         | 			     |
//         : System_Info             :
//         |                         |
// 0x0150 -+- - - - - - - - - - - - -+- APP_DATA, SYS_DATA
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
// 0x10ff -+- - - - - - - - - - - - -+- RAM End
//

__END_SYS

#endif
