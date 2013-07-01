// EPOS Task Abstraction Initialization

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
