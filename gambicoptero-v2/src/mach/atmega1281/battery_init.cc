#include <mach/atmega1281/battery.h>
#include <system/kmalloc.h>

__BEGIN_SYS

void ATMega1281_Battery::init()
{
    system_battery = new(kmalloc(sizeof(ATMega1281_Battery))) ATMega1281_Battery();
}

__END_SYS
