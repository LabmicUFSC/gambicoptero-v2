// EPOS ATMega16 SPI Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega16_spi_h
#define __atmega16_spi_h

#include "../avr_common/spi.h"

__BEGIN_SYS

class ATMega16_SPI: public SPI_Common, private AVR_SPI
{
public:
    ATMega16_SPI() {}

    char get() { return AVR_SPI::get(); }
    void put(char c) { AVR_SPI::put(c); }
    void int_enable() { AVR_SPI::int_enable(); }
    void int_disable() { AVR_SPI::int_disable(); }
};

__END_SYS

#endif
