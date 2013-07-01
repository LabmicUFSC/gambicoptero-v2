#include "../include/error.h"

#include <machine.h>

void Abort()
{
	System::Machine::panic();
}
