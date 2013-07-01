// EPOS ATMega16 SPI Mediator Declarations

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
