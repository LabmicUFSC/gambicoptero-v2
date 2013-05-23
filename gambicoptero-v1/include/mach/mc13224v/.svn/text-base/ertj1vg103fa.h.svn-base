// EPOS ERTJ1VG103FA Sensor Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __ertj1vg103fa_h
#define __ertj1vg103fa_h

#include <adc.h>

__BEGIN_SYS

class ERTJ1VG103FA {
protected:
    static const int div_resistor = 10000; // 10 kohm
    static const int adc_max_value = 4095; // 11 bits ADC
    static const int temperature_unit = Traits<Temperature_Sensor>::UNITS;

    enum {
        KELVIN = 0,
        CELCIUS
    };

    static const float A = 0.0010750492;
    static const float B = 0.00027028218;
    static const float C = 0.00000014524838;
    static const float Correction = 10.37;
    static const float Kelvin_to_Celsius = 273.15;

public:
    ERTJ1VG103FA(unsigned int unit = 0);
    ~ERTJ1VG103FA() {}

    int get() { return resistance(_adc.get()); }

    int sample() { return convert_temperature(_adc.get()); }

    bool enable() { return true;}
    void disable() {}
    bool data_ready() { return _adc.finished(); }

private:
    float convert_temperature(int value);
    int voltage(int read);
    float resistance(int read);

    ADC _adc;
};

__END_SYS

#endif

