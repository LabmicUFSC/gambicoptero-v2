#include <display.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <uart.h>
#include <adc.h>
#include <cpu.h>
#include <machine.h>
#include <thread.h>
#include <alarm.h>
#include <timer.h>

__USING_SYS

int main() {
    unsigned char count = 0;

    CPU::out8(Machine::IO::DDRA, 0xff);
    CPU::out8(Machine::IO::PORTA, ~0);

    while(1) {
	CPU::out8(Machine::IO::PORTA, ~count++);
	Alarm::delay(1000000);
    }
}
