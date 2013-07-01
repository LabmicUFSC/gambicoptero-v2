// EPOS ML310 Mediator Declarations

#ifndef __ml310_h
#define __ml310_h

#include <utility/list.h>
#include <arch/ppc32/cpu.h>
#include <arch/ppc32/mmu.h>
#include <arch/ppc32/tsc.h>
#include <machine.h>
#include "ic.h"
#include "device.h"

__BEGIN_SYS

class ML310: public Machine_Common
{
private:
    static const unsigned int HARD_INT = Traits<ML310>::HARDWARE_INT_OFFSET;
    static const unsigned int INT_BASE = Traits<ML310>::INT_BASE;
    static const unsigned int INT_VECTOR_SIZE = 64;

    typedef PPC32::Reg32 Reg32;
    typedef PPC32::Log_Addr Log_Addr;
    typedef PPC32::Context Context;

public:
    typedef void (int_handler)(unsigned int);

public:
    ML310() {}

    static int_handler * int_vector(unsigned int i) {
       if(i < INT_VECTOR_SIZE) {
         return (int_handler *) *((unsigned int *)(Machine::INT_BASE + (4*i)));
       } else {
         return 0;
       }
    }

    static void int_vector(unsigned int i, int_handler * h) {
       if(i < INT_VECTOR_SIZE) {
         unsigned int * tmp = (unsigned int *)(Machine::INT_BASE + (4*i));
         *tmp = (unsigned int)h;
       }
    }

    template<typename Dev>
    static Dev * seize(const Type_Id & type, unsigned int unit) { 
        return reinterpret_cast<Dev *>(ML310_Device::seize(type, unit));
    }

    static void release(const Type_Id & type, unsigned int unit) { 
        ML310_Device::release(type, unit); 
    }

    static int irq2int(int i) { return i; }
    static int int2irq(int i) { return i; }

    static void panic();
    static void reboot();
    static void poweroff();

    static void init();

private:
    static void int_not(unsigned int interrupt);
    static void exc_program(unsigned int interrupt);

};

__END_SYS

#include "ic.h"
#include "pci.h"
#include "timer.h"
#include "rtc.h"
#include "uart.h"
#include "display.h"
#include "sensor.h"
#include "nic.h"

#endif
