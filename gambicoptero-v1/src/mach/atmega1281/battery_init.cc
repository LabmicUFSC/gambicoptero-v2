#include <mach/atmega1281/battery.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <system/kmalloc.h>

__BEGIN_SYS

void ATMega1281_Battery::init()
{
    system_battery = new(kmalloc(sizeof(ATMega1281_Battery))) ATMega1281_Battery();
}

__END_SYS
