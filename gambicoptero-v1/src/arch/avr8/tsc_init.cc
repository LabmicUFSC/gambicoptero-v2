// EPOS AVR8 TSC Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <tsc.h>
#include <machine.h>
#include <ic.h>

__BEGIN_SYS

void AVR8_TSC::init()
{
    db<Init, AVR8_TSC>(TRC) << "AVR8_TSC::init()\n";

    tccr1b(CS12 | CS10);

    _ts = 0;
   
    enable();
    
}

__END_SYS
