#ifndef __integrator_machine_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __integrator_machine_h

#include <machine.h>
#include <cpu.h>
#include <mmu.h>
#include <tsc.h>

__BEGIN_SYS

class IntegratorCP: public Machine_Common
{
    static const unsigned long CM_CTRL = 0x1000000C;
public:
    IntegratorCP() {}

    static unsigned int cpu_id() { return 0; }
    static unsigned int n_cpus() { return 1; }

    static void panic() { 
		db<IntegratorCP>(ERR) << "PANIC!\n";
		CPU::int_disable();
		for(;;);
	}
    static void reboot() { 
        db<IntegratorCP>(TRC) << "Machine::reboot()\n";
        // qemu is messing with the console when we reset the board
        // this for() will avoid reseting by now
        //for(;;);
        CPU::out32(CM_CTRL, CPU::in32(CM_CTRL) | (1 << 3)); 
    }
    static void poweroff() {  for(;;); }

    static void init(); 

    static void smp_barrier() {};
    static void smp_init(unsigned int) {};
private:

};

__END_SYS

#include "flash.h"
#include "info.h"
#include "uart.h"
#include "rtc.h"
#include "timer.h"
#include "adc.h"
#include "nic.h"

#endif
