// EPOS CMAC Init

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>
#include <mach/atmega128/cmac128.h>

__BEGIN_SYS

void CMAC::init(unsigned int n) {

    IC::int_vector(IC::IRQ_TIMER2_COMP, &timer_handler);

    IC::int_vector(IC::IRQ_SPI_STC, &spi_handler);

    _timer.frequency(30);

    _timer.enable();

}

 __END_SYS
