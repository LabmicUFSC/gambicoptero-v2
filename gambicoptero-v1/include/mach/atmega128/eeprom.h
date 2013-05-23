// EPOS ATMega128 EEPROM Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega128_eeprom_h
#define __atmega128_eeprom_h

#include "../avr_common/eeprom.h"

__BEGIN_SYS

class ATMega128_EEPROM: public EEPROM_Common, private AVR_EEPROM
{
public:
    typedef EEPROM_Common::Address Address;

public:
    ATMega128_EEPROM() {}

    unsigned char read(const Address & a) { return AVR_EEPROM::read(a); }
    void write(const Address & a, unsigned char d) { AVR_EEPROM::write(a, d); }

    int size() const { return AVR_EEPROM::size(); }
};

__END_SYS


#endif
