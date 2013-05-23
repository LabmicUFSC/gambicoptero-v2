// EPOS-- MC13224V Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/mc13224v/battery.h>
#include <mach/mc13224v/buck_regulator.h>
#include <system/kmalloc.h>

__BEGIN_SYS

void MC13224V_Battery::init()
{
    db<Init, MC13224V_Battery>(TRC) << "MC13224V_Battery::init()\n";

    system_battery = new(kmalloc(sizeof(MC13224V_Battery))) MC13224V_Battery();

    if (Traits<MC13224V_Battery>::buck_enabled)
    {
        MC13224V_Buck_Regulator::enable();
        system_battery->check_buck();
    }
}

__END_SYS
