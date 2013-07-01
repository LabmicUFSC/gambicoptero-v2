// EPOS ATMega1281 Flash Mediator Declarations

#ifndef __atmega1281_flash_h
#define __atmega1281_flash_h

#include "../avr_common/flash.h"

__BEGIN_SYS

class ATMega1281_Flash: public Flash_Common, private AVR_Flash
{
public:
    typedef Flash_Common::Address Address;

public:
    ATMega1281_Flash() {}

    unsigned char read(const Address & a) { return AVR_Flash::read(a); }
    int write(Address a, unsigned char * d, unsigned int s) { return AVR_Flash::write(a, d, s); }
};

__END_SYS

#endif

