// EPOS ATMega16 Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega16_h
#define __atmega16_h

#include <cpu.h>
#include <mmu.h>
#include <tsc.h>
#include "../avr_common/machine.h"
#include "memory_map.h"

__BEGIN_SYS

class ATMega16: public Machine_Common, private AVR_Machine
{
public:
    typedef IO_Map<ATMega16> IO;

public:
    template<typename Dev>
    static Dev * seize(const Type_Id & type, unsigned int unit) {
        //not implemented
	return 0;
    }

    static void release(const Type_Id & type, unsigned int unit) {
        //not implemented
    }

    static void panic() { AVR_Machine::panic(); }
    static void reboot() { CPU::reboot(); }
    static void poweroff();

    static unsigned int n_cpus() { return 0; }
    static unsigned int cpu_id() { return 0; }
    static void smp_init(unsigned int n_cpus) { }
    static void smp_barrier(int n_cpus = 0) { }

    static int irq2int(int i) { return i; }
    static int int2irq(int i) { return i; }

    static void init();
};

__END_SYS

#include "ic.h"
#include "timer.h"
#include "rtc.h"
#include "eeprom.h"
#include "uart.h"
#include "spi.h"
#include "nic.h"
#include "adc.h"

#endif
