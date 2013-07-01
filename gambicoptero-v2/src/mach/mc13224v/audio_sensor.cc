// EPOS-- Audio Mediator Implementation

#include <mach/mc13224v/audio_sensor.h>

__USING_SYS

Audio_Sensor::Audio_Sensor(unsigned int unit)
    : _adc(ADC::SINGLE_ENDED_ADC3)
{
    _adc.get(); // discard first read
    for (volatile unsigned long i = 0; i <= 50 * (Traits<Machine>::CLOCK / 1000UL); i++); //delay
}
