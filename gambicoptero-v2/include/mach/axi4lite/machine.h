// EPOS AXI4LITE Mediator Declarations

#ifndef __axi4lite_h
#define __axi4lite_h

#include <utility/list.h>
#include <arch/mips32/cpu.h>
#include <arch/mips32/mmu.h>
#include <arch/mips32/tsc.h>
#include <machine.h>
#include "ic.h"
#include "device.h"

__BEGIN_SYS


class AXI4LITE: public Machine_Common
{
private:
    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;
    typedef CPU::Context Context;
    typedef CPU::Reg16 Reg16;

public:
    AXI4LITE() {}

    static void panic();
    static void reboot() { /*TODO*/ };
	static void poweroff() { /*TODO*/ };

    static unsigned int n_cpus() { return 1; }
    static unsigned int cpu_id() { return 0; }
    static void smp_init(unsigned int n_cpus) { }
    static void smp_barrier(int n_cpus = 0) { }

    static void init();
	
};

__END_SYS

#include "ic.h"
#include "timer.h"
#include "rtc.h"
#include "uart.h"
#include "display.h"
#include "nic.h"

#endif
