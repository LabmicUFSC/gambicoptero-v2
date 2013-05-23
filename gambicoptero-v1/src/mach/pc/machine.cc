// EPOS PC Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/pc/machine.h>

__BEGIN_SYS

// Class attributes
volatile unsigned int PC::_n_cpus;

// Class methods
void PC::panic()
{
    CPU::int_disable(); 
    Display::position(24, 73);
    Display::puts("PANIC!");
    CPU::halt();
}

void PC::reboot()
{
    for(int i = 0; (i < 300) && (CPU::in8(0x64) & 0x02); i++)
	i8255::ms_delay(1);

    // Sending 0xfe to the keyboard controller port causes it to pulse
    // the reset line
    CPU::out8(0x64, 0xfe);

    for(int i = 0; (i < 300) && (CPU::in8(0x64) & 0x02); i++)
	i8255::ms_delay(1);
}

__END_SYS
