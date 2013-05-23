#ifndef __cmac_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __cmac_h

#include <utility/observer.h>
#include <utility/random.h>
#include <system/kmalloc.h>
#include <battery.h>
#include <neighboring.h>
#include <radio.h>
#include <timer.h>

__BEGIN_SYS

template <typename T>
class CMAC: public NIC_Common::Observed {
public:
    enum CMAC_STATE {
        OFF,
        SYNCHRONOUS_SYNC,
        ACTIVE,
        ASYNCHRONOUS_SYNC,
        RX_CONTENTION, RX_DATA, UNPACK, ACK_TX,
        TX_CONTENTION, PACK, TX_DATA, ACK_RX,
    };

    enum CMAC_STATE_TRANSITION {
        SYNC_END, TIMER_INT, KEEP_ALIVE,
        /* send */
        TX_CONTENTION_OK, TX_PENDING, PACK_OK, PACK_FAILED, CHANNEL_BUSY, CHANNEL_IDLE, TX_END, TX_ERROR, TX_OK, TX_FAILED,
        /* receive */
        RX_CONTENTION_OK, RX_PENDING, PREAMBLE_DETECTED, TIMEOUT, RX_END, RX_ERROR, UNPACK_OK, UNPACK_FAILED, RX_OK,
    };

    typedef Radio_Common::Address Address;
    typedef Radio_Common::Protocol Protocol;
    typedef Radio_Common::Statistics Statistics;
    static const int FRAME_BUFFER_SIZE = T::FRAME_BUFFER_SIZE;

public:
    CMAC(int unit = 0) {
        _stats = new(kmalloc(sizeof(Statistics))) Statistics();
        _addr  = new(kmalloc(sizeof(Address))) Address(Traits<CMAC<T> >::ADDRESS);
        _radio = new(kmalloc(sizeof(T))) T();
        _timer = new(kmalloc(sizeof(Timer_CMAC))) Timer_CMAC(alarm_handler_function);

        CMAC<T>::init(unit);
    }

    void tx_p();
    void rx_p();

    int send(const Address & dst, const Protocol & prot,
            const void *data, unsigned int size) {
        if (Traits<CMAC<T> >::time_triggered) {
            if (_tx_pending || (size > mtu())) {
                db<CMAC<T> >(WRN) << "CMAC::send - another TX pending or data size > MTU\n";
                return -1;
            }

        } else {
            if (_tx_pending || _rx_pending || (size > mtu())) {
                db<CMAC<T> >(WRN) << "CMAC::send - another TX or RX pending or data size > MTU\n";
                return -1;
            }
        }

        if (size == 0) {
            db<CMAC<T> >(ERR) << "CMAC::send - data size = 0\n";
            return 0;
        }

        CMAC_STATE_TRANSITION result;

        _tx_data 	    = data;
        _tx_data_size   = size;
        _tx_dst_address = dst;
        _tx_pending     = true;
        _tx_protocol    = prot;

        unsigned long start_time = alarm_ticks_ms;

        if (Traits<CMAC<T> >::time_triggered) {
            db<CMAC<T> >(INF) << "CMAC::send - waiting for TX handling\n";
//            _sem_tx->p();
            tx_p();
            result = _state_machine_result;

        } else {
            db<CMAC<T> >(TRC) << "CMAC::send - calling state machine\n";
            result = state_machine();
        }

        if (result == TX_OK) {
            _stats->tx_packets += 1;
            _stats->tx_bytes += size;
        }

        _stats->tx_time += alarm_ticks_ms - start_time;

        _tx_pending = false;

        return result;
    }

    int receive(Address * src, Protocol * prot,
            void * data, unsigned int size) {

        if (Traits<CMAC<T> >::time_triggered) {
            if (_rx_pending || (size > mtu())) {
                db<CMAC<T> >(WRN) << "CMAC::receive - another RX pending or buffer size > MTU\n";
                return -1;
            }

        } else {
            if (_rx_pending || _tx_pending || (size > mtu())) {
                db<CMAC<T> >(WRN) << "CMAC::receive - another RX or TX pending or buffer size > MTU\n";
                return -1;
            }
        }

        CMAC_STATE_TRANSITION result;

        _rx_data 	  = data;
        _rx_data_size = size;
        _rx_pending   = true;

        unsigned long start_time = alarm_ticks_ms;

        if (Traits<CMAC<T> >::time_triggered) {
            db<CMAC<T> >(INF) << "CMAC::receive - waiting for RX handling\n";
//            _sem_rx->p();
            rx_p();
            result = _state_machine_result;

        } else {
            db<CMAC<T> >(TRC) << "CMAC::receive - calling state machine\n";
            result = state_machine();
        }

        _stats->rx_time += alarm_ticks_ms - start_time;

        *src  = _rx_src_address;
        *prot = _rx_protocol;

        _rx_pending = false;

        if (result == RX_OK) {
            _stats->rx_packets += 1;
            _stats->rx_bytes   += _rx_data_size;

            notify((int) *prot);

            return _rx_data_size;

        } else
            return result * (-1);
    } 

    /**
     * For CMAC, we use the following statistics:
     * (all the statistics are related only to data packets)
     *
     * rx_packets 	= number of packets successfully received
     * rx_bytes 	= number of bytes successfully received
     * rx_time 		= time spent receiving packets
     * tx_packets 	= number of packets successfully transmitted
     * tx_bytes 	= number of bytes successfully transmitted
     * tx_time 		= time spent transmitting packets
     * dropped_packets  = number of packets dropped due to errors
     * total_tx_packets = total number of packets transmitted
     */
    const Statistics & statistics() {
        return (const Statistics&) *_stats;
    }

    const Address & address() {
        return (const Address&) *_addr;
    }

    void reset() {}

    void config(int frequency, int power) {}

    unsigned int mtu() const { return Traits<CMAC<T> >::MTU; }

    static void init(unsigned int n);

    static CMAC_STATE_TRANSITION state_machine();

    static void state_machine_handler();

    static unsigned long alarm_time() {
        return alarm_ticks_ms;
    }

protected:
    /* timing control */
    typedef void (event_handler)(void);

    static event_handler *alarm_ev_handler;
    static volatile unsigned long alarm_ticks_ms;
    static unsigned long alarm_event_time_ms;
    static Timer_CMAC * _timer;
    static volatile bool timeout;

    static void alarm_handler_function();

    static void alarm_event_time(unsigned long event_time) {
        alarm_event_time_ms = alarm_ticks_ms + event_time;
    }

