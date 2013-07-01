// EPOS AVRMCU_LOADER


#include <utility/ostream.h>
#include <display.h>
#include <uart.h>
#include <adc.h>
#include <cpu.h>
#include <machine.h>

__USING_SYS

int main() {
    unsigned int count = 0;
    CPU::out8(Machine::IO::DDRB,0xff);

    Display display;

    while(1){
      CPU::out8(Machine::IO::PORTB,~count++);
      for(unsigned int i = 0; i < 0xffff; i++);
      display.puts("EPOS\n");
    }

    return 0;

}

