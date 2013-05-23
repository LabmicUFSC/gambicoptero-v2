// EPOS Task Abstraction Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/elf.h>
#include <system.h>
#include <task.h>

__BEGIN_SYS

void Task::init()
{
    System_Info<Machine> * si = System::info();

    db<Init, Task>(TRC) << "Task::init()\n";

    if(si->bm.extras_offset != -1)
	db<Init>(INF) << "Task:: additional tasks at " 
		      << (void *)si->pmm.ext_base 
		      << ":" << si->pmm.ext_top << "\n";
}

__END_SYS
