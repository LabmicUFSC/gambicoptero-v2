// EPOS CMAC Init

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
