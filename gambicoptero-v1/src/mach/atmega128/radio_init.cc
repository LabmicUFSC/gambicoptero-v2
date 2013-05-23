// EPOS ATMega128 Radio (CC1000) NIC Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <mach/atmega128/machine.h>
#include <mach/atmega128/radio.h>

__BEGIN_SYS

void ATMega128_Radio::init(unsigned int unit)
{
    db<Init, ATMega128_Radio>(TRC) << "ATMega128_Radio::init(unit=" << unit << ")\n";
}

__END_SYS