    static unsigned long alarm_event_time_left() {
        return alarm_ticks_ms - alarm_event_time_ms;
    }

    static void alarm_busy_delay(unsigned long time) {
        unsigned long delay = alarm_ticks_ms + time;
        while (delay <= alarm_ticks_ms);
    }

    static void alarm_activate(event_handler * handler, unsigned long period) {
        alarm_ev_handler = handler;
        alarm_event_time(period);
    }

    static void alarm_deactivate() {
        alarm_ev_handler = 0;
    }

protected:
    static T * _radio;

    /* timer control variables */
    static unsigned int _sleeping_period; // ms

    static Address           *  _addr;
    static volatile Statistics * _stats;
    static volatile CMAC_STATE _state;

    /* used only when TIME_TRIGGERED = true */
    static Semaphore * _sem_rx;
    static Semaphore * _sem_tx;
    static CMAC_STATE_TRANSITION _state_machine_result;
    static volatile bool _last_sm_exec_tx; // true if the last execution of the state machine executed an TX
    static volatile bool _last_sm_exec_rx; // true if the last execution of the state machine executed an RX
    static volatile bool _on_active_cycle;

    static unsigned char _frame_buffer[FRAME_BUFFER_SIZE];
    static unsigned int _frame_buffer_size;

    static void *        _rx_data;
    static unsigned int  _rx_data_size;
    static Address       _rx_src_address;
    static volatile bool _rx_pending;
    static Protocol      _rx_protocol;

    static const void *  _tx_data;
    static unsigned int  _tx_data_size;
    static Address       _tx_dst_address;
    static volatile bool _tx_pending;
    static Protocol      _tx_protocol;

    static int _transmission_count;

    static unsigned char _data_sequence_number;

    static int _consecutive_failures;

    friend class CMAC_State<T>;
    friend class Empty<T>;
    friend class Sync_Empty<T>;
    friend class Pack_Empty<T>;
    friend class Contention_Empty<T>;
    friend class Tx_Empty<T>;
    friend class Ack_Rx_Empty<T>;
    friend class Lpl_Empty<T>;
    friend class Rx_Empty<T>;
    friend class Unpack_Empty<T>;
    friend class Ack_Tx_Empty<T>;

    friend class Synchronous_Sync<T>;
    friend class Asynchronous_Sync<T>;
    friend class Rx_Contention<T>;
    friend class Tx_Contention<T>;

    friend class Generic_Active<T>;
    friend class Generic_Tx<T>;
    friend class Generic_Rx<T>;
    friend class Generic_Lpl<T>;
    friend class CSMA<T>;

    friend class IEEE802154_Beacon_Sync<T>;
    friend class IEEE802154_Pack<T>;
    friend class IEEE802154_Unpack<T>;
    friend class IEEE802154_Ack_Rx<T>;
    friend class IEEE802154_Ack_Tx<T>;
    friend class IEEE802154_Slotted_CSMA_Contention<T>;
};

/**
 * CMAC states implementation 
 */
template<typename T>
class CMAC_State {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input);
};

/**
 * Empty states implementation
 */
template<typename T>
class Empty: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "Empty - UNPACK_FAILED\n";
        return CMAC<T>::UNPACK_FAILED;
    }
};

template<typename T>
class Sync_Empty: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        if (CMAC<T>::_rx_pending) {
            db<CMAC<T> >(TRC) << "Sync_Empty - RX_PENDING\n";
            return CMAC<T>::RX_PENDING;

        } else if (CMAC<T>::_tx_pending) {
            db<CMAC<T> >(TRC) << "Sync_Empty - TX_PENDING\n";
            return CMAC<T>::TX_PENDING;

        } else {
            db<CMAC<T> >(TRC) << "Sync_Empty - SYNC_END\n";
            return CMAC<T>::SYNC_END;
        }
    }
};

template<typename T>
class Pack_Empty: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "Pack_Empty - PACK_OK\n";
        CMAC<T>::_transmission_count += 1;
        return CMAC<T>::PACK_OK;
    }
};

template<typename T>
class Contention_Empty: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "Contention_Empty - CHANNEL_IDLE\n";
        return CMAC<T>::CHANNEL_IDLE;
    }
};

template<typename T>
class Tx_Empty: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "Tx_Empty - TX_END\n";
        return CMAC<T>::TX_END;
    }
};

template<typename T>
class Ack_Rx_Empty: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "Ack_Rx_Empty - TX_OK\n";
        CMAC<T>::_transmission_count = 0;
        return CMAC<T>::TX_OK;
    }
};

template<typename T>
class Lpl_Empty: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "Lpl_Empty - PREAMBLE_DETECTED\n";
        return CMAC<T>::PREAMBLE_DETECTED;
    }
};

template<typename T>
class Rx_Empty: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "Rx_Empty - RX_END\n";
        char data[16] = "RX_Empty State\n";
        CMAC<T>::_rx_data_size = 16;
        char *aux = reinterpret_cast<char*>(CMAC<T>::_rx_data);
        for (int i = 0; i < 16; ++i) {
            aux[i] = data[i];
        }
        return CMAC<T>::RX_END;
    }
};

template<typename T>
class Unpack_Empty: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "Unpack_Empty - UNPACK_OK\n";
        return CMAC<T>::UNPACK_OK;
    }
};

template<typename T>
class Ack_Tx_Empty: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "Ack_Tx_Empty - RX_OK\n";
        return CMAC<T>::RX_OK;
    }
};

/**
 * Generic states implementation
 */
template<typename T>
class Synchronous_Sync: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        // TODO
        return CMAC<T>::SYNC_END;
    }
};

template<typename T>
class Generic_Active: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "Active - Setting timeout\n";

        CMAC<T>::timeout = false;
        CMAC<T>::alarm_activate(&alarm_handler, Traits<CMAC<T> >::TIMEOUT);

        if (CMAC<T>::_rx_pending) {
            db<CMAC<T> >(TRC) << "Active - RX_PENDING\n";
            return CMAC<T>::RX_PENDING;

        } else if (CMAC<T>::_tx_pending) {
            db<CMAC<T> >(TRC) << "Active - TX_PENDING\n";
            return CMAC<T>::TX_PENDING;

        } else {
            db<CMAC<T> >(TRC) << "Active - SYNC_END\n";
            return CMAC<T>::SYNC_END;
        }
    }

