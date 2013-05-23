// EPOS-- MC13224V Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/mc13224v/machine.h>

__BEGIN_SYS

void MC13224V::check_flash_erase() {

    // Check if GPIO_11 (EPOSMote-Startup button 1) is set (button is pressed)
    CPU::out32(IO::GPIO_BASE, CPU::in32(IO::GPIO_BASE) & ~(1 << 11)); // is input
    CPU::out32(IO::GPIO_DATA_SEL0, CPU::in32(IO::GPIO_DATA_SEL0) &  ~(1 << 11)); // input from the pad
    CPU::out32(IO::GPIO_FUNC_SEL0, CPU::in32(IO::GPIO_FUNC_SEL0) & ~(0x3 << (11*2))); // all as GPIO

    if(CPU::in32(IO::GPIO_DATA0) & (1 << 11)) // read pin state
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
