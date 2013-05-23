// EPOS-- MC13224V Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/mc13224v/machine.h>
#include <mach/mc13224v/battery.h>

__BEGIN_SYS

void MC13224V::init()
{
    db<Init, MC13224V>(TRC) << "MC13224V::init()\n";

    if (Traits<MC13224V>::flash_erase_checking)
        check_flash_erase();

    if (Traits<MC13224V_Battery>::enabled)
        MC13224V_Battery::init();

    if (Traits<MC13224V_NIC>::enabled)
		MC13224V_NIC::init();
}

__END_SYS

