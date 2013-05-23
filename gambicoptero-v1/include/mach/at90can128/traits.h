#ifndef __at90can128_traits_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __at90can128_traits_h

#include "../avr_common/traits.h"

__BEGIN_SYS

class AT90CAN128_Common;
template <> struct Traits<AT90CAN128_Common>: public Traits<AVR_Common>
{
};

template <> struct Traits<AT90CAN128>: public Traits<AT90CAN128_Common>, public Traits<AVR_Machine>
{
    static const unsigned int MAX_CPUS = 1;
    static const unsigned long long CLOCK = 8000000;

    static const unsigned int BOOT_IMAGE_ADDR = 0x0000;

    static const unsigned int APPLICATION_STACK_SIZE = 256;
    static const unsigned int APPLICATION_HEAP_SIZE = 512;

    static const unsigned int SYSTEM_STACK_SIZE = 64;
    static const unsigned int SYSTEM_HEAP_SIZE = 8 *  APPLICATION_STACK_SIZE;

    static const unsigned int PRIORITY_HEAP_BASE_ADDR = 0;
    static const unsigned int PRIORITY_HEAP_SIZE = 4;
    static const unsigned int PRIORITY_HEAP_TOP_ADDR = PRIORITY_HEAP_BASE_ADDR + PRIORITY_HEAP_SIZE - 1;
};

template <> struct Traits<AT90CAN128_Timer_1>: public Traits<AT90CAN128_Common>
{
    // Should be between 28 and 7200 Hz
    static const int FREQUENCY = 720; // Hz
};

template <> struct Traits<AT90CAN128_Timer_2>: public Traits<AT90CAN128_Common>
{
    // Should be between 28 and 7200 Hz
    static const int FREQUENCY = 720; // Hz
};

template <> struct Traits<AT90CAN128_Timer_3>: public Traits<AT90CAN128_Common>
{
    // Should be between 28 and 7200 Hz
    static const int FREQUENCY = 100; // Hz
};

template <> struct Traits<AT90CAN128_RTC>: public Traits<AT90CAN128_Common>
{
    static const unsigned int EPOCH_DAY = 1;
    static const unsigned int EPOCH_MONTH = 1;
    static const unsigned int EPOCH_YEAR = 1970;
    static const unsigned int EPOCH_DAYS = 719499;
};

template <> struct Traits<AT90CAN128_ADC>: public Traits<AT90CAN128_Common>, public Traits<AVR_ADC>
{
};

template <> struct Traits<AT90CAN128_UART>: public Traits<AT90CAN128_Common>, public Traits<AVR_UART>
{
    static const bool enabled = false;
};

template <> struct Traits<AT90CAN128_CAN>: public Traits<void>
{
    static const bool enabled = true;
    static const int MAX_MOBS      = 15; // AVR message objects 0..14
    static const int REGISTER_MOBS = 8;  // mobs per register(CANIE/CANEN)
    static const int MAX_DATASIZE  = 8;  // Message data size(bytes)
};

template <> struct Traits<AT90CAN128_Display>: public Traits<void>
{
    static const int COLUMNS = 80;
    static const int LINES = 24;
    static const int TAB_SIZE = 8;
};

__END_SYS

#endif
