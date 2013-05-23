// EPOS ATMega16 ADC Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega16_adc_h
#define __atmega16_adc_h

#include "../avr_common/adc.h"

__BEGIN_SYS

class ATMega16_ADC: public ADC_Common, private AVR_ADC
{
public:
    ATMega16_ADC() : AVR_ADC(SINGLE_ENDED_ADC0, SYSTEM_REF,
			     SINGLE_CONVERSION_MODE, CLOCK >> 7) {}
    ATMega16_ADC(unsigned char channel, Hertz frequency) 
	: AVR_ADC(channel, SYSTEM_REF, SINGLE_CONVERSION_MODE, frequency) {}
    ATMega16_ADC(unsigned char channel, unsigned char reference,
		 unsigned char trigger, Hertz frequency) 
	: AVR_ADC(channel, reference, trigger, frequency) {}

    void config(unsigned char channel, unsigned char reference,
		unsigned char trigger, Hertz frequency) {
	AVR_ADC::config(channel, reference, trigger, frequency);
    }
    void config(unsigned char * channel, unsigned char * reference,
		unsigned char * trigger, Hertz * frequency) {
	AVR_ADC::config(channel, reference, trigger, frequency);
    }

    int sample() { return AVR_ADC::sample(); }
    int get() { return AVR_ADC::get(); }
    bool finished() { return AVR_ADC::finished(); }

    void reset() { return AVR_ADC::reset(); }
};

__END_SYS

#endif
