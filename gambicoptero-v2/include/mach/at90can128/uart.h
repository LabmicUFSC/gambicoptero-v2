// EPOS AT90CAN128_UART Declarations

#ifndef __at90can128_uart_h
#define __at90can128_uart_h

#include "../avr_common/uart.h"

__BEGIN_SYS

class AT90CAN128_UART: public UART_Common, public AVR_UART
{
public:
    AT90CAN128_UART(unsigned int unit = 0) : AVR_UART(9600,8,0,1,unit) {}
    AT90CAN128_UART(unsigned int baud, unsigned int data_bits, unsigned int parity,
	       unsigned int stop_bits, unsigned int unit = 0) 
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



