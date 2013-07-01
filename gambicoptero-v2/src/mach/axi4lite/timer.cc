// EPOS AXI4LITE Timer Mediator Implementation

#include <machine.h>
#include <timer.h>

__BEGIN_SYS

// Class attributes
AXI4LITE_Timer::Timer_Reg * AXI4LITE_Timer::_timer_reg = reinterpret_cast<AXI4LITE_Timer::Timer_Reg *>(Traits<AXI4LITE_Timer>::BASE_ADDRESS);
AXI4LITE_Timer * AXI4LITE_Timer::_channels[CHANNELS];

// Class methods
void AXI4LITE_Timer::int_handler(unsigned int i) 
{
	db<AXI4LITE_Timer>(TRC) << "<Timer::int_handler>";
	
	//Reprogram the Timer - NOT required, timer is now periodic
	//*_timer_reg = *_timer_reg + (CLOCK / FREQUENCY);
			
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
