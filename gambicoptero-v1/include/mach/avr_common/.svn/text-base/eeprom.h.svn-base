// EPOS AVR EEPROM Mediator Common Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __avr_eeprom_h
#define __avr_eeprom_h

#include <eeprom.h>

__BEGIN_SYS

class AVR_EEPROM
{
protected:
    AVR_EEPROM() {}

private:
    typedef IO_Map<Machine> IO;
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;
    typedef CPU::Reg16 Address;

    static const unsigned int SIZE = Traits<EEPROM>::SIZE; // bytes

public:
    //EEPROM IO Register Bit Offsets
    enum {
#if defined (__AVR_ATmega1281__)
	//EECR
        EEPM1 = 5,
        EEPM0 = 4,
#endif
	//EECR
	EERIE = 3,
	EEMWE = 2, // EEMPE on atmega1281
	EEWE  = 1, // EEPE  on atmega1281
	EERE  = 0,
        //EEARH
        EEAR8 = 1,
	//EEARL
        EEAR7 = 7,
        EEAR6 = 6,
        EEAR5 = 5,
        EEAR4 = 4,
        EEAR3 = 3,
        EEAR2 = 2,
        EEAR1 = 1,
        EEAR0 = 0,
    };

public:
    unsigned char read(const Address & addr) {
        // wait for completion of previous write
        while (eecr() & (1 << EEWE));
        eearhl(addr);
        eecr(eecr() | (1 << EERE));

        return eedr();
    }

    void write(const Address & addr, unsigned char data) {
        // wait for completion of previous write
        while (eecr() & (1 << EEWE));
        eearhl(addr);
        eedr(data);
        eecr(eecr() | 0x4);
        eecr(eecr() | 0x2);
    }

    int size() const { return SIZE; }

private:
    Reg8 eedr() { return CPU::in8(IO::EEDR); }
    void eedr(Reg8 value) { CPU::out8(IO::EEDR,value); }
    Reg8 eecr() { return CPU::in8(IO::EECR); }
    void eecr(Reg8 value) { CPU::out8(IO::EECR,value); }
    Reg8 eearl() { return CPU::in8(IO::EEARL); }
    void eearl(Reg8 value) { CPU::out8(IO::EEARL,value); }
    Reg8 eearh() { return CPU::in8(IO::EEARH); }
    void eearh(Reg8 value) { CPU::out8(IO::EEARH,value); }
    Reg16 eearhl() {
        Reg16 value = eearl();
	value |= ((Reg16)eearh()) << 8;
	return value;
    }
    void eearhl(Reg16 value) {
        eearh((Reg8)(value >> 8));
	eearl((Reg8)value);
    }
};

__END_SYS

#endif

