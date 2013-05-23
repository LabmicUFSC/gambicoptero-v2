// EPOS-- MC13224V Interrupt Controller Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mc13224v_ic_h
#define __mc13224v_ic_h

#include <ic.h>
#include <cpu.h>
#include <system/memory_map.h>

__BEGIN_SYS

class MC13224V_IC: public IC_Common
{
    private:
        typedef IO_Map<Machine> IO;

    public:
        typedef void (*Interrupt_Handler)();

    protected:

        static Interrupt_Handler vector[];

        MC13224V_IC() {}

    public:


	enum {
		IRQ_SWI = 0,
		IRQ_UART1,
		IRQ_UART2,
		IRQ_CRM,
		IRQ_I2C,
		IRQ_TIMER,
		IRQ_SPIF,
		IRQ_MACA,
		IRQ_SSI,
		IRQ_ADC,
        IRQ_SPI,
        IRQ_TSC
    };

    static void init();

	static void enable(Interrupt_Id id) {
		CPU::out32(IO::ITC_INTENNUM, id);
	}

	static void disable() {
		CPU::out32(IO::ITC_INTENABLE, 0x00000000);
	}

	static void disable(Interrupt_Id id) { 
		CPU::out32(IO::ITC_INTDISNUM, id);
	}

	static void force_interrupt(Interrupt_Id id) {
		CPU::out32(IO::ITC_INTFRC, (1 << id));
	}

	static bool interrupt_pending(Interrupt_Id id) {
	    return (((CPU::in32(IO::ITC_NIPEND) & (1 << id)) >> id) == 1);
	}

	static void int_vector(Interrupt_Id id, Interrupt_Handler handler){
        db<MC13224V_IC>(INF) << "Registering handler for interrupt " << id << " at " << (void*) handler << "\n";
		vector[id] = handler;
	}

	static void int_handler();

};



__END_SYS

#endif
