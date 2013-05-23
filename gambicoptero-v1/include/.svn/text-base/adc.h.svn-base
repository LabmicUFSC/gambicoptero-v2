// EPOS ADC Mediator Common Package

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __adc_h
#define __adc_h

#include <system/config.h>

__BEGIN_SYS

class ADC_Common
{
protected:
    ADC_Common() {}

public:
    enum Channel {
        SINGLE_ENDED_ADC0 = 0,
        SINGLE_ENDED_ADC1 = 1,
        SINGLE_ENDED_ADC2 = 2,
        SINGLE_ENDED_ADC3 = 3,
        SINGLE_ENDED_ADC4 = 4,
        SINGLE_ENDED_ADC5 = 5,
        SINGLE_ENDED_ADC6 = 6,
        SINGLE_ENDED_ADC7 = 7
    };

    enum Reference {
        SYSTEM_REF   = 0,
        EXTERNAL_REF = 1,
        INTERNAL_REF = 3
    };

    enum Trigger {
        SINGLE_CONVERSION_MODE = 0,
        FREE_RUNNING_MODE      = 1,
    };
};

__END_SYS

#ifdef __ADC_H
#include __ADC_H
#endif

#endif
