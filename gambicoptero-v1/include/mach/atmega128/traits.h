#ifndef __atmega128_traits_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __atmega128_traits_h

#include "../avr_common/traits.h"

__BEGIN_SYS

class ATMega128_Common;
template <> struct Traits<ATMega128_Common>: public Traits<AVR_Common>
{
};

template <> struct Traits<ATMega128>: public Traits<ATMega128_Common>, public Traits<AVR_Machine>
{
    static const unsigned long long CLOCK = 7372800; //Mica2
    static const unsigned int BOOT_IMAGE_ADDR = 0x0000;

    static const unsigned int APPLICATION_STACK_SIZE = 256;
    static const unsigned int APPLICATION_HEAP_SIZE = 512;

    static const unsigned int SYSTEM_STACK_SIZE = 64;
    static const unsigned int SYSTEM_HEAP_SIZE = 2000;//8 *  APPLICATION_STACK_SIZE;

    static const unsigned int PRIORITY_HEAP_BASE_ADDR = 0;
    static const unsigned int PRIORITY_HEAP_SIZE = 4;
    static const unsigned int PRIORITY_HEAP_TOP_ADDR = PRIORITY_HEAP_BASE_ADDR + PRIORITY_HEAP_SIZE - 1;

};

template <> struct Traits<ATMega128_Timer_1>: public Traits<ATMega128_Common>
{
    // Should be between 30 and 7200 Hz
    static const int FREQUENCY = 720; // Hz

};

template <> struct Traits<ATMega128_Timer_2>: public Traits<ATMega128_Common>
{
    // Should be between 30 and 7200 Hz
    static const int FREQUENCY = 720; // Hz

};

template <> struct Traits<ATMega128_Timer_3>: public Traits<ATMega128_Common>
{
    // Should be between 30 and 7200 Hz
    static const int FREQUENCY = 7200; // Hz

};

template <> struct Traits<ATMega128_RTC>: public Traits<ATMega128_Common>
{
    static const unsigned int EPOCH_DAY = 1;
    static const unsigned int EPOCH_MONTH = 1;
    static const unsigned int EPOCH_YEAR = 1970;
    static const unsigned long EPOCH_DAYS = 719499;
};

template <> struct Traits<ATMega128_EEPROM>: public Traits<ATMega128_Common>
{
    static const unsigned int SIZE = 4096; // bytes
};

template <> struct Traits<ATMega128_Flash>: public Traits<ATMega128_Common>
{
    static const unsigned int PAGE_SIZE = 256; // bytes
};

template <> struct Traits<ATMega128_UART>: public Traits<ATMega128_Common>, public Traits<AVR_UART>
{
};

template <> struct Traits<ATMega128_ADC>: public Traits<ATMega128_Common>, public Traits<AVR_ADC>
{
};

template <> struct Traits<ATMega128_SPI>: public Traits<ATMega128_Common>
{
};

template <> struct Traits<ATMega128_NIC>: public Traits<void>
{
    static const bool enabled = true;

    typedef LIST<CMAC> NICS;

    static const unsigned int RADIO_UNITS = NICS::Count<ATMega128_Radio>::Result;
};

template <> struct Traits<CMAC>: public Traits<void>
{
    static const bool enabled = true;
    static const unsigned int FREQUENCY = 0;
    static const unsigned int POWER = 5;
};

template <> struct Traits<ATMega128_Temperature_Sensor>: public Traits<void>
{
    typedef LIST<MTS300_Temperature> SENSORS;
};

template <> struct Traits<ATMega128_Photo_Sensor>: public Traits<void>
{
    typedef LIST<MTS300_Photo> SENSORS;
};

template <> struct Traits<ATMega128_Accelerometer>: public Traits<void>
{
    typedef LIST<ADXL202> SENSORS;
};

__END_SYS

#endif
