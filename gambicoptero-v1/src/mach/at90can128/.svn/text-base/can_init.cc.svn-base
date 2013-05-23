// EPOS AT90CAN128_CAN Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.


#include <mach/at90can128/can.h>

__BEGIN_SYS

// Class initialization
int AT90CAN128_CAN::init()
{
    db<AT90CAN128_CAN>(TRC) << "AT90CAN128_CAN::init()\n";
    for( int i = 0; i < _Traits::MAX_MOBS; i++ )
	mobs[i].mob_index(i);
   
    return 0;
}

__END_SYS
