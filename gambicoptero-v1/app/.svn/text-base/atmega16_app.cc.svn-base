// EPOS AVRMCU_LOADER

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.


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

