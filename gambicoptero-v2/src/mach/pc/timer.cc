// EPOS PC Timer Mediator Implementation

#include <machine.h>
#include <timer.h>

__BEGIN_SYS

// Class attributes
PC_Timer * PC_Timer::_channels[CHANNELS];

// Class methods
void PC_Timer::int_handler(unsigned int i)
{
    if(Traits<Thread>::smp) {
      if(_channels[ALARM])
        _channels[ALARM]->_handler();
    } else {
      if((Machine::cpu_id() == 0) && _channels[ALARM]) {     	
		_channels[ALARM]->_handler();
	  }
	}
   
    if(_channels[SCHEDULER] && (--_channels[SCHEDULER]->_current[Machine::cpu_id()] <= 0)) {
	_channels[SCHEDULER]->_current[Machine::cpu_id()] = _channels[SCHEDULER]->_initial;
	_channels[SCHEDULER]->_handler();
    }
}

__END_SYS
