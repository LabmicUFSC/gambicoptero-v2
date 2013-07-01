#ifndef __atmega1281_battery_h
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
