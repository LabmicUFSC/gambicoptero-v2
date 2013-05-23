// EPOS PLASMA UART Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __plasma_uart_h
#define __plasma_uart_h

#include <uart.h>
#include <cpu.h>
#include <ic.h>

__BEGIN_SYS

class PLASMA_UART: protected UART_Common
{
private:
    typedef CPU::Reg8 Reg8;

    static const unsigned int CLOCK = Traits<PLASMA_UART>::CLOCK / 16; ////Verify !!!
    static const unsigned int UART_DATA_ADDRESS   = Traits<PLASMA_UART>::DATA_ADDRESS;
    static const unsigned int UART_STATUS_ADDRESS = Traits<PLASMA_UART>::STATUS_ADDRESS;

public:

    enum IRQ_UART {
	UART_WRITE_BUSY      = 0x02,
	UART_DATA_AVALIABLE  = 0x01
    };

public:
    PLASMA_UART(unsigned int unit = 0) : _unit(unit) {
    }

    char get() {
	while (*(volatile unsigned int*)UART_STATUS_ADDRESS &  UART_DATA_AVALIABLE == 0);
	unsigned char c = *(volatile unsigned char*)UART_DATA_ADDRESS;
	return c;
    }

    void put(char c) {
	while (*(volatile unsigned int*)UART_STATUS_ADDRESS & UART_WRITE_BUSY == 1);
	*(volatile unsigned char*)UART_DATA_ADDRESS = (unsigned char)(c);
    }

    void reset() {
    }

    void loopback(bool flag) {
    }

    void int_enable(bool receive = true, bool send = true, bool line = true, bool modem = true) {
    }

    void int_disable() {
    }

    static void int_handler(unsigned int interrupt);

    static void init();

private:

private:
    int _unit;
};

__END_SYS

#endif
