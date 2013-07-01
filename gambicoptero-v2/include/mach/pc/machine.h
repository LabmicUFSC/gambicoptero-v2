// EPOS PC Mediator Declarations

#ifndef __pc_h
#define __pc_h

#include <utility/list.h>
#include <cpu.h>
#include <mmu.h>
#include <tsc.h>
#include <machine.h>
#include "info.h"
#include "memory_map.h"
#include "ic.h"

__BEGIN_SYS

class PC: public Machine_Common
{
private:
    static const bool smp = Traits<Thread>::smp;
    
    typedef IA32::Reg32 Reg32;
    typedef IA32::Log_Addr Log_Addr;

public:
    PC() {}
  
    static void panic();
    static void reboot();
    static void poweroff();

    static unsigned int n_cpus() { return smp ? _n_cpus : 1; }
    static unsigned int cpu_id() { return smp ? APIC::id() : 0; }

    static void smp_init(unsigned int n_cpus) {
	if(smp) {
	    _n_cpus = n_cpus;
	    APIC::remap(); 
	}
    };

    static void smp_barrier(int n_cpus = _n_cpus) {
	static volatile int ready[2];
	static volatile int i;

	if(smp) {
	    int j = i;

	    CPU::finc(ready[j]);

	    if(cpu_id() == 0) {
		while(ready[j] < n_cpus); // wait for all CPUs to be ready
		i = !i;                   // toggle ready
		ready[j] = 0;             // signalizes waiting CPUs
	    } else
		while(ready[j]);          // wait for CPU[0] signal
	}
    }

    static void init();

private:
    static volatile unsigned int _n_cpus;
};

__END_SYS

#include "pci.h"
#include "timer.h"
#include "rtc.h"
#include "eeprom.h"
#include "uart.h"
#include "display.h"
#include "nic.h"

#endif
