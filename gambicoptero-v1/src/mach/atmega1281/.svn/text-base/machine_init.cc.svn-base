// EPOS ATMega1281 Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>
#include <system.h>

extern "C" { void __epos_library_app_entry(void); }

__BEGIN_SYS

void ATMega1281::init()
{
    db<Init, ATMega1281>(TRC) << "ATMega1281::init()\n";

    System::info()->lmm.app_entry =
	reinterpret_cast<unsigned int>(&__epos_library_app_entry);

    IC::disable();

    AT86RF230::init();

    // Initialize the hardware
    if(Traits<ATMega1281_NIC>::enabled)
	ATMega1281_NIC::init();

    return;
}

__END_SYS

