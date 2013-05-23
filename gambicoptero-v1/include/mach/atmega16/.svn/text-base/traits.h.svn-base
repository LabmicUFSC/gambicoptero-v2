#ifndef __atmega16_traits_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __atmega16_traits_h

#include "../avr_common/traits.h"

__BEGIN_SYS

class ATMega16_Common;
template <> struct Traits<ATMega16_Common>: public Traits<AVR_Common>
{
};

template <> struct Traits<ATMega16>: public Traits<ATMega16_Common>, public Traits<AVR_Machine>
{
    static const unsigned int MAX_CPUS = 1;
    static const unsigned long long CLOCK = 8000000;
    static const unsigned int BOOT_IMAGE_ADDR = 0x0000;

    static const unsigned int APPLICATION_STACK_SIZE = 128;
    static const unsigned int APPLICATION_HEAP_SIZE = 256;

    static const unsigned int SYSTEM_STACK_SIZE = 64;
    static const unsigned int SYSTEM_HEAP_SIZE = 4 *  APPLICATION_STACK_SIZE;

    static const unsigned int PRIORITY_HEAP_BASE_ADDR = 0;
    static const unsigned int PRIORITY_HEAP_SIZE = 4;
    static const unsigned int PRIORITY_HEAP_TOP_ADDR = PRIORITY_HEAP_BASE_ADDR + PRIORITY_HEAP_SIZE - 1;
};

template <> struct Traits<ATMega16_Timer_1>: public Traits<ATMega16_Common>
{
    static const int FREQUENCY = 800; // Hz
};

template <> struct Traits<ATMega16_Timer_2>: public Traits<ATMega16_Common>
{
    static const int FREQUENCY = 800; // Hz
};

template <> struct Traits<ATMega16_Timer_3>: public Traits<ATMega16_Common>
{
    static const int FREQUENCY = 800; // Hz
};


template <> struct Traits<ATMega16_RTC>: public Traits<ATMega16_Common>
{
    static const unsigned int EPOCH_DAY = 1;
    static const unsigned int EPOCH_MONTH = 1;
    static const unsigned int EPOCH_YEAR = 1970;
    static const unsigned int EPOCH_DAYS = 719499;
};

template <> struct Traits<ATMega16_EEPROM>: public Traits<ATMega16_Common>
{
    static const unsigned int SIZE = 512; // bytes
};

template <> struct Traits<ATMega16_UART>: public Traits<ATMega16_Common>, public Traits<AVR_UART>
{
};

template <> struct Traits<ATMega16_ADC>: public Traits<ATMega16_Common>, public Traits<AVR_ADC>
{
};

template <> struct Traits<ATMega16_SPI>: public Traits<ATMega16_Common>
{
};

template <> struct Traits<ATMega16_Display>: public Traits<ATMega16_Common>
{
    static const bool on_serial = true;
};

template <> struct Traits<ATMega16_NIC>: public Traits<void>
{
    static const bool enabled = false;

    typedef LIST<Radio> NICS;

    static const unsigned int RADIO_UNITS = NICS::Count<Radio>::Result;
};

__END_SYS

#endif
