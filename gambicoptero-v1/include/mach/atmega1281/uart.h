// EPOS ATMega1281 UART Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega1281_uart_h
#define __atmega1281_uart_h

#include "../avr_common/uart.h"

__BEGIN_SYS

class ATMega1281_UART: public UART_Common, private AVR_UART
{
public:
    ATMega1281_UART(unsigned int unit = Traits<ATMega1281_UART>::DEFAULT_UNIT) : AVR_UART(9600, 8, 0, 1, unit) {}
    ATMega1281_UART(unsigned int baud, unsigned int data_bits, 
		   unsigned int parity, unsigned int stop_bits,
		   unsigned int unit = Traits<ATMega1281_UART>::DEFAULT_UNIT) 
	: AVR_UART(baud, data_bits, parity, stop_bits,unit) {}

    void config(unsigned int baud, unsigned int data_bits,
		unsigned int parity, unsigned int stop_bits) {
	AVR_UART::config(baud, data_bits, parity, stop_bits);
    }
    void config(unsigned int * baud, unsigned int * data_bits,
		unsigned int * parity, unsigned int * stop_bits) {
	AVR_UART::config(*baud, *data_bits, *parity, *stop_bits);
    }

    char get() { while(!rxd_full()); return rxd(); }
    void put(char c) { while(!txd_empty()); txd(c); }

    void loopback(bool flag) { AVR_UART::loopback(flag); }
};

__END_SYS

#endif

