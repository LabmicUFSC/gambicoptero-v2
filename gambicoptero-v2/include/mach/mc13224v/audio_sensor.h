// EPOS Audio Sensor Mediator Declarations

#ifndef __audio_h
#define __audio_h

#include <adc.h>

__BEGIN_SYS

class Audio_Sensor
{
public:
    Audio_Sensor(unsigned int unit = 0);
    ~Audio_Sensor() {}

    int sample() { return _adc.get(); }

private:
    ADC _adc;
};

__END_SYS

#endif

