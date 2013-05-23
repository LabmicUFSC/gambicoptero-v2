// EPOS AVR_ADC Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/avr_common/adc.h>

__BEGIN_SYS

bool AVR_ADC::_in_use = false;
Traits<AVR_ADC>::Power_Modes AVR_ADC::_op_mode;

__END_SYS
