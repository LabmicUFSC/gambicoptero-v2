// EPOS CMAC Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>
#include <nic.h>
#include <mach/atmega128/cmac128.h>
#include <utility/ostream.h>

__BEGIN_SYS

int CMAC::send(const Address & dst, const Protocol & prot,
	       const void *data, unsigned int size) {

    if(_tx_available){
	_tx_available = false;
	Frame * _frame  = new((void *)&_tx_frame) 
	    Frame(dst,_addr,prot,data,size,0,0);
	_tx_frame_ptr= (unsigned char *) _frame;
	_tx_data_count = 0;
	_tx_preamble_count = 0;
	return size;
    } 
    return 0;
	
}

int CMAC::receive(Address * src, Protocol * prot, 
		  void * data, unsigned int size) {
    if(!_rx_available)
	return 0;

    *src = _rx_frame._src;
    *prot = _rx_frame._prot;
    memcpy(data, (void*)&_rx_frame._data, size);
    CPU::int_disable();
    _rx_available = false;
    CPU::int_enable();

    return size;
}

/* -------------- TX -------------- */

void CMAC::tx_state_machine() {

    switch (_tx_state) {
    case TX_BACKOFF:
	for(unsigned int i = 0; i < 0xff; i++);
	_tx_state = TX_PREAMBLE;
	break;

    case TX_PREAMBLE:
	if(tx_preamble())
	    _tx_state = TX_DATA;
	break;

    case TX_DATA:
	if(tx_data()) {
	    _tx_state = TX_DONE;
	    tx_handle();
	}
	break;
    default:
    	break;
    }

}


bool CMAC::tx_preamble(){
    if (_tx_preamble_count++ < PREAMBLE_LENGTH){
	_cc1000.put(~0x55);
	return false;
    } else if (_tx_preamble_count++ == PREAMBLE_LENGTH+1){
	_cc1000.put(~PREAMBLE_KEY>>8);
	return false;	
    } else{
	_cc1000.put(~(unsigned char)PREAMBLE_KEY);
	_tx_preamble_count = 0;
	return true;
    }
}

bool CMAC::tx_data(){
    if (_tx_data_count++ < sizeof(Frame)){
	_cc1000.put(~*_tx_frame_ptr++);
	return false;
    } else {
	_cc1000.put(~0x55);
	return true;
    }
}


void CMAC::tx_handle(){
    _cc1000.int_disable();
    _cc1000.disable();
    _stats.tx_bytes += sizeof(Frame);
    _stats.tx_packets++;
    _tx_available = true;
    _timer.enable();
}

/* -------------- RX -------------- */

void CMAC::rx_state_machine() {

    _rx_data_byte = _cc1000.get();

    switch (_rx_state) {
    case RX_IDLE:
	if(rx_preamble())
	    _rx_state = RX_SYNC;
	break;

    case RX_SYNC:
	if(rx_sync())
	    _rx_state = RX_DATA;
	break;
				
    case RX_DATA:
	if(rx_data()) {
	    _rx_state = RX_DONE;
	    rx_handle();
	} 
	break;
    default:
    	break;
    }

}


bool CMAC::rx_preamble(){
    if ((_rx_data_byte == 0xAA) || (_rx_data_byte == 0x55)) {
	if (++_rx_preamble_count > 2) {
	    _rx_preamble_count = _rx_sync_count = 0;
	    _rx_bit_offset = 0;
	    return true;
	} 
    } 
    return false;
}

bool CMAC::rx_sync(){

    if ((_rx_data_byte == 0xAA) || (_rx_data_byte == 0x55)) {
	_rx_data_word.msb = _rx_data_byte;
    } else {
	switch(_rx_sync_count){
	case 0:
	    _rx_data_word.lsb = _rx_data_byte;
	    break;
	case 1:
	case 2: {
	    unsigned int tmp = _rx_data_word.word;
	    _rx_data_word.msb = _rx_data_word.lsb;
	    _rx_data_word.lsb = _rx_data_byte;
	    for (int i = 0; i < 8; i++) {
		tmp <<= 1;					
		if (_rx_data_byte & 0x80)
		    tmp |= 0x01;				
		_rx_data_byte <<= 1;
		if (tmp == PREAMBLE_KEY) {
		    _rx_bit_offset = 7 - i;
		    return true;
		}				
	    }
	    break;
	}
	default:
	    _rx_preamble_count = 0;
	    rx_giveup();
	    break;
	}
	_rx_sync_count++;
    }
    return false;
}

