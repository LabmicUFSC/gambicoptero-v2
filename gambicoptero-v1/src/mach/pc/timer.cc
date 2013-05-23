// EPOS PC Timer Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>
#include <timer.h>

__BEGIN_SYS

// Class attributes
PC_Timer * PC_Timer::_channels[CHANNELS];

// Class methods
void PC_Timer::int_handler(unsigned int i)
{
    if((Machine::cpu_id() == 0) && _channels[ALARM])
	_channels[ALARM]->_handler();

    if(_channels[SCHEDULER] && (--_channels[SCHEDULER]->_current <= 0)) {
	_channels[SCHEDULER]->_current = _channels[SCHEDULER]->_initial;
	_channels[SCHEDULER]->_handler();
    }
}

__END_SYS