private:
    static void alarm_handler() {
        db<CMAC<T> >(INF) << "Active - timeout\n";
        while (!CMAC<T>::timeout) CMAC<T>::timeout = true; // no excuses now
    }
};

template<typename T>
class Asynchronous_Sync: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    enum STATE {
        BACKOFF,
        LISTEN, 
        TX_PREAMBLE,
        RX_ACK_PREAMBLE,
        RX_PREAMBLE,
        TX_ACK_PREAMBLE,
        TX_CONTENTION,
        RX_CONTENTION,
    };

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        STATE state = BACKOFF;
        CMAC_STATE_TRANSITION result;

        while ((state != TX_CONTENTION) && (state != RX_CONTENTION)) {
            switch (state) {
                case BACKOFF:
                    Backoff<T>::execute(result);

                case LISTEN:
                    if (CMAC<T>::_rx_pending) {
                        result = Generic_Lpl<T>::execute(result, Traits<CMAC<T> >::TIMEOUT);

                        if (result == CMAC<T>::CHANNEL_BUSY)
                            state = RX_PREAMBLE;
                        else
                            return CMAC<T>::TIMEOUT;

                    } else {
                        result = CSMA<T>::execute(result);

                        if (result == CMAC<T>::CHANNEL_BUSY)
                            state = BACKOFF;
                        else
                            state = TX_PREAMBLE;
                    }

                    break;

                case TX_PREAMBLE:
                    state = TX_CONTENTION;
                    break;
                case RX_ACK_PREAMBLE:
                    break;
                case RX_PREAMBLE:
                    state = RX_CONTENTION;
                    break;
                case TX_ACK_PREAMBLE:
                    break;
                case TX_CONTENTION:
                    break;
                case RX_CONTENTION:
                    break;
            }
        }

        return input;
    }
};

template<typename T>
class Backoff: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        if (Traits<CMAC<T> >::BACKOFF > 0)
            execute(input, Traits<CMAC<T> >::BACKOFF);

        return input;
    }

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input, unsigned long ms) {
        for (unsigned long i = 0; i <= ms * (Traits<Machine>::CLOCK / 1000UL); i++);

        return input;
    }
};

template<typename T>
class Rx_Contention: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        if (!Traits<CMAC<T> >::rts_cts)
            return CMAC<T>::RX_CONTENTION_OK;

        // TODO
        return CMAC<T>::RX_CONTENTION_OK;
    }
};

template<typename T>
class Tx_Contention: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        if (!Traits<CMAC<T> >::rts_cts)
            return CMAC<T>::TX_CONTENTION_OK;

        // TODO
        return CMAC<T>::TX_CONTENTION_OK;
    }
};

template<typename T>
class Generic_Tx: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        /**
         * According to the state machine TX should never fail, 
         * but hardware failure may happen.
         */
        int result;

        for (int var = 0; var < 5; ++var) {
            db<CMAC<T> >(TRC) << "Generic_Tx - sending\n";
            result = CMAC<T>::_radio->send(CMAC<T>::_frame_buffer, CMAC<T>::_frame_buffer_size);

            if (result == 0) {
                db<CMAC<T> >(WRN) << "Generic_Tx - Tx failed - Trying again\n";
                CMAC<T>::_radio->off();

            } else
                break;
        }

        if (result) {
            db<CMAC<T> >(TRC) << "Generic_Tx - TX_END\n";
            return CMAC<T>::TX_END;

        } else {
            db<CMAC<T> >(ERR) << "Generic_Tx - TX_ERROR\n";
            return CMAC<T>::TX_ERROR;
        }
    }
};

template<typename T>
class Generic_Rx: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        /**
         * According to the state machine RX should never fail, 
         * but hardware failure may happen.
         */
        int result;

        db<CMAC<T> >(TRC) << "Generic_Rx - receiving\n";
        result = CMAC<T>::_radio->receive(&(CMAC<T>::_frame_buffer[0]));

        if (!(result > 0)) {
            db<CMAC<T> >(ERR) << "Generic_Rx - RX_ERROR\n";
            CMAC<T>::_radio->off();
            return CMAC<T>::RX_ERROR;
        }

        CMAC<T>::_frame_buffer_size = result;

        db<CMAC<T> >(TRC) << "Generic_Rx - RX_END\n";
        return CMAC<T>::RX_END;
    }
};

template<typename T>
class Generic_Lpl: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        return execute(input, 0);
    }

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input, unsigned long soft_timeout) {
        db<CMAC<T> >(TRC) << "Generic_Lpl - Listening\n";

        CMAC<T>::_radio->set_event_handler(&_event_handler);

        _frame_received = false;
        CMAC<T>::_radio->listen();

        bool timeout;
        if (soft_timeout == 0) {
            while (!(timeout = CMAC<T>::timeout) && !_frame_received);

        } else {
            unsigned long timeout2 = CMAC<T>::alarm_ticks_ms + soft_timeout;
            //		while (!(timeout = (CMAC<T>::alarm_ticks_ms >= timeout2) || CMAC<T>::timeout) && !_frame_received);
            while (!(timeout = (CMAC<T>::alarm_ticks_ms >= timeout2)) && !_frame_received);
        }

        db<CMAC<T> >(INF) << "Generic_Lpl::event_handler - FRAME_RECEIVED 2 \n";

        CMAC_STATE_TRANSITION transition;
        if (timeout) {
            db<CMAC<T> >(WRN) << "Generic_Lpl - TIMEOUT\n";
            CMAC<T>::_radio->off();
            transition = CMAC<T>::CHANNEL_IDLE;
        } else {
            db<CMAC<T> >(INF) << "Generic_Lpl - PREAMBLE_DETECTED\n";
            transition = CMAC<T>::CHANNEL_BUSY;
        }

        return transition;
    }

private:
    static void wait(unsigned int us) {
        for (unsigned int i = 0; i <= us * (Traits<Machine>::CLOCK / 1000000UL); i++); 
    }

    static void _event_handler(typename T::Event event) {
        wait(250);
        if (event == T::SFD_DETECTED) {
            CMAC<T>::_radio->off();
            while (!_frame_received) _frame_received = true; // no excuses now
            db<CMAC<T> >(INF) << "Generic_Lpl::event_handler - FRAME_RECEIVED" << _frame_received << "\n";
        }
    }

    static volatile bool _frame_received;
};

