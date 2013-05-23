#ifndef __atmega1281_traits_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __atmega1281_traits_h

#include "../avr_common/traits.h"

__BEGIN_SYS

class ATMega1281_Common;
template <> struct Traits<ATMega1281_Common>: public Traits<AVR_Common>
{
};

template <> struct Traits<ATMega1281>: public Traits<ATMega1281_Common>, public Traits<AVR_Machine>
{
    static const unsigned int MAX_CPUS = 1;
    static const unsigned long long CLOCK     = 8000000;
    static const unsigned int BOOT_IMAGE_ADDR = 0x0000;

    static const unsigned int APPLICATION_STACK_SIZE = 256; 
    static const unsigned int APPLICATION_HEAP_SIZE  = 2048;

    static const unsigned int SYSTEM_STACK_SIZE = 0;   // not used
    static const unsigned int SYSTEM_HEAP_SIZE  = 4096;

    static const unsigned int PRIORITY_HEAP_BASE_ADDR = 0;
    static const unsigned int PRIORITY_HEAP_SIZE = 4;
    static const unsigned int PRIORITY_HEAP_TOP_ADDR = PRIORITY_HEAP_BASE_ADDR + PRIORITY_HEAP_SIZE - 1;
};

template <> struct Traits<ATMega1281_Timer_1>: public Traits<ATMega1281_Common>
{
    static const int FREQUENCY = 15; // Hz
};

template <> struct Traits<ATMega1281_Timer_2>: public Traits<ATMega1281_Common>
{
    static const int FREQUENCY = 100; // Hz
};

template <> struct Traits<ATMega1281_Timer_3>: public Traits<ATMega1281_Common>
{
    static const int FREQUENCY = 100; // Hz
};

template <> struct Traits<ATMega1281_RTC>: public Traits<ATMega1281_Common>
{
    static const unsigned long EPOCH_DAY   = 1;
    static const unsigned long EPOCH_MONTH = 1;
    static const unsigned long EPOCH_YEAR  = 1970;
    static const unsigned long EPOCH_DAYS  = 719499;
};

template <> struct Traits<ATMega1281_EEPROM>: public Traits<ATMega1281_Common>
{
    static const unsigned int SIZE = 4096; // bytes
};

template <> struct Traits<ATMega1281_Flash>: public Traits<ATMega1281_Common>
{
    static const unsigned int PAGE_SIZE = 256; // bytes
};

template <> struct Traits<ATMega1281_UART>: public Traits<ATMega1281_Common>, public Traits<AVR_UART>
{
    static const unsigned int DEFAULT_UNIT = 1;
};

template <> struct Traits<ATMega1281_ADC>: public Traits<ATMega1281_Common>, public Traits<AVR_ADC>
{
};

template <> struct Traits<ATMega1281_SPI>: public Traits<ATMega1281_Common>
{
};

template <> struct Traits<ATMega1281_NIC>: public Traits<void>
{
    static const bool enabled = true;

    typedef LIST<ATMega1281_Radio> NICS;

    static const unsigned int RADIO_UNITS = NICS::Count<ATMega1281_Radio>::Result;
};

template <> struct Traits<ATMega1281_Temperature_Sensor>: public Traits<void>
{
    typedef LIST<SHT11_Temperature> SENSORS;
};

template <> struct Traits<ATMega1281_Humidity_Sensor>: public Traits<void>
{
    typedef LIST<SHT11_Humidity> SENSORS;
};

__END_SYS

#endif
