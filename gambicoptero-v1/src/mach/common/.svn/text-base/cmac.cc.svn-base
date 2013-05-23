// EPOS CMAC Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>
#include <semaphore.h>

#ifdef __cmac_h

__BEGIN_SYS

template<> 
CMAC<Radio_Wrapper>::CMAC_STATE_TRANSITION CMAC<Radio_Wrapper>::state_machine() {
    CMAC_STATE_TRANSITION result = TIMER_INT;
    _state = ACTIVE;

    db<CMAC>(TRC) << "CMAC::state_machine - starting state machine\n";

    while (_state != OFF) {
        switch (_state) {
            case SYNCHRONOUS_SYNC:
                result = Synchronous_Sync<Radio_Wrapper>::execute(result);

                if (result == TIMEOUT)
                    _state = OFF;
                else
                    _state = ACTIVE;

                break;

            case ACTIVE:
                result = Generic_Active<Radio_Wrapper>::execute(result);

                if (result == SYNC_END)
                    _state = OFF;
                else 
                    _state = ASYNCHRONOUS_SYNC;

                break;

            case ASYNCHRONOUS_SYNC:
                result = Asynchronous_Sync<Radio_Wrapper>::execute(result);

                if (result == RX_PENDING)
                    _state = RX_CONTENTION;
                else if (result == TX_PENDING)
                    _state = TX_CONTENTION;
                else if (result == TIMEOUT)
                    _state = OFF;

                break;

            case RX_CONTENTION:
                result = Rx_Contention<Radio_Wrapper>::execute(result);

                if (result == RX_CONTENTION_OK)
                    _state = RX_DATA;
                else if (result == TIMEOUT)
                    _state = OFF;

                break;

            case RX_DATA:
                result = Generic_Rx<Radio_Wrapper>::execute(result);

                if (result == RX_END)
                    _state = UNPACK;
                else if (result == RX_ERROR)
                    _state = OFF;

                break;

            case UNPACK:
                result = IEEE802154_Unpack<Radio_Wrapper>::execute(result);

                if (result == UNPACK_FAILED)
                    _state = ACTIVE;
                else if (result == UNPACK_OK) {
                    if (_tx_dst_address == Radio_Common::BROADCAST) {
                        _state = OFF;
                        result = RX_OK;

                    } else 
                        _state = ACK_TX;
                }

                break;

            case ACK_TX:
                result = IEEE802154_Ack_Tx<Radio_Wrapper>::execute(result);

                if (result == KEEP_ALIVE)
                    _state = RX_DATA;
                else 
                    _state = OFF;

                break;

            case TX_CONTENTION:
                result = Tx_Contention<Radio_Wrapper>::execute(result);

                if (result == TX_CONTENTION_OK)
                    _state = PACK;
                else if (result == TIMEOUT)
                    _state = OFF;

                break;

            case PACK:
                if (Traits<CMAC>::time_triggered)
                    _last_sm_exec_tx = true;

                result = IEEE802154_Pack<Radio_Wrapper>::execute(result);

                if (result == PACK_OK)
                    _state = TX_DATA;
                else if (result == PACK_FAILED)
                    _state = OFF;

                break;

            case TX_DATA:
                result = Generic_Tx<Radio_Wrapper>::execute(result);

                if (result == TX_END) {
                    if (_tx_dst_address == Radio_Common::BROADCAST) {
                        _state = OFF;
                        result = TX_OK;

                    } else 
                        _state = ACK_RX;

                } else if (result == TX_ERROR)
                    _state = OFF;

                break;

            case ACK_RX:
                result = IEEE802154_Ack_Rx<Radio_Wrapper>::execute(result);

                if ((result == TX_PENDING) || (result == KEEP_ALIVE))
                    _state = PACK;
                else
                    _state = OFF;

                break;

            case OFF:
                break;
        }
    }

    db<CMAC>(TRC) << "CMAC::state_machine - state machine finished executing\n";
    return result;
}

template<>
void CMAC<Radio_Wrapper>::tx_p() {
    _sem_tx->p();
}

template<>
void CMAC<Radio_Wrapper>::rx_p() {
    _sem_rx->p();
}