template<typename T>
class CSMA: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        if (!Traits<CMAC<T> >::csma)
            return CMAC<T>::CHANNEL_IDLE;

        db<CMAC<T> >(TRC) << "CSMA - Checking channel\n";

        unsigned int nb = 0;
        unsigned int be = MIN_BE;

        while (nb < MAX_BACKOFFS) {
            /* delay = random(2^BE - 1) * Period in ms */
            unsigned long delay = static_cast<unsigned long> ((Pseudo_Random::random() % (2 << (be-1))) * UNIT_BACKOFF_PERIOD);

            db<CMAC<T> >(INF) << "CSMA - Delay = " << delay << "\n";

            /* for some reason the delay can NEVER be 0 */
            if (delay == 0) 
                delay = static_cast<unsigned long>(UNIT_BACKOFF_PERIOD);

            CMAC<T>::alarm_busy_delay(delay);

            /* Clear Channel Assesment(CCA) */
            bool aux = CMAC<T>::_radio->cca();

            if (aux) {
                db<CMAC<T> >(TRC) << "CSMA - CHANNEL_IDLE\n";
                return CMAC<T>::CHANNEL_IDLE;
            }

            nb = nb + 1;
            be = be + 1;
            if (be > MAX_BE)
                be = MAX_BE;
        }

        db<CMAC<T> >(WRN) << "CSMA - CHANNEL_BUSY\n";
        return CMAC<T>::CHANNEL_BUSY;
    }

private:
    enum {
        MIN_BE 		    = 3,
        MAX_BE 		    = 5,
        MAX_BACKOFFS  	    = 8,
        UNIT_BACKOFF_PERIOD = 15 /* ms */
    };
};

/**
 * IEEE 802.15.4 states implementation
 */
template<typename T>
class IEEE802154_Frame: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;
    typedef typename CMAC<T>::Address Address;

    enum {
        FRAME_TYPE_BEACON = 0,
        FRAME_TYPE_DATA = 1,
        FRAME_TYPE_ACK = 2,
        FRAME_TYPE_MAC_COMMAND = 3, //NOT SUPPORTED
        SECURITY_ENABLED_ON = 1, //NOT SUPPORTED
        SECURITY_ENABLED_OFF = 0, //NOT SUPPORTED
        FRAME_PENDING_ON = 1, //NOT SUPPORTED
        FRAME_PENDING_OFF = 0, //NOT SUPPORTED
        ACK_REQUEST_ON = 1, //NOT SUPPORTED
        ACK_REQUEST_OFF = 0, //NOT SUPPORTED
        INTRA_PAN_SAME_PAN = 1,
        INTRA_PAN_OTHER_PAN = 0,//PanIdentifier Field needed - NOT SUPORTED
        ADDRESSING_MODE_PAN_AND_ADDRESS_NOT_PRESENT = 0, //NOT SUPPORTED
        ADDRESSING_MODE_RESERVED = 1, //NOT SUPPORTED
        ADDRESSING_MODE_SHORT_ADDRESS = 2,//16bits addresses
        ADDRESSING_MODE_EXTENDED_ADDRESS = 3, //NOT SUPPORTED
    };

    typedef struct {
        unsigned frameType :3;
        unsigned securityEnable :1; //NOT SUPPORTED
        unsigned framePending :1; //NOT SUPPORTED
        unsigned ackRequest :1; //NOT SUPPORTED
        unsigned intraPan :1;
        unsigned reserved1 :3; //NOT SUPPORTED
        unsigned destinationAddressingMode :2;
        unsigned reserved2 :2; //NOT SUPPORTED
        unsigned sourceAddressingMode :2;
    } frame_control_t; // 2 bytes

    typedef struct {
        frame_control_t frame_control;
        unsigned beacon_sequence_n :8;
        Address source_address; // Only 16bits addresses suported
        unsigned beacon_order :4;
        unsigned superframe_order :4;
        unsigned final_cap_slot :4; //NOT SUPPORTED
        unsigned batery_life_ext :1; //NOT SUPPORTED
        unsigned reserved :1; //NOT SUPPORTED
        unsigned pan_coordinator :1; //NOT SUPPORTED
        unsigned association_permit :1; //NOT SUPPORTED
        //For the sake of simplicity, the following variable fields are not supported
        //GTS fields
        //Pending address fields
        //Beacon payload field
        unsigned char dummy[6]; //radio workaround, the minimum MAC frame size is 15 (probably a hardware bug)
        unsigned frame_check_sequence :16;
    } beacon_frame_t; // 9 bytes

    typedef struct {
        frame_control_t frame_control;
        unsigned data_sequence_n :8;
        unsigned char dummy[10]; //radio workaround, the minimum MAC frame size is 15 (probably a hardware bug)
        unsigned frame_check_sequence :16;
    } ack_frame_t; // 5 bytes

    typedef struct {
        frame_control_t frame_control;
        unsigned data_sequence_n :8;
        //only 16 bits addresses without PAN ID's are suported
        Address destination_address;
        Address source_address;
    } data_frame_header_t; // 7 bytes

    friend Debug &operator<< (Debug &out, frame_control_t &fc) {
        out << "frameType: " << fc.frameType << "\n";
        //<< "securityEnable: " << fc.securityEnable << "\n"
        //<< "framePending: " << fc.framePending << "\n"
        //<< "ackRequest: " << fc.ackRequest << "\n"
        //<< "intraPan: " << fc.intraPan << "\n"
        //<< "destAddrMode: " << fc.destinationAddressingMode << "\n"
        //<< "srcAddrMode: " << fc.sourceAddressingMode << "\n";
        return out;
    }

    friend Debug &operator<< (Debug &out, beacon_frame_t &bc) {
        out << bc.frame_control
            << "beacon_seq_n: " << bc.beacon_sequence_n << "\n"
            << "src_addr: " << bc.source_address << "\n"
            << "beacon_order: " << bc.beacon_order << "\n"
            << "superframe_order: " << bc.superframe_order << "\n"
            //<< "final_cap_slot: " << bc.final_cap_slot << "\n"
            //<< "bat_life_ext: " << bc.batery_life_ext << "\n"
            //<< "pan_coord: " << bc.pan_coordinator << "\n"
            //<< "assoc_permit: " << bc.association_permit << "\n"
            << "CRC: " << bc.frame_check_sequence << "\n";
        return out;
    }

    friend Debug &operator<< (Debug &out, ack_frame_t &ack) {
        out << ack.frame_control
            << "data_seq_n: " << ack.data_sequence_n << "\n"
            << "CRC: " << ack.frame_check_sequence << "\n";
        return out;
    }

    friend Debug &operator<< (Debug &out, data_frame_header_t &data_header) {
        out << data_header.frame_control
            << "data_seq_n: " << data_header.data_sequence_n << "\n"
            << "src_addr: " << data_header.source_address << "\n"
            << "dst_addr: " << data_header.destination_address << "\n";
        return out;
    }
};

