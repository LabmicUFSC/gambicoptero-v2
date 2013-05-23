// EPOS-- MC13224V Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mc13224v_h
#define __mc13224v_h

#include <machine.h>
#include <cpu.h>
#include <mmu.h>
#include <tsc.h>
#include "memory_map.h"

__BEGIN_SYS

class MC13224V: public Machine_Common
{
public:
    typedef IO_Map<MC13224V> IO;
    typedef void (int_handler)(unsigned int);

public:
    MC13224V() {}

    static void smp_init(unsigned int n_cpus) {}
    static unsigned int n_cpus() { return 1; }
    static unsigned int cpu_id() { return 0; }
    static void smp_barrier(unsigned int n_cpus = 1) { }

    static int_handler * int_vector(unsigned int i) {
    	return 0;
    }
    static void int_vector(unsigned int i, int_handler * h) {
    }

    template<typename Dev>
    static Dev * seize(const Type_Id & type, unsigned int unit) {
    	return 0;
    }

    static void release(const Type_Id & type, unsigned int unit) {
    }

    static void panic() { 
		db<MC13224V>(ERR) << "PANIC!\n";
		CPU::int_disable();
		for(;;);
	}
    static void reboot() { }
    static void poweroff();

    static int irq2int(int i) { return i; }
    static int int2irq(int i) { return i; }

    static void init();

    static unsigned int clock() { return Traits<Machine>::CLOCK; }

    static void check_flash_erase();
    static void unbrick();

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
