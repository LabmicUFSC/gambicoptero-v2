// EPOS AVR_ADC Mediator Implementation

#include <mach/avr_common/adc.h>

__BEGIN_SYS

bool AVR_ADC::_in_use = false;
Traits<AVR_ADC>::Power_Modes AVR_ADC::_op_mode;

__END_SYS