template<typename T>
class IEEE802154_Beacon_Sync: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;
    typedef typename IEEE802154_Frame<T>::beacon_frame_t beacon_frame_t;

    enum {
        MAX_BEACON_ORDER 	 = 8, // up to 14
        MAX_SUPERFRAME_ORDER = 8, // up to 14

        DEFAULT_BEACON_ORDER     = 7,
        DEFAULT_SUPERFRAME_ORDER = 4,
    };

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "IEEE802154_Beacon_Sync - Setting timeout\n";

        if (Traits<CMAC<T> >::COORDINATOR) {
            //send beacons
            _beacon_order = DEFAULT_BEACON_ORDER;
            _superframe_order = DEFAULT_SUPERFRAME_ORDER;
            set_time_out();
            set_sleeping_period();
            _beacon_sequence_n = 0;
            send_beacon();
            send_beacon();
            send_beacon();
            send_beacon();
            //send_beacon(); highly unstable

        } else {
            //receive beacons
            set_time_out();
            CMAC_STATE_TRANSITION result = receive_beacon();

            if (result == CMAC<T>::TIMEOUT) {
                int consecutive_failures = 0;
                while (result != CMAC<T>::RX_END) {
                    set_time_out();
                    result = receive_beacon();

                    //GAMBI - radio HW bug handling - GAMBI
                    if ((result == CMAC<T>::TIMEOUT) || (result == CMAC<T>::RX_ERROR)) {
                        ++consecutive_failures;
                        if (consecutive_failures >= 5) {
                            db<CMAC<T> >(WRN) << "CMAC<T>::IEEE802154_Beacon_Sync - Operation failed 5 times in a row, reseting radio\n";
                            consecutive_failures = 0;
                            CMAC<T>::_radio->reset();
                            CMAC<T>::_radio->off();
                        }

                    } else {
                        consecutive_failures = 0;
                    }
                }
            }

            set_time_out();
            set_sleeping_period();
            db<CMAC<T> >(INF) << "IEEE802154_Beacon_Sync - Synchronized with coordinator\n";
        }

        if (CMAC<T>::_rx_pending) {
            db<CMAC<T> >(TRC) << "IEEE802154_Beacon_Sync - RX_PENDING\n";
            return CMAC<T>::RX_PENDING;

        } else if (CMAC<T>::_tx_pending) {
            db<CMAC<T> >(TRC) << "IEEE802154_Beacon_Sync - TX_PENDING\n";
            return CMAC<T>::TX_PENDING;

        } else {
            db<CMAC<T> >(TRC) << "IEEE802154_Beacon_Sync - SYNC_END\n";
            return CMAC<T>::SYNC_END;
        }
    }

private:
    static CMAC_STATE_TRANSITION pack_beacon(CMAC_STATE_TRANSITION previous) {
        db<CMAC<T> >(TRC) << "IEEE802154_Beacon_Sync - Creating frame\n";

        beacon_frame_t *beacon_frame_ptr =
            reinterpret_cast<beacon_frame_t*>(CMAC<T>::_frame_buffer);

        CMAC<T>::_frame_buffer_size = sizeof(beacon_frame_t);

        beacon_frame_ptr->frame_control.frameType = IEEE802154_Frame<T>::FRAME_TYPE_BEACON;
        beacon_frame_ptr->frame_control.intraPan = IEEE802154_Frame<T>::INTRA_PAN_SAME_PAN;
        beacon_frame_ptr->frame_control.destinationAddressingMode = IEEE802154_Frame<T>::ADDRESSING_MODE_SHORT_ADDRESS;
        beacon_frame_ptr->frame_control.sourceAddressingMode = IEEE802154_Frame<T>::ADDRESSING_MODE_SHORT_ADDRESS;

        beacon_frame_ptr->beacon_sequence_n = ++_beacon_sequence_n;
        beacon_frame_ptr->source_address = CMAC<T>::_addr;
        beacon_frame_ptr->beacon_order = _beacon_order;
        beacon_frame_ptr->superframe_order = _superframe_order;

        beacon_frame_ptr->frame_check_sequence =
            CRC::crc16(reinterpret_cast<char*>(CMAC<T>::_frame_buffer), CMAC<T>::_frame_buffer_size - 2);

        db<CMAC<T> >(INF) << "IEEE802154_Beacon_Sync - Frame created:\n"
            << *beacon_frame_ptr;

        db<CMAC<T> >(TRC) << "IEEE802154_Beacon_Sync - PACK_OK\n";
        return CMAC<T>::PACK_OK;
    }

    static CMAC_STATE_TRANSITION send_beacon() {
        CMAC_STATE_TRANSITION result = CMAC<T>::TX_PENDING;

        result = pack_beacon(result);
        result = Generic_Tx<T>::execute(result);

        db<CMAC<T> >(TRC) << "IEEE802154_Beacon_Sync - TX_END\n";
        return result;
    }

    static CMAC_STATE_TRANSITION unpack_beacon(CMAC_STATE_TRANSITION previous) {
        if (CMAC<T>::_frame_buffer_size == 0) {
            db<CMAC<T> >(ERR) << "IEEE802154_Beacon_Sync - UNPACK_FAILED - Frame size == 0\n";
            return CMAC<T>::UNPACK_FAILED;
        }

        db<CMAC<T> >(TRC) << "IEEE802154_Beacon_Sync - Decoding frame\n";

        beacon_frame_t *beacon_frame_ptr =
            reinterpret_cast<beacon_frame_t*>(CMAC<T>::_frame_buffer);

        db<CMAC<T> >(INF) << "IEEE802154_Beacon_Sync - Frame decoded:\n"
            << *beacon_frame_ptr;

        unsigned short crc = CRC::crc16(reinterpret_cast<char*>(CMAC<T>::_frame_buffer), CMAC<T>::_frame_buffer_size - 2);

        if (beacon_frame_ptr->frame_check_sequence != crc) {
            db<CMAC<T> >(WRN) << "IEEE802154_Beacon_Sync - CRC error: " << crc << "\n";
            return CMAC<T>::UNPACK_FAILED;
        }

        if (beacon_frame_ptr->frame_control.frameType != IEEE802154_Frame<T>::FRAME_TYPE_BEACON) {
            db<CMAC<T> >(WRN) << "IEEE802154_Beacon_Sync - UNPACK_FAILED - Incorrect frame type\n";
            return CMAC<T>::UNPACK_FAILED;
        }

        _beacon_sequence_n = beacon_frame_ptr->beacon_sequence_n;
        _beacon_order = beacon_frame_ptr->beacon_order;
        _superframe_order = beacon_frame_ptr->superframe_order;

        db<CMAC<T> >(TRC) << "IEEE802154_Beacon_Sync - UNPACK_OK\n";
        return CMAC<T>::UNPACK_OK;
    }

    static CMAC_STATE_TRANSITION receive_beacon() {
        CMAC_STATE_TRANSITION result = CMAC<T>::RX_PENDING;

        while (result != CMAC<T>::UNPACK_OK) {
            result = Generic_Lpl<T>::execute(result);
            if (result == CMAC<T>::CHANNEL_IDLE) {
                db<CMAC<T> >(WRN) << "IEEE802154_Beacon_Sync - TIMEOUT\n";
                return CMAC<T>::TIMEOUT;
            }
            result = Generic_Rx<T>::execute(result);
            result = unpack_beacon(result);
        }

        db<CMAC<T> >(TRC) << "IEEE802154_Beacon_Sync - RX_END\n";
        return CMAC<T>::RX_END;
    }

    static void alarm_handler() {
        db<CMAC<T> >(INF) << "IEEE802154_Beacon_Sync - timeout\n";
        while (!CMAC<T>::timeout) CMAC<T>::timeout = true; // no excuses now
        //CMAC<T>::alarm_deactivate();
    }

    static void set_time_out() {
        //set the active cycle in ms
        unsigned short timeout = 0x0001 << _superframe_order;
        timeout *= 15;
        CMAC<T>::timeout = false;
        CMAC<T>::alarm_activate(&alarm_handler, timeout);
    }

    static void set_sleeping_period() {
        unsigned short active_time = 0x0001 << _superframe_order;
        active_time *= 15;
        unsigned short total_time = 0x0001 << _beacon_order;
        total_time *= 15;
        CMAC<T>::_sleeping_period = total_time - active_time;
        //clock drift error compensation
        if (!Traits<CMAC<T> >::COORDINATOR) {
            //unsigned short aux = (0x0001 << _superframe_order); // 2^SO
            CMAC<T>::_sleeping_period -= 7 * _beacon_sequence_n; // the beacon transmit time is 7 ms
        }
    }

    //static bool _coordinator;
    //static bool _first_execution;
    static unsigned char _beacon_order;
    static unsigned char _superframe_order;
    static unsigned char _beacon_sequence_n;
};

