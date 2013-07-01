// EPOS CPU Affinity Scheduler Abstraction Implementation

#include <scheduler.h>

__BEGIN_SYS

// Class attributes
int Scheduling_Criteria::CPU_Affinity::_next_cpu;
int Scheduling_Criteria::PEDF::_next_cpu;
int Scheduling_Criteria::PRM::_next_cpu;

__END_SYS
