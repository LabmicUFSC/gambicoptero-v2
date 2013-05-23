// EPOS AVR Serial Peripheral Interface Common Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __avr_spi_h
#define __avr_spi_h

#include <system/memory_map.h>
#include <cpu.h>
#include <spi.h>

__BEGIN_SYS

class AVR_SPI
{
protected:
    AVR_SPI() {	configure(); }

private:
    typedef IO_Map<Machine> IO;
    typedef AVR8::Reg8 Reg8;

public:
    // SPI IO Registers
    enum {
        SPCR     = IO::SPCR,
        SPSR     = IO::SPSR,
        SPDR     = IO::SPDR
    };

    // SPI IO Register Bits
    enum {
        // SPCR
        SPIE     = 0x80,
        SPE      = 0x40,
        DORD     = 0x20,
        MSTR     = 0x10,
        CPOL     = 0x08,
        CPHA     = 0x04,
        SPR1     = 0x02,
        SPR0     = 0x01,
        // SPSR
        SPIF     = 0x80,
        WCOL     = 0x40,
        SPI2X    = 0x01,
    };

    enum {
	SPI_PORT_DIR = IO::DDRB,
	SS_PIN       = 0x01,
	SCK_PIN      = 0x02,
	MOSI_PIN     = 0x04,
	MISO_PIN     = 0x08,
    };

public:  
    void configure(unsigned char mode = 0, unsigned char order = 0,
		   unsigned char clk_polarity = 0,
		   unsigned char clk_phase = 0) {
	ddr(ddr() | (MISO_PIN & ~MOSI_PIN &  ~SS_PIN & ~SCK_PIN));
	spcr(SPE);
    }

    bool complete() { return (spsr() & SPIF); } 

    void int_enable() { spcr(spcr() | SPIE); }
    void int_disable() { spcr(spcr() & ~SPIE); }

    char get() { /* while(!complete()); */ return spdr(); }
    void put(char c) { spdr(c);  /* while(!complete());  */ }

protected:
    static Reg8 spcr(){ return AVR8::in8(SPCR); }
    static void spcr(Reg8 value){ AVR8::out8(SPCR,value); }   
    static Reg8 spsr(){ return AVR8::in8(SPSR); }
    static void spsr(Reg8 value){ AVR8::out8(SPSR,value); }       
    static Reg8 spdr(){ return AVR8::in8(SPDR); }
    static void spdr(Reg8 value){ AVR8::out8(SPDR,value); }    
    static Reg8 ddr(){ return CPU::in8(SPI_PORT_DIR); }
    static void ddr(Reg8 value){ CPU::out8(SPI_PORT_DIR,value); }
};

__END_SYS

#endif
