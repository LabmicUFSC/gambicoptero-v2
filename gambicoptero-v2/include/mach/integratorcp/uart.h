#ifndef __pl011_h__
#define __pl011_h__

#include <uart.h>

__BEGIN_SYS

class PL011_UART : public UART_Common {
    protected:
        const static unsigned char TX_BUFFER_FULL  = (1 << 5);
        const static unsigned char RX_BUFFER_EMPTY = (1 << 4);

    public:
        PL011_UART(unsigned long base) {
            _base = (unsigned long*)base;
            _flag = (unsigned long*)(base + 0x18);
        }

        void put(unsigned char c) {
            while (*_flag & TX_BUFFER_FULL);
            *_base = c;
        }

        char get() {
            while (*_flag & RX_BUFFER_EMPTY);
            *_flag |= RX_BUFFER_EMPTY;
            return (char)(*_base & 0xFF);
        }

    protected:
        volatile unsigned long * _base;
        volatile unsigned long * _flag;
};

// VersatilePB has 4 UARTs
class VersatilePB_UART : public PL011_UART {
    public:
        VersatilePB_UART(unsigned int unit=0) : PL011_UART((0x10009+unit) << 12) {}
};

// IntegratorCP has 2 UARTs
class IntegratorCP_UART : public PL011_UART {
    public:
        IntegratorCP_UART(unsigned int unit=0) :
            PL011_UART((0x16+unit)<<24) {}

        // Dummy contructor with configuration
        IntegratorCP_UART(unsigned int baud, unsigned int data_bits, unsigned int parity,
	    unsigned int stop_bits, unsigned int unit = 0) : PL011_UART((0x16+unit)<<20) {}
};

// Those are the same
class RealView_UART : public PL011_UART {
    public:
        RealView_UART(unsigned int unit=0) : PL011_UART((0x10009+unit) << 12) {}
};


__END_SYS

#endif
