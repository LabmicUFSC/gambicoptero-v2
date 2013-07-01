// EPOS System Abstraction Initialization

#include <system.h>
#include <alarm.h>
#include <task.h>

__BEGIN_SYS

void System::init()
{
    if(Traits<Alarm>::enabled)
	Alarm::init();
    if(Traits<Task>::enabled)
	Task::init();
}

__END_SYS
