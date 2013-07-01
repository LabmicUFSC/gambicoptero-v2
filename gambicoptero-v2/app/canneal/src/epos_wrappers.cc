// EPOS wrappers implementation

#include <utility/random.h>
#include <machine.h>
#include <clock.h>
#include "../include/epos_wrappers.h"

static System::Pseudo_Random random;

void srandom(int seed)
{
    random.seed(seed);
}

void exit(int s)
{
    System::Machine::reboot();
}

time_t time ( time_t * timer )
{
    System::Clock clock;
    return clock.now();
}

clock_t clock ( void )
{
    return 0;
}