template<typename T>
class IEEE802154_Pack: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;
    typedef typename IEEE802154_Frame<T>::data_frame_header_t data_frame_header_t;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {

        db<CMAC<T> >(TRC) << "IEEE802154_Pack - Creating frame\n";

        data_frame_header_t *header_ptr =
            reinterpret_cast<data_frame_header_t*>(CMAC<T>::_frame_buffer);

        unsigned int *data_size_ptr =
            reinterpret_cast<unsigned int*>
            (&(CMAC<T>::_frame_buffer[sizeof(data_frame_header_t)]));

        unsigned char *protocol_ptr = 
            &(CMAC<T>::_frame_buffer[sizeof(data_frame_header_t) + 2]);

        unsigned short *remaining_energy_ptr = 
            reinterpret_cast<unsigned short*>
            (&(CMAC<T>::_frame_buffer[sizeof(data_frame_header_t) + 2 + 2]));

        unsigned char *payload_ptr = &(CMAC<T>::_frame_buffer[sizeof(data_frame_header_t) + 2 + 2 + 2]);

        int offset = (CMAC<T>::_tx_data_size % 2) ? 1 : 0;
        unsigned short *crc_ptr =
            reinterpret_cast<unsigned short*>
            (&(CMAC<T>::_frame_buffer[sizeof(data_frame_header_t) + CMAC<T>::_tx_data_size + 2 + 2 + 2 + offset]));

        CMAC<T>::_frame_buffer_size = sizeof(data_frame_header_t) + CMAC<T>::_tx_data_size + 2 + 2 + 2 + 2 + offset;

        header_ptr->frame_control.frameType                 = IEEE802154_Frame<T>::FRAME_TYPE_DATA;
        header_ptr->frame_control.intraPan                  = IEEE802154_Frame<T>::INTRA_PAN_SAME_PAN;
        header_ptr->frame_control.destinationAddressingMode = IEEE802154_Frame<T>::ADDRESSING_MODE_SHORT_ADDRESS;
        header_ptr->frame_control.sourceAddressingMode      = IEEE802154_Frame<T>::ADDRESSING_MODE_SHORT_ADDRESS;

        header_ptr->source_address      = *CMAC<T>::_addr;
        header_ptr->destination_address = CMAC<T>::_tx_dst_address;
        CMAC<T>::_data_sequence_number  = (CMAC<T>::_data_sequence_number < 255) ? (CMAC<T>::_data_sequence_number + 1) : 0;
        header_ptr->data_sequence_n     = CMAC<T>::_data_sequence_number;

        const unsigned char *aux = reinterpret_cast<const unsigned char*>(CMAC<T>::_tx_data);
        for (unsigned int i = 0; i < CMAC<T>::_tx_data_size; ++i) {
            payload_ptr[i] = aux[i];
        }

        *data_size_ptr = CMAC<T>::_tx_data_size;

        *protocol_ptr = CMAC<T>::_tx_protocol;

        *remaining_energy_ptr = Battery::sys_batt().sample();

        *crc_ptr = CRC::crc16(reinterpret_cast<char*>(CMAC<T>::_frame_buffer), CMAC<T>::_frame_buffer_size - 2);

        db<CMAC<T> >(INF) << "IEEE802154_Pack - Frame created:\n"
            << *header_ptr
            << "payload_size: " << CMAC<T>::_tx_data_size << "\n"
            << "protocol: " << CMAC<T>::_tx_protocol << "\n"
            << "remaining_energy: " << *remaining_energy_ptr << "\n"
            << "CRC: " << *crc_ptr << "\n";

        db<CMAC<T> >(TRC) << "IEEE802154_Pack - PACK_OK\n";

        CMAC<T>::_transmission_count += 1;

        CMAC<T>::_stats->total_tx_packets += 1;

        return CMAC<T>::PACK_OK;
    }
};

