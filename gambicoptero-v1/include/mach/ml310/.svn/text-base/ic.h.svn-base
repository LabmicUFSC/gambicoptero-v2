// EPOS ML310 Interrupt Controller Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __ml310_ic_h
#define __ml310_ic_h

#include <cpu.h>
#include <ic.h>

__BEGIN_SYS

class ML310_IC: public IC_Common
{
private:
    typedef CPU::Reg32 Mask;

public:
    enum {
        ISR = Traits<ML310_IC>::BASE_ADDRESS + 0,
        IPR = Traits<ML310_IC>::BASE_ADDRESS + 4,
        IER = Traits<ML310_IC>::BASE_ADDRESS + 8,
        IAR = Traits<ML310_IC>::BASE_ADDRESS + 12,
        SIE = Traits<ML310_IC>::BASE_ADDRESS + 16,
        CIE = Traits<ML310_IC>::BASE_ADDRESS + 20,
        IVR = Traits<ML310_IC>::BASE_ADDRESS + 24,
        MER = Traits<ML310_IC>::BASE_ADDRESS + 28
    };

    // ML310 Interrupts
    enum {
        //Processor exceptions....
        INT_MACHINE         = 0,
        INT_DEBUG           = 1,
        INT_WATCHDOG        = 2,
        INT_INSTTBLMISS     = 3,
        INT_INSTSTORE       = 4,
        INT_PROGRAM         = 5,
        INT_SYSCALL         = 6,
        INT_DATATBLMISS     = 7,
        INT_DATASTORE       = 8,
        INT_ALIGNMENT       = 9,

        //Irqs ...
        INT_FITIMER         = 10,
        INT_PITIMER         = 11,

        //IntC Irqs
        INT_PCI_INT_OR      = 12,
        INT_OPB_PCI_IRPT    = 13,
        INT_SPI_INTR        = 14,
        INT_IIC_TEMP_CRIT   = 15,
        INT_IIC_IRQ         = 16,
        INT_IIC_INTR        = 17,
        INT_SYSACE_INTR     = 18,
        INT_UART_INTR       = 19,

    };

    // IRQs - Review!
    static const unsigned int IRQS = 32;
    enum {
        IRQ_TIMER = INT_PITIMER,
    };

public:
    ML310_IC() {}

    static void enable(IRQ irq) {
        db<ML310_IC>(TRC) << "ML310_IC::enable(irq=" << irq << ")\n";
        if(irq > 11 && irq < 20) {
          irq -= 12;
          sie((1 << irq));
        } else {//PCI Interrupts
          //kout << "External Interrupt !!????\n";
        }
    }

    static void disable() {
        db<ML310_IC>(TRC) << "Disabling all IC interrupts!\n";
        cie(0xFFFFFFFF);
    }

    static void disable(IRQ irq) {
        db<ML310_IC>(TRC) << "ML310_IC::disable(irq=" << irq << ")\n";
        if(irq > 11 && irq < 20) {
          irq -= 12;
          cie(1 << irq);
        } else { //PCI Interrupts
          //kout << "External Interrupt !!????\n";
        }
    }

    static void init();

private:
    static void sie(const Mask & mask) {
        *((volatile Mask *)SIE) = mask;
    }
    static void cie(const Mask & mask) {
        *((volatile Mask *)CIE) = mask;
    }

};

__END_SYS

#endif
