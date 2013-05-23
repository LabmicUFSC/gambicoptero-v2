// EPOS Alarm Abstraction Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <alarm.h>

__BEGIN_SYS

int Alarm::init()
{
    db<Init, Alarm>(TRC) << "Alarm::init()\n";
    
    _timer = new (kmalloc(sizeof(Alarm_Timer))) Alarm_Timer(&handler);

    return 0;
}

__END_SYS