template<typename T>
class IEEE802154_Unpack: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;
    typedef typename IEEE802154_Frame<T>::data_frame_header_t data_frame_header_t;
    typedef typename Neighboring::Node Node;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        if (CMAC<T>::_frame_buffer_size == 0) {
            db<CMAC<T> >(ERR) << "IEEE802154_Unpack - UNPACK_FAILED - Frame size == 0\n";
            return CMAC<T>::UNPACK_FAILED;
        }

        input = CMAC<T>::UNPACK_OK;

        db<CMAC<T> >(TRC) << "IEEE802154_Unpack - Decoding frame\n";

        data_frame_header_t *header_ptr =
            reinterpret_cast<data_frame_header_t*>(CMAC<T>::_frame_buffer);

        CMAC<T>::_rx_data_size = CMAC<T>::_frame_buffer[sizeof(data_frame_header_t)];

        CMAC<T>::_rx_protocol = CMAC<T>::_frame_buffer[sizeof(data_frame_header_t) + 2];

        unsigned short *remaining_energy_ptr = 
            reinterpret_cast<unsigned short*>(&(CMAC<T>::_frame_buffer[sizeof(data_frame_header_t) + 2 + 2]));

        unsigned char *payload_ptr = &(CMAC<T>::_frame_buffer[sizeof(data_frame_header_t) + 2 + 2 + 2]);

        unsigned short *crc_ptr =
            reinterpret_cast<unsigned short*>(&(CMAC<T>::_frame_buffer[CMAC<T>::_frame_buffer_size - 2]));

        db<CMAC<T> >(INF) << "IEEE802154_Unpack - Frame decoded:\n"
            << *header_ptr
            << "payload_size: " << CMAC<T>::_rx_data_size << "\n"
            << "protocol: " << CMAC<T>::_rx_protocol << "\n"
            << "remaining_energy: " << *remaining_energy_ptr << "\n"
            << "CRC: " << *crc_ptr << "\n";

        unsigned short crc = CRC::crc16(reinterpret_cast<char*>(CMAC<T>::_frame_buffer), CMAC<T>::_frame_buffer_size - 2);

        if (*crc_ptr != crc) {
            db<CMAC<T> >(WRN) << "IEEE802154_Unpack - CRC error: " << crc << "\n";
            CMAC<T>::_stats->dropped_packets += 1;
            return CMAC<T>::UNPACK_FAILED;

        } else if (header_ptr->frame_control.frameType != IEEE802154_Frame<T>::FRAME_TYPE_DATA) {
            db<CMAC<T> >(WRN) << "IEEE802154_Unpack - UNPACK_FAILED - Incorrect frame type\n";
            input = CMAC<T>::UNPACK_FAILED;

        } else if (!(header_ptr->destination_address == *CMAC<T>::_addr) && !(header_ptr->destination_address == Radio_Common::BROADCAST)) {
            db<CMAC<T> >(INF) << "IEEE802154_Unpack - UNPACK_FAILED - Wrong address\n";
            input = CMAC<T>::UNPACK_FAILED;

        } else {
            CMAC<T>::_data_sequence_number = header_ptr->data_sequence_n;

            unsigned char *aux = reinterpret_cast<unsigned char*>(CMAC<T>::_rx_data);
            for (unsigned int i = 0; i < CMAC<T>::_rx_data_size; ++i) {
                aux[i] = payload_ptr[i];
            }
            CMAC<T>::_rx_src_address = typename CMAC<T>::Address(header_ptr->source_address);

            db<CMAC<T> >(TRC) << "IEEE802154_Unpack - UNPACK_OK\n";
        }

        if (Traits<Neighboring>::enabled) {
            Node * n = new(kmalloc(sizeof(Node))) Node(CMAC<T>::_rx_src_address, *remaining_energy_ptr, CMAC<T>::_radio->lqi(), CMAC<T>::_radio->rssi());
            Neighboring::get_instance()->add_node(n);

            db<CMAC<T> >(ERR) << "CMAC - Neighborhood status\n"
                << Neighboring::get_instance();
        }

        return input;
    }
};

template<typename T>
class IEEE802154_Ack_Rx: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;
    typedef typename IEEE802154_Frame<T>::ack_frame_t ack_frame_t;

    static const unsigned long ACK_TIMEOUT  = 30; // the RX path doesn't take more than 30 ms
    static const int MAX_TRANSMISSION_COUNT =  4; // the RX path doesn't take more than 30 ms

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        if (!Traits<CMAC<T> >::ack)
            return CMAC<T>::TX_OK;

        CMAC_STATE_TRANSITION result = CMAC<T>::RX_PENDING;

        unsigned int size = CMAC<T>::_frame_buffer_size;
        unsigned char aux[size];

        memcpy(aux, CMAC<T>::_frame_buffer, size);

        while (result != CMAC<T>::UNPACK_OK) {
            wait(250);
            result = Generic_Lpl<T>::execute(result, ACK_TIMEOUT);
            if (result == CMAC<T>::CHANNEL_IDLE) {
                if (CMAC<T>::_transmission_count < MAX_TRANSMISSION_COUNT) {
                    if (CMAC<T>::timeout) {
                        db<CMAC<T> >(WRN) << "IEEE802154_Ack_Rx - TIMEOUT\n";
                        return CMAC<T>::TIMEOUT;

                    } else {
                        db<CMAC<T> >(WRN) << "IEEE802154_Ack_Rx - Retransmitting\n";
                        CMAC<T>::_frame_buffer_size = size;
                        memcpy(CMAC<T>::_frame_buffer, aux, size);

                        return CMAC<T>::TX_PENDING;
                    }

                } else {
                    db<CMAC<T> >(WRN) << "IEEE802154_Ack_Rx - TX_FAILED\n";
                    CMAC<T>::_transmission_count = 0;
                    return CMAC<T>::TX_FAILED;
                }
            }
            result = Generic_Rx<T>::execute(result);
            result = unpack_ack(result);
        }

        db<CMAC<T> >(TRC) << "IEEE802154_Ack_Rx - TX_OK\n";
        CMAC<T>::_transmission_count = 0;
        return CMAC<T>::TX_OK;
    }

