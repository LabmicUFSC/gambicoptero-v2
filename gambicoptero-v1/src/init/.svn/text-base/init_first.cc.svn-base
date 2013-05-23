// EPOS First Thread Initializer

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/heap.h>
#include <mmu.h>
#include <thread.h>

__BEGIN_SYS

class Init_First
{
public:
    Init_First() {

	db<Init>(TRC) << "Init_First(CPU=" << Machine::cpu_id() << ")\n";

	Machine::smp_barrier();

	// Initialize the Thread abstraction,
	// thus creating the first application thread
	db<Init>(INF) << "Starting the application thread ...\n";
	db<Init>(INF) << "Init ends here!\n\n";
	if(Traits<Thread>::enabled)
	    Thread::init();
    }
};

// Global object "init_first" must be constructed last in the context of the
// OS, for it activates the first application thread (usually main()) 
Init_First init_first;

__END_SYS
