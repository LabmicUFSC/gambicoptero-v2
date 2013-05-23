#ifndef __atmega1281_battery_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __atmega1281_battery_h

#include <battery.h>
#include <adc.h>

__BEGIN_SYS

class ATMega1281_Battery : public Battery_Common
{
public:
    ATMega1281_Battery() {}

    static void init();

    const unsigned short get() { return 0; }
    const unsigned short sample() { return 0; }
    const unsigned short charge() { return 0; }

    static ATMega1281_Battery & sys_batt() { return *system_battery; }

private:
    static ATMega1281_Battery * system_battery;
};

__END_SYS

#endif
