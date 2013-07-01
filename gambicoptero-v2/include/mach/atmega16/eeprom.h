// EPOS ATMega16 EEPROM Mediator Declarations

#ifndef __atmega16_eeprom_h
#define __atmega16_eeprom_h

#include "../avr_common/eeprom.h"

__BEGIN_SYS

class ATMega16_EEPROM: public EEPROM_Common, private AVR_EEPROM
{
public:
    typedef EEPROM_Common::Address Address;

public:
    ATMega16_EEPROM() {}

    unsigned char read(const Address & a) { return AVR_EEPROM::read(a); }
    void write(const Address & a, unsigned char d) { AVR_EEPROM::write(a, d); }

    int size() const { return AVR_EEPROM::size(); }
};

__END_SYS


#endif
