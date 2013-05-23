#ifndef __cmac128_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __cmac128_h

#include "../avr_common/cc1000/cc1000.h"
#include <nic.h>
#include <cpu.h>
#include <utility/crc.h>
#include <timer.h>
#include <radio.h>


__BEGIN_SYS 

class CMAC: public Radio_Common {

private:
    typedef Radio_Common::Address Address;

    enum CMAC_STATE { IDLE, RX, TX };

    enum TX_STATE { TX_PENDING, TX_BACKOFF, TX_PREAMBLE, TX_DATA, TX_DONE };

    enum RX_STATE { RX_IDLE, RX_SYNC, RX_DATA, RX_DONE };

    static const int FREQUENCY = Traits<CMAC>::FREQUENCY;
    static const int POWER = Traits<CMAC>::POWER;

    static const unsigned int PREAMBLE_LENGTH = 80;
    static const unsigned int PREAMBLE_KEY = 0XCC33; 
    static const unsigned int MAX_RX_SYNC_TRIES = 5;

public:
  
    CMAC(int unit = 0) {
	_cc1000.config(FREQUENCY,POWER);
	_addr = _cc1000.id();
    }

    int send(const Address & dst, const Protocol & prot,
	     const void *data, unsigned int size);

    int receive(Address * src, Protocol * prot, 
		void * data, unsigned int size);

    const Address & address() {
	return _addr;
    }
	
    const Statistics & statistics() {
	return (Statistics&)_stats; 
    }

    void reset() { 
	_cc1000.config(FREQUENCY,POWER);
    }

    void config(int frequency, int power){
	_cc1000.config(frequency,power);
    }

    unsigned int mtu() const { return MTU; }

    static void init(unsigned int n);

    static void timer_handler();
    static void spi_handler();

private:



    static void tx_state_machine();
    static bool tx_preamble();
    static bool tx_data();
    static void tx_handle();

    static void rx_state_machine();
    static bool rx_preamble();
    static bool rx_sync();
    static bool rx_data();
    static int  rx_handle();
    static void rx_giveup();

private:

    static CC1000                   _cc1000;
    static Address                  _addr;
    static volatile Statistics      _stats;

    static volatile CMAC_STATE      _state;

    static volatile TX_STATE        _tx_state;
    static volatile Frame           _tx_frame;
    static volatile unsigned char * _tx_frame_ptr;
    static volatile bool            _tx_available;
    static volatile unsigned char   _tx_preamble_count;
    static volatile unsigned char   _tx_data_count;

    static volatile RX_STATE        _rx_state;
    static  Frame           _rx_frame;
    static volatile unsigned char * _rx_frame_ptr;
    static volatile bool            _rx_available;
    static volatile unsigned char   _rx_preamble_count;
    static volatile unsigned char   _rx_sync_tries;
    static volatile unsigned char   _rx_sync_count;
    static volatile unsigned char   _rx_bit_offset;
    static volatile unsigned char   _rx_data_byte;
    static volatile union data_word {
	struct {
	    unsigned char lsb;
	    unsigned char msb;
	};
	unsigned int word;
    }  _rx_data_word;
    static volatile unsigned char   _rx_data_count;
    static volatile unsigned char   _rx_tries;

    static Timer_2        _timer;

};

__END_SYS
#endif
