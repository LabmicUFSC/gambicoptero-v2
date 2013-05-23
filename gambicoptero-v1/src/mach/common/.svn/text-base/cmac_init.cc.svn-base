// EPOS CMAC Init

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>

#ifdef __cmac_h

__BEGIN_SYS

template<> CMAC<Radio_Wrapper>::event_handler* CMAC<Radio_Wrapper>::alarm_ev_handler = 0;
template<> volatile unsigned long CMAC<Radio_Wrapper>::alarm_ticks_ms = 0;
template<> unsigned long CMAC<Radio_Wrapper>::alarm_event_time_ms     = 0;

template<> Timer_CMAC * CMAC<Radio_Wrapper>::_timer = 0;

template<> unsigned int CMAC<Radio_Wrapper>::_sleeping_period = Traits<CMAC<Radio_Wrapper> >::SLEEPING_PERIOD;

template<>
void CMAC<Radio_Wrapper>::init(unsigned int n) {
    Radio_Wrapper::init();

    if (Traits<CMAC<Radio_Wrapper> >::time_triggered) {
        db<CMAC<Radio_Wrapper> >(TRC) << "CMAC::init - creating state machine alarm\n";

        alarm_activate(&(state_machine_handler), _sleeping_period);
    }

    _timer->frequency(500);
    _timer->reset();
    _timer->enable();

    Pseudo_Random::seed(Traits<CMAC<Radio_Wrapper> >::ADDRESS); // for CSMA - should be on its own init
}

 __END_SYS

#endif
