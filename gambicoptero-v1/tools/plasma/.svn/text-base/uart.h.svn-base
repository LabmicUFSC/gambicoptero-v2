// EPOS-- Plasma UART Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __plasma_uart_h
#define __plasma_uart_h

class Plasma_UART
{
public:

    Plasma_UART(unsigned int unit = 0) : _unit(unit) {
    }
    bool has_data() {
	irq_status = (volatile unsigned int *)0x20000020;
	return ((*irq_status & 0x01) != 0);
    }
    bool can_send() {
	irq_status = (volatile unsigned int *)0x20000020;
	return ((*irq_status & 0x02) != 0);
    }
    unsigned char get() {
            irq_status = (volatile unsigned int *)0x20000020;
            while((*irq_status & 0x01) == 0);
	    unsigned char byte = (unsigned char)*(volatile unsigned int*)0x20000000;
	    return byte;
    }
    void put(unsigned char c) {
	    irq_status = (volatile unsigned int*)0x20000020;
	    while((*irq_status & 0x02) == 0);
	    *(volatile unsigned int*)0x20000000 = (unsigned int)c;
    }

private:
    int _unit;
    volatile unsigned int * irq_status;
    volatile unsigned int * buffer;

};

#endif
