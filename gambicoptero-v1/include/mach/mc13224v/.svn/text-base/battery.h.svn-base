// EPOS-- MC13224V Buck Regulator Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mc13224v_battery_h
#define __mc13224v_battery_h

#include <battery.h>
#include <adc.h>

__BEGIN_SYS

class MC13224V_Battery : public Battery_Common
{
protected:
    // voltages in millivolts (mV):
    static const unsigned short battery_reference = 1200;
    static const unsigned short hysteresis = 100;
    // counts in the adc:
    static const unsigned short ADC_3p3v = 1489;
    static const unsigned short ADC_1p8v = 2730;
    static const unsigned short adc_max = 4095;
    static const unsigned short buck_disable_adc_threshold = 1966;
    static const unsigned short buck_enable_adc_threshold = 1890;

    static const unsigned int frequency = Traits<MC13224V_Battery>::frequency;

public:
    MC13224V_Battery();

    static void init();

    const unsigned short get();
    const unsigned short sample();
    const unsigned short charge();

    static MC13224V_Battery & sys_batt();

private:
    static const unsigned short read_to_voltage(unsigned short value);
    static const unsigned short read_to_charge(unsigned short value);

    void check_buck();
    void check_buck(unsigned short sample);

    static const void battery_over_threshold_set(unsigned short value);
    static const void battery_over_threshold_enable();
    static const void battery_over_threshold_disable();
    static const void battery_under_threshold_set(unsigned short value);
    static const void battery_under_threshold_enable();
    static const void battery_under_threshold_disable();

    MC13224V_Battery_ADC _adc;
    static MC13224V_Battery * system_battery;
};

__END_SYS

#endif
