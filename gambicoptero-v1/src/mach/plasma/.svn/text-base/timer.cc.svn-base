// EPOS PLASMA Timer Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>
#include <timer.h>

__BEGIN_SYS

// Class attributes
//PLASMA_Timer::Count PLASMA_Timer::_count = (1<<18); //fixed by hardware
PLASMA_Timer * PLASMA_Timer::_channels[CHANNELS];

// Class methods
void PLASMA_Timer::int_handler(unsigned int i) 
{
	db<PLASMA_Timer>(TRC) << "<Timer::int_handler>";
	
	static bool cnt_lo = false;
  	// Gambiarra do Timer ...
  	if(cnt_lo){
		IC::disable(2);
		IC::enable(3);
		cnt_lo = false;
  	} else {
		IC::disable(3);
		IC::enable(2);
      	cnt_lo = true;
  	}
		
	//Always calls ALARM Channel - As Alarm frequency is equal to Timer Frequency, count is always 1, 
	if(_channels[ALARM])
		_channels[ALARM]->_handler();
	
	// Left SCHEDULER as last, as it can change running context...
	if(_channels[TEST] && (--_channels[TEST]->_current <= 0)){
		_channels[TEST]->_current = _channels[TEST]->_initial;
		_channels[TEST]->_handler();
	}	
		
	//Check if SCHEDULER channel must be handled.
	if(_channels[SCHEDULER] && (--_channels[SCHEDULER]->_current <= 0)){
		_channels[SCHEDULER]->_current = _channels[SCHEDULER]->_initial;
		_channels[SCHEDULER]->_handler();
	}
	
}

__END_SYS
