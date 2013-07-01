// EPOS-- MC13224V Mediator Initialization

#include <mach/mc13224v/machine.h>
#include <mach/mc13224v/battery.h>

__BEGIN_SYS

void MC13224V::init()
{
    db<Init, MC13224V>(TRC) << "MC13224V::init()\n";

    if (Traits<MC13224V_Flash>::enabled || Traits<MC13224V>::flash_erase_checking)
        MC13224V_Flash::init();

    if (Traits<MC13224V>::flash_erase_checking)
        check_flash_erase();

    if (Traits<MC13224V_Battery>::enabled)
        MC13224V_Battery::init();

    if (Traits<MC13224V_NIC>::enabled)
		MC13224V_NIC::init();

}

__END_SYS
