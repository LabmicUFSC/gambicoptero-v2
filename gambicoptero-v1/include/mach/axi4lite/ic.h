// EPOS AXI4LITE Interrupt Controller Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __axi4lite_ic_h
#define __axi4lite_ic_h

#include <cpu.h>
#include <ic.h>

__BEGIN_SYS

class AXI4LITE_IC: public IC_Common
{
private:
    typedef CPU::Reg32 Mask;

	typedef struct {
	  volatile unsigned int edge_enable; // 1 -> edge triggered, 0-> level trigered
	  volatile unsigned int polarity;    // 1 -> rising edge / high level, 0-> falling edge / low level
	  volatile unsigned int mask;    	 // 1 -> masked / disable, 0-> not masked / enabled
	  volatile unsigned int pending;     // 1 -> pending; write 1's to clear pending ints
	} MM_Reg;

	static const unsigned int BASE_ADDRESS = Traits<AXI4LITE_IC>::BASE_ADDRESS;
	static const unsigned int EDGE_ENABLE = Traits<AXI4LITE_IC>::EDGE_ENABLE;
	static const unsigned int POLARITY = Traits<AXI4LITE_IC>::POLARITY;
    static const unsigned int INTS = Traits<AXI4LITE_IC>::NUMBER_OF_INTERRUPTS;
	static const unsigned int CPU_JMP_ADDRESS = Traits<AXI4LITE_IC>::CPU_JMP_ADDRESS;

public:

    enum {
		IRQ_TIMER		= 0,
		IRQ_UART_READ	= 1,
		IRQ_UART_WRITE	= 2,
		IRQ_GPIO_0		= 3,
		IRQ_GPIO_1		= 4,
		IRQ_GPIO_2		= 5,
		IRQ_GPIO_3		= 6
    };

public:

    AXI4LITE_IC() {}
	
	static Interrupt_Handler int_vector(Interrupt_Id i) {
		if (i < INTS) 
			return _int_vector[i];
        else {
            db<AXI4LITE>(WRN) << "Interrupt number " << i << " is greater than VECTOR_SIZE (" << INTS << ")\n";
			return 0;
        }
    }
	
    static void int_vector(Interrupt_Id i, Interrupt_Handler h) {
		if (i < INTS) _int_vector[i] = h;
	}
		
    static void enable(Interrupt_Id irq) {
		db<AXI4LITE_IC>(TRC) << "AXI4LITE_IC::enable(irq=" << irq << ")\n";
		_regs->mask &= ~(1<<irq);
    }

    static void mask(unsigned int mask) {
       db<AXI4LITE_IC>(TRC) << "AXI4LITE_IC::mask(mask=" << mask << ")\n";
	   _regs->mask = mask;
    }

    static void disable() {
        db<AXI4LITE_IC>(TRC) << "AXI4LITE_IC Disabling all IC interrupts!\n";
		_regs->mask = ~0;
		_regs->pending = ~0;
    }

    static void disable(Interrupt_Id irq) {
		db<AXI4LITE_IC>(TRC) << "AXI4LITE_IC::disable(irq=" << irq << ")\n";
		_regs->mask |= (1<<irq);
		_regs->pending = (1<<irq);
    }

    static int irq2int(int i) { return i; } 
    static int int2irq(int i) { return i; } 

    static void int_handler(Interrupt_Id i); 

    static void init();

private:
    static void interrupt_dispatcher();
	static void int_no_handler(Interrupt_Id interrupt);
    
	static MM_Reg * _regs;
    static Interrupt_Handler _int_vector[INTS];
	
};

__END_SYS

#endif