private:
    static void wait(unsigned int us) {
        for (unsigned int i = 0; i <= us * (Traits<Machine>::CLOCK / 1000000UL); i++); 
    }

    static CMAC_STATE_TRANSITION unpack_ack(CMAC_STATE_TRANSITION input) {

        if (CMAC<T>::_frame_buffer_size == 0) {
            db<CMAC<T> >(ERR) << "IEEE802154_Ack_Rx - UNPACK_FAILED - Frame size == 0\n";
            return CMAC<T>::UNPACK_FAILED;
        }

        db<CMAC<T> >(TRC) << "IEEE802154_Ack_Rx - Decoding frame\n";

        ack_frame_t *ack_frame_ptr =
            reinterpret_cast<ack_frame_t*>(CMAC<T>::_frame_buffer);

        db<CMAC<T> >(INF) << "IEEE802154_Ack_Rx - Frame decoded:\n"
            << *ack_frame_ptr;

        unsigned short crc = CRC::crc16(reinterpret_cast<char*>(CMAC<T>::_frame_buffer), CMAC<T>::_frame_buffer_size - 2);

        if (ack_frame_ptr->frame_check_sequence != crc) {
            db<CMAC<T> >(WRN) << "IEEE802154_Ack_Rx - CRC error: " << crc << "\n";
            return CMAC<T>::UNPACK_FAILED;
        }

        if (ack_frame_ptr->frame_control.frameType != IEEE802154_Frame<T>::FRAME_TYPE_ACK) {
            db<CMAC<T> >(WRN) << "IEEE802154_Ack_Rx - UNPACK_FAILED - Incorrect frame type\n";
            return CMAC<T>::UNPACK_FAILED;
        }

        if (ack_frame_ptr->data_sequence_n != CMAC<T>::_data_sequence_number) {
            db<CMAC<T> >(WRN) << "IEEE802154_Ack_Rx - UNPACK_FAILED - Incorrect data sequence number\n";
            return CMAC<T>::UNPACK_FAILED;
        }

        db<CMAC<T> >(TRC) << "IEEE802154_Ack_Rx - UNPACK_OK\n";
        return CMAC<T>::UNPACK_OK;
    }
};

template<typename T>
class IEEE802154_Ack_Tx: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;
    typedef typename IEEE802154_Frame<T>::ack_frame_t ack_frame_t;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        if (!Traits<CMAC<T> >::ack)
            return CMAC<T>::RX_OK;

        CMAC_STATE_TRANSITION result = CMAC<T>::TX_PENDING;

        result = pack_ack(result);
        CMAC<T>::_radio->off();
        result = Generic_Tx<T>::execute(result);
        wait(100);
        CMAC<T>::_radio->off();

        db<CMAC<T> >(TRC) << "IEEE802154_Ack_Tx - RX_OK\n";
        return CMAC<T>::RX_OK;
    }

private:
    static void wait(unsigned int us) {
        for (unsigned int i = 0; i <= us * (Traits<Machine>::CLOCK / 1000000UL); i++); 
    }

    static CMAC_STATE_TRANSITION pack_ack(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "IEEE802154_Ack_Tx - Creating frame\n";

        ack_frame_t *ack_frame_ptr =
            reinterpret_cast<ack_frame_t*>(CMAC<T>::_frame_buffer);

        CMAC<T>::_frame_buffer_size = sizeof(ack_frame_t);

        ack_frame_ptr->frame_control.frameType = IEEE802154_Frame<T>::FRAME_TYPE_ACK;
        ack_frame_ptr->frame_control.intraPan = IEEE802154_Frame<T>::INTRA_PAN_SAME_PAN;
        ack_frame_ptr->frame_control.destinationAddressingMode = IEEE802154_Frame<T>::ADDRESSING_MODE_SHORT_ADDRESS;
        ack_frame_ptr->frame_control.sourceAddressingMode = IEEE802154_Frame<T>::ADDRESSING_MODE_SHORT_ADDRESS;

        ack_frame_ptr->data_sequence_n = CMAC<T>::_data_sequence_number;

        memset(ack_frame_ptr->dummy, 0, 10);

        ack_frame_ptr->frame_check_sequence =
            CRC::crc16(reinterpret_cast<char*>(CMAC<T>::_frame_buffer), CMAC<T>::_frame_buffer_size - 2);

        db<CMAC<T> >(INF) << "IEEE802154_Ack_Tx - Frame created:\n"
            << *ack_frame_ptr;

        db<CMAC<T> >(TRC) << "IEEE802154_Ack_Tx - PACK_OK\n";
        return CMAC<T>::PACK_OK;
    }
};

template<typename T>
class IEEE802154_Slotted_CSMA_Contention: public CMAC_State<T> {
public:
    typedef typename CMAC<T>::CMAC_STATE_TRANSITION CMAC_STATE_TRANSITION;

    static CMAC_STATE_TRANSITION execute(CMAC_STATE_TRANSITION input) {
        db<CMAC<T> >(TRC) << "IEEE802154_Slotted_CSMA_Contention - Checking channel\n";

        unsigned int nb = 0;
        unsigned int be = MIN_BE;
        while (nb < MAX_BACKOFFS) {

            //delay = random(2^BE -1)*Period in ms
            unsigned long delay = static_cast<unsigned long> ((Pseudo_Random::random() % (2 << (be-1))) * UNIT_BACKOFF_PERIOD);

            //for some reason the delay can NEVER be 0
            if (delay == 0) 
                delay = static_cast<unsigned long>(UNIT_BACKOFF_PERIOD);

            if (CMAC<T>::timeout || (delay >= CMAC<T>::alarm_event_time_left()))
                return CMAC<T>::TIMEOUT;

            db<CMAC<T> >(INF) << "IEEE802154_Slotted_CSMA_Contention - Delay = " << delay << "\n";

            CMAC<T>::alarm_busy_delay(delay);

            //Clear Channel Assesment(CCA)
            bool aux = CMAC<T>::_radio->cca();
            if (aux) {
                db<CMAC<T> >(TRC) << "IEEE802154_Slotted_CSMA_Contention - CHANNEL_IDLE\n";
                return CMAC<T>::CHANNEL_IDLE;
            }
            nb = nb + 1;
            be = be + 1;
            if (be > MAX_BE)
                be = MAX_BE;
        }

        db<CMAC<T> >(WRN) << "IEEE802154_Slotted_CSMA_Contention - CHANNEL_BUSY\n";
        return CMAC<T>::CHANNEL_BUSY;
    }

private:
    enum{
        MIN_BE = 4,
        MAX_BE = 7,
        MAX_BACKOFFS = 8,
        UNIT_BACKOFF_PERIOD = 30 // ms
    };
};

__END_SYS

#endif