bool CMAC::rx_data(){

    _rx_data_word.msb = _rx_data_word.lsb;
    _rx_data_word.lsb = _rx_data_byte;
	    
    *_rx_frame_ptr++ = 0x00ff & _rx_data_word.word >> _rx_bit_offset;
		
    if (++_rx_data_count == sizeof(Frame)) 
	return true;

    return false;
	
}

int CMAC::rx_handle() {

    rx_giveup();
						
    unsigned short crc = 
	CRC::crc16((char *) &_rx_frame, sizeof(Frame) - 2);
		
    _stats.rx_bytes += sizeof(Frame);
    _stats.rx_packets++;
			
    if(crc == _rx_frame._crc) {
	CPU::int_disable();
	_rx_available = true;
	CPU::int_enable();
	return _rx_frame._len;
    } else {
	_stats.dropped_packets++;
	return 0;
    }
		
}

void CMAC::rx_giveup(){
    _cc1000.int_disable();
    _cc1000.disable();
    _rx_state = RX_IDLE;
    _timer.enable();
}

/* ----------- Handlers ----------- */

// Triggered each CC1000 data tick
void CMAC::spi_handler(){
    switch(_state) {
    case RX:
	rx_state_machine();
	break;
    case TX:
	tx_state_machine();
	break;
    default:
    	break;
    }
}

// Triggered after sleeping
void CMAC::timer_handler(){

    if(!_tx_available) {
	_cc1000.enable();
	_timer.disable();

	_state = TX;
	_tx_state = TX_BACKOFF;

	_cc1000.tx_mode();

	_cc1000.int_enable();

    } else if (!_rx_available) {
	_cc1000.enable();
	_timer.disable();

	_cc1000.rx_mode();
	_rx_frame_ptr= (unsigned char *) &_rx_frame;
	_rx_data_count = 0;   
	_rx_sync_count = 0;
	_rx_sync_tries = 0;
	_state = RX;
	_rx_state = RX_IDLE;

	_cc1000.int_enable();
    }

}


// Static Class Members

CC1000                    CMAC::_cc1000;
CMAC::Address             CMAC::_addr;
volatile CMAC::Statistics CMAC::_stats;

volatile CMAC::CMAC_STATE CMAC::_state;

volatile CMAC::TX_STATE   CMAC::_tx_state;
volatile CMAC::Frame      CMAC::_tx_frame;
volatile unsigned char *  CMAC::_tx_frame_ptr;
volatile bool             CMAC::_tx_available = true;
volatile unsigned char    CMAC::_tx_preamble_count = 0;
volatile unsigned char    CMAC::_tx_data_count = 0;

volatile CMAC::RX_STATE   CMAC::_rx_state;
CMAC::Frame      CMAC::_rx_frame;
volatile unsigned char *  CMAC::_rx_frame_ptr;
volatile bool             CMAC::_rx_available = false;
volatile unsigned char    CMAC::_rx_preamble_count = 0;
volatile unsigned char    CMAC::_rx_sync_count = 0;
volatile unsigned char    CMAC::_rx_sync_tries = 0;
volatile unsigned char    CMAC::_rx_bit_offset;
volatile unsigned char    CMAC::_rx_data_byte;
volatile CMAC::data_word  CMAC::_rx_data_word;
volatile unsigned char    CMAC::_rx_data_count = 0;
volatile unsigned char    CMAC::_rx_tries = 0;

ATMega128_Timer_2 CMAC::_timer;

 __END_SYS


