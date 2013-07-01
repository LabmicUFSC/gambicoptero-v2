#include <display.h>
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
