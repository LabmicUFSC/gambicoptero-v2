// EPOS ERTJ1VG103FA Sensor Mediator Declarations

#ifndef __ertj1vg103fa_h
#define __ertj1vg103fa_h

#include <adc.h>

__BEGIN_SYS

class ERTJ1VG103FA {
protected:
    static const float div_resistor = 20000; // 20 kohm
    static const float adc_max_value = (1 << Traits<ADC>::RESOLUTION) - 1;
    static const float to_celcius_factor = Traits<Temperature_Sensor>::UNITS * 273.15;
    static const float ERROR = Traits<Temperature_Sensor>::ERROR;

    enum {
        KELVIN = 0,
        CELCIUS
    };

    static const float A = 0.0010291947674225f;
    static const float B = 0.000239127518397795f;
    static const float C = 0.000000156627714973031f;

public:
    ERTJ1VG103FA(unsigned int unit = 0);
    ~ERTJ1VG103FA() {}

    int get() { return resistance(_adc.get()); }

    int sample() {
    	return convert_temperature(_adc.get()) - to_celcius_factor;
    }

    bool enable() { return _adc.enable();}
    void disable() { _adc.disable(); }
    bool data_ready() { return _adc.finished(); }

private:
    float convert_temperature(int value);
    float resistance(int read);

    ADC _adc;
};

__END_SYS

#endif
