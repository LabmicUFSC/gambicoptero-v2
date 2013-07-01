// EPOS-- MC13224V Flash Mediator Declarations

#include <machine.h>
#include <mach/mc13224v/flash.h>

__BEGIN_SYS

bool MC13224V_Flash::read(const Address & a, char * d, unsigned int s)
{
    db<MC13224V_Flash>(TRC) << "MC13224V_Flash::read( " << a << ", " << d << ", " << s << ")\n";

    bool status = true;

    power(FULL);

    if((_type != gNvmType_SST_c) && (a >= DATA_BASE))
        unlock_last_page();

    _error = nvm_read(_interface, _type, d, a, s);
    if(_error != 0)
    {
        db<MC13224V_Flash>(ERR) << "MC13224V_Flash: Failed to read from NVM! (error = " << _error << ")\n\r";
        status = false;
    }

    if((_type != gNvmType_SST_c) && (a >= DATA_BASE))
        lock_last_page();
    power(OFF);
    return status;
}

int MC13224V_Flash::write(Address a, char * d, unsigned int s)
{
    db<MC13224V_Flash>(TRC) << "MC13224V_Flash::write( " << a << ", " << d << ", " << s << ")\n";

    bool status = true;

    power(FULL);

    if((_type != gNvmType_SST_c) && (a >= DATA_BASE))
        unlock_last_page();

    _error = nvm_write(_interface, _type, d, a, s);
    if(_error != 0)
    {
        db<MC13224V_Flash>(ERR) << "MC13224V_Flash: Failed to write to NVM! (error = " << _error << ")\n\r";
        status = false;
    }

    if((_type != gNvmType_SST_c) && (a >= DATA_BASE))
        lock_last_page();
    power(OFF);
    return status;
}

bool MC13224V_Flash::backup_reserved()
{
    bool status = true;

    _reserved = (char*)kmalloc(RESERVED_SIZE);
    if(!_reserved)
    {
        db<MC13224V_Flash>(ERR) << "MC13224V_Flash: Failed to backup reserved memory! (Out of Memory)\n\r";
        status = false;
    }
    else
    {
        if(!read(RESERVED_BASE, _reserved, RESERVED_SIZE))
        {
            db<MC13224V_Flash>(ERR) << "MC13224V_Flash: Failed to backup reserved memory!\n\r";
            status = false;
        }
    }

    if((status == false) && _reserved) {
        kfree(_reserved);
        _reserved = 0;
    }
    return status;
}

bool MC13224V_Flash::restore_reserved()
{
    bool status = true;

    if(!_reserved)
    {
        db<MC13224V_Flash>(ERR) << "MC13224V_Flash: Failed to restore reserved memory! (no backup memory)\n\r";
        status = false;
    }
    else
    {
        if(!write(RESERVED_BASE, _reserved, RESERVED_SIZE))
        {
            db<MC13224V_Flash>(ERR) << "MC13224V_Flash: Failed to restore reserved memory!\n\r";
            status = false;
        }
    }

    if(_reserved) {
        kfree(_reserved);
        _reserved = 0;
    }
    return status;
}

bool MC13224V_Flash::erase(Address sector)
{
    db<MC13224V_Flash>(TRC) << "MC13224V_Flash::erase( " << sector << ")\n";

    bool status = true;

    if(sector < 0)
    {
        db<MC13224V_Flash>(WRN) << "MC13224V_Flash: invalid sector number!\n";
        status = false;
    }

    if(_type != gNvmType_SST_c)
    {
        if(!backup_reserved())
            status = false;
    }

    power(FULL);

    if((_type != gNvmType_SST_c) && (sector == 4))
        unlock_last_page();

    unsigned int sector_map = 1 << sector;

    _error = nvm_erase(_interface, _type, sector_map);
    if(_error != 0)
    {
        db<MC13224V_Flash>(ERR) << "MC13224V_Flash: Failed to erase NVM! (error = " << _error << ")\n\r";
        status = false;
    }

    if(_type != gNvmType_SST_c)
    {
        if(!restore_reserved())
            status = false;
    }

    if((_type != gNvmType_SST_c) && (sector == 4))
        lock_last_page();

    power(OFF);

    return status;
}

bool MC13224V_Flash::erase_all()
{
    db<MC13224V_Flash>(TRC) << "MC13224V_Flash::erase_all()\n";

    bool status = true;

    if(!backup_reserved())
        status = false;
    else
    {
        power(FULL);

        unlock_last_page();

        unsigned int sector_map = 0xffffffff;

        _error = nvm_erase(_interface, _type, sector_map);
        if(_error != 0)
        {
            db<MC13224V_Flash>(ERR) << "MC13224V_Flash: Failed to erase NVM! (error = " << _error << ")\n\r";
            status = false;
        }

        if(!restore_reserved())
            status = false;

        lock_last_page();

        power(OFF);
    }

    return status;
}

void MC13224V_Flash::power(MC13224V_Flash::OP_Mode mode)
{
    if(mode == OFF)
        MC13224V_Buck_Regulator::NVM_1P8V_disable();
    else
        MC13224V_Buck_Regulator::NVM_1P8V_enable();
}

MC13224V_Flash::nvmInterface_t MC13224V_Flash::_interface =
        MC13224V_Flash::gNvmInternalInterface_c;
MC13224V_Flash::nvmType_t MC13224V_Flash::_type =
        MC13224V_Flash::gNvmType_NoNvm_c;
MC13224V_Flash::nvmErr_t MC13224V_Flash::_error =
        MC13224V_Flash::gNvmErrNoError_c;
char * MC13224V_Flash::_reserved = 0;
unsigned int MC13224V_Flash::_block_size = 0;

MC13224V_Flash::nvm_detect_func MC13224V_Flash::nvm_detect =
        reinterpret_cast<nvm_detect_func>(reinterpret_cast<void *>(NVM_DETECT_ADDR));

MC13224V_Flash::nvm_erase_func MC13224V_Flash::nvm_erase =
        reinterpret_cast<nvm_erase_func>(reinterpret_cast<void *>(NVM_ERASE_ADDR));

MC13224V_Flash::nvm_read_func MC13224V_Flash::nvm_read =
        reinterpret_cast<nvm_read_func>(reinterpret_cast<void *>(NVM_READ_ADDR));

MC13224V_Flash::nvm_write_func MC13224V_Flash::nvm_write =
        reinterpret_cast<nvm_read_func>(reinterpret_cast<void *>(NVM_WRITE_ADDR));

MC13224V_Flash::nvm_setsvar_func MC13224V_Flash::nvm_setsvar =
        reinterpret_cast<nvm_setsvar_func>(reinterpret_cast<void *>(NVM_SETSVAR_ADDR));





//
//
//MC13224V_Flash::crm_set_spif_voltage_level_func MC13224V_Flash::CRM_SetSPIFVoltageLevel = reinterpret_cast<crm_set_spif_voltage_level_func>(reinterpret_cast<void *>(CRM_SET_SPIF_VOLTAGE_LEVEL_ADDR));

__END_SYS

