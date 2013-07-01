// EPOS-- MC13224V Mediator Implementation

#include <mach/mc13224v/machine.h>
#include <mach/mc13224v/gpio_pin.h>

__BEGIN_SYS

void MC13224V::check_flash_erase() {

    GPIO_Pin pin(Traits<Machine>::flash_erase_checking_pin);

    if(pin.get()) // read pin state
    {
        unbrick();
    }
}

void MC13224V::unbrick() {
    OStream cout;

    cout << "This will reset the processor to original firmware.\n\r";

    MC13224V_Buck_Regulator::NVM_1P8V_enable();

    MC13224V_Flash::nvmErr_t err;
    MC13224V_Flash::nvmType_t type = MC13224V_Flash::gNvmType_NoNvm_c;
    err = MC13224V_Flash::nvm_detect(MC13224V_Flash::gNvmInternalInterface_c, &type);
    if(err != 0) {
        cout << "Failed to detect NVM! (error = " << err << ")\n\r";
    }
    else if(type == Flash::gNvmType_NoNvm_c) {
        cout << "No NVM detected!\n\r";
    }
    else {
        cout << "Found a memory type " << type << ". Trying to erase...\n\r";

        err = Flash::nvm_erase(Flash::gNvmInternalInterface_c, type, 0x1);

        if(err == 0) {
            cout << "Memory reset complete!\n\r";
        }
        else {
            cout << "Oops... we found an error (code = " << err << ")\n\r";
        }
    }

    while (true);
}

__END_SYS
