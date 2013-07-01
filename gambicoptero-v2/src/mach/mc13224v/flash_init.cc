// EPOS-- MC13224V Flash Mediator Initialization

#include <system/config.h>
#include <mach/mc13224v/flash.h>

__BEGIN_SYS

void MC13224V_Flash::init()
{
    db<Init, MC13224V_Flash>(TRC) << "MC13224V_Flash::init()\n";

    power(FULL);

    _error = nvm_detect(_interface, &_type);
    if(_error != 0) {
        db<Init, MC13224V_Flash>(ERR) << "MC13224V_Flash: Failed to detect NVM! (error = " << _error << ")\n\r";
    }
    else if(_type == Flash::gNvmType_NoNvm_c) {
        db<Init, MC13224V_Flash>(WRN) << "MC13224V_Flash: No NVM detected!\n\r";
    }
    else {
        db<Init, MC13224V_Flash>(INF) << "MC13224V_Flash: Found a memory type " << _type << "\n\r";
        if(_type == Flash::gNvmType_SST_c) _block_size = 4 * 1024;
        else _block_size = 32 * 1024;
    }

    //TODO: realy needed?
    unlock_last_page();
}

__END_SYS