template<>
void CMAC<Radio_Wrapper>::state_machine_handler() {
    if (Traits<CMAC<Radio_Wrapper> >::time_triggered) {
        _on_active_cycle = true;

        _state_machine_result = state_machine();

        // kills the rest of the active cycle
        if (Traits<CMAC<Radio_Wrapper> >::TIMEOUT != 0) {
            while (!timeout);
        }

        alarm_activate(&(CMAC<Radio_Wrapper>::state_machine_handler), _sleeping_period);

        if (_last_sm_exec_tx) {
            _last_sm_exec_tx = false;
            _sem_tx->v();

        } else if (_last_sm_exec_rx) {
            _last_sm_exec_rx = false;
            _sem_rx->v();
        }
    }
}

template<>
void CMAC<Radio_Wrapper>::alarm_handler_function() {
    CPU::int_disable();

    // increment in 2 ms //see cmac_init
    alarm_ticks_ms += 2;

    CPU::int_enable();

    if ((alarm_ev_handler != 0) && (alarm_ticks_ms >= alarm_event_time_ms)) {
        db<CMAC>(INF) << "CMAC::alarm_handler_function - calling alarm_ev_handler\n";
        event_handler *tmp = alarm_ev_handler;
        alarm_ev_handler = 0;
        (tmp());
    }
}

template<> Radio_Wrapper * CMAC<Radio_Wrapper>::_radio = 0;

template<> CMAC<Radio_Wrapper>::Address * CMAC<Radio_Wrapper>::_addr = 0;

template<> volatile CMAC<Radio_Wrapper>::CMAC_STATE CMAC<Radio_Wrapper>::_state = CMAC<Radio_Wrapper>::OFF;
template<> volatile CMAC<Radio_Wrapper>::Statistics * CMAC<Radio_Wrapper>::_stats = 0;

// used only when TIME_TRIGGERED = true
template<> Semaphore * CMAC<Radio_Wrapper>::_sem_rx = 0;
template<> Semaphore * CMAC<Radio_Wrapper>::_sem_tx = 0;
template<> CMAC<Radio_Wrapper>::CMAC_STATE_TRANSITION CMAC<Radio_Wrapper>::_state_machine_result = CMAC<Radio_Wrapper>::UNPACK_FAILED;
template<> volatile bool CMAC<Radio_Wrapper>::_last_sm_exec_tx = false;
template<> volatile bool CMAC<Radio_Wrapper>::_last_sm_exec_rx = false;
template<> volatile bool CMAC<Radio_Wrapper>::_on_active_cycle = false;

template<> unsigned char CMAC<Radio_Wrapper>::_frame_buffer[FRAME_BUFFER_SIZE] = {};
template<> unsigned int CMAC<Radio_Wrapper>::_frame_buffer_size = 0;

template<> void* CMAC<Radio_Wrapper>::_rx_data = 0;
template<> unsigned int CMAC<Radio_Wrapper>::_rx_data_size = 0;
template<> CMAC<Radio_Wrapper>::Address CMAC<Radio_Wrapper>::_rx_src_address = 0;
template<> volatile bool CMAC<Radio_Wrapper>::_rx_pending = false;
template<> CMAC<Radio_Wrapper>::Protocol CMAC<Radio_Wrapper>::_rx_protocol = 0;

template<> const void* CMAC<Radio_Wrapper>::_tx_data = 0;
template<> unsigned int CMAC<Radio_Wrapper>::_tx_data_size = 0;
template<> CMAC<Radio_Wrapper>::Address CMAC<Radio_Wrapper>::_tx_dst_address = 0;
template<> volatile bool CMAC<Radio_Wrapper>::_tx_pending = false;
template<> CMAC<Radio_Wrapper>::Protocol CMAC<Radio_Wrapper>::_tx_protocol = 0;

template<> int CMAC<Radio_Wrapper>::_transmission_count = 0;

template<> unsigned char CMAC<Radio_Wrapper>::_data_sequence_number = 0;

// CMAC states static variables
template<> volatile bool CMAC<Radio_Wrapper>::timeout = false;
template<> volatile bool Generic_Lpl<Radio_Wrapper>::_frame_received = false;

// IEEE802.15.4 specific variables
template<> unsigned char IEEE802154_Beacon_Sync<Radio_Wrapper>::_beacon_order      = IEEE802154_Beacon_Sync<Radio_Wrapper>::MAX_BEACON_ORDER;
template<> unsigned char IEEE802154_Beacon_Sync<Radio_Wrapper>::_superframe_order  = IEEE802154_Beacon_Sync<Radio_Wrapper>::MAX_SUPERFRAME_ORDER;
template<> unsigned char IEEE802154_Beacon_Sync<Radio_Wrapper>::_beacon_sequence_n = 0;

template<> int CMAC<Radio_Wrapper>::_consecutive_failures = 0;

__END_SYS

#endif
