// EPOS AXI4LITE UART Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __axi4lite_uart_h
#define __axi4lite_uart_h

#include <uart.h>
#include <cpu.h>
#include <ic.h>

__BEGIN_SYS

class AXI4LITE_UART: protected UART_Common
{
private:
    typedef CPU::Reg8 Reg8;

	typedef struct {
	  volatile unsigned int clkdiv;  // Set to 50e6 (clock) divided by baud rate (no x16 factor)
	  volatile unsigned int txlevel; // Number of spaces in the FIFO for writes
	  volatile unsigned int rxlevel; // Number of available elements in the FIFO for reads
	  volatile unsigned int txchar;  // Write characters to be sent here
	  volatile unsigned int rxchar;  // Read received characters here
	} MM_Reg;

    static const unsigned int CLOCK = Traits<AXI4LITE_UART>::CLOCK;
    static const unsigned int BASE_ADDRESS = Traits<AXI4LITE_UART>::BASE_ADDRESS;
	static const unsigned int BAUDRATE = Traits<AXI4LITE_UART>::BAUDRATE;

public:
    AXI4LITE_UART(unsigned int unit = 0) : _unit(unit) { }

    char get() {
		while ((_regs->rxlevel) == 0);  // wait for data to be ready;
		return (char)_regs->rxchar;
	}

    void put(char c) {
		while (_regs->txlevel == 0);    // wait for fifo to have space
	  	_regs->txchar = (unsigned int)c;
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
	static MM_Reg * _regs;
	
};

__END_SYS

#endif
