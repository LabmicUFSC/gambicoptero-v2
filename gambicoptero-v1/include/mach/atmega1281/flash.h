// EPOS ATMega1281 Flash Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

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

