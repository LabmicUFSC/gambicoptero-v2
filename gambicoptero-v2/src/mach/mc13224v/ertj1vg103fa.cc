// EPOS-- ERTJ1VG103FA Termistor Mediator Implementation

#include <mach/mc13224v/ertj1vg103fa.h>
#include <battery.h>
#include <utility/math.h>

__USING_SYS

ERTJ1VG103FA::ERTJ1VG103FA(unsigned int unit)
 : _adc(ADC::SINGLE_ENDED_ADC0)
{
    _adc.get(); // discard first read
}

float ERTJ1VG103FA::convert_temperature(int value) {
    float log_R = Math::logf(resistance(value));
    float den = (A + B*log_R + C*log_R*log_R*log_R);
    return (1.0f / den) + ERROR;
}

float ERTJ1VG103FA::resistance(int read) {
    return (((float)adc_max_value / (float)(adc_max_value - read)) - 1.0f) * (float)div_resistor;
}
