// EPOS AT90CAN128_CAN Initialization


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
