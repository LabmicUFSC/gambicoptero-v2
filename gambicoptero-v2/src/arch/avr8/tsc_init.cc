// EPOS AVR8 TSC Mediator Initialization

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
