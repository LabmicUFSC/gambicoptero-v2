// EPOS-- MC13224V IC Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/mc13224v/ic.h>

#include <machine.h>
#include <system/memory_map.h>

extern unsigned short motor[4];
__BEGIN_SYS

MC13224V_IC::Interrupt_Handler MC13224V_IC::vector[12];
OStream coute;
unsigned char state = 0;

void MC13224V_IC::int_handler() {
    if(CPU::in16(IO::TIMER2_SCTRL) & (1 << 15)) {
        CPU::out32(IO_Map<Machine>::GPIO_DATA0, 0x00000000);
        CPU::out16(
            IO::TIMER2_CSCTRL,
            CPU::in16(IO::TIMER2_CSCTRL) & ~(1 << 4)
        );
        CPU::out16(
            IO::TIMER2_SCTRL,
            CPU::in16(IO::TIMER2_SCTRL) & ~(1 << 15)
        );
        return;
    }
    
   if(CPU::in16(IO::TIMER2_SCTRL) & (1 << 13)) {
        CPU::out32(IO_Map<Machine>::GPIO_DATA0, (1 << (2+23+state)));
        CPU::out16(IO::TIMER2_COMP1, motor[state]);
        state++;
        if(state == 4) state = 0;
        
        CPU::out16(
            IO::TIMER2_SCTRL,
            CPU::in16(IO::TIMER2_SCTRL) & ~(1 << 13)
        );
        return;
    } 


    CPU::Reg16 pending = CPU::in16(IO_Map<Machine>::ITC_NIPEND);
    //db<MC13224V_IC> (TRC) << "MC13224V_IC::int_handler(" << (int)pending << ")\n";

    // Maybe we can just shut down everything else and
    // assume all interrupts are timer interrupts ?
    if((1 << IRQ_TIMER) & pending)
    { CPU::Reg16 r;
        if((Traits<TSC>::enabled) && ((r = CPU::in16(IO::TIMER3_CSCTRL)) & 0x0010)) {
            CPU::out16(IO::TIMER3_CSCTRL, r & ~0x0010);
            db<MC13224V_IC> (INF) << "Interrupt! IRQ_TSC\n";
            vector[IRQ_TSC]();
            return;
        }
        db<MC13224V_IC> (INF) << "Interrupt! IRQ_TIMER\n";
        vector[IRQ_TIMER]();
    } else if((1 << IRQ_UART1) & pending)
    {
        db<MC13224V_IC> (INF) << "Interrupt! IRQ_UART1\n";
        vector[IRQ_UART1]();
    } else if((1 << IRQ_UART2) & pending)
    {
        db<MC13224V_IC> (INF) << "Interrupt! IRQ_UART2\n";
        vector[IRQ_UART2]();
    } else if((1 << IRQ_MACA) & pending)
    {
        db<MC13224V_IC> (INF) << "Interrupt! IRQ_MACA\n";
        vector[IRQ_MACA]();
    } else
    {
        db<MC13224V_IC> (INF) << "Unexpected Interrupt! => " << (unsigned int)pending << "\n";
//        Machine::panic();
    }
}

__END_SYS
