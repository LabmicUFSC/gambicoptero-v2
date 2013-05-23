#ifndef __avr8_traits_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __avr8_traits_h

__BEGIN_SYS

// AVR8
template <> struct Traits<AVR8>: public Traits<void>
{
    enum Power_Modes
    {
        FULL                = 0,
        IDLE                = 1,
        ADC_NOISE_REDUCTION = 2,
        POWER_DOWN          = 3,
        POWER_SAVE          = 4,
        NATIVE_STANDBY      = 5,//For this mode an external oscilator is needed
        EXTENDED_STANDBY    = 6,//For this mode an external oscilator is needed
        LIGHT               = IDLE,
        STANDBY             = ADC_NOISE_REDUCTION,
        OFF                 = POWER_DOWN
    };
    static const bool power_management = true;
};

template <> struct Traits<AVR8_TSC>: public Traits<void>
{
	static const bool enabled = true;
};

template <> struct Traits<AVR8_MMU>: public Traits<void>
{
};

__END_SYS

#endif
