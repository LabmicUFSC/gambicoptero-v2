// EPOS Application Initializer

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/heap.h>
#include <mmu.h>
#include <machine.h>
#include <application.h>

__BEGIN_SYS

class Init_Application
{
public:
    Init_Application() {
	db<Init>(TRC) << "\nInit_Application(CPU="
		      << Machine::cpu_id() << ")\n";

	// Only the boot CPU runs INIT_APPLICATION
	Machine::smp_barrier();
	if(Machine::cpu_id() != 0)
	    return;

	// Initialize Application's heap
	db<Init>(INF) << "Initializing application's heap \n";
	Application::heap()->
	    free(MMU::alloc(MMU::pages(Traits<Machine>::APPLICATION_HEAP_SIZE)),
		 Traits<Machine>::APPLICATION_HEAP_SIZE);
	if(Traits<Heap>::priority_alloc){
	    db<Init>(INF) << "Initializing application's priority heap \n";
	    Application::priority_heap()->free(reinterpret_cast<void*>(Traits<Machine>::PRIORITY_HEAP_BASE_ADDR), Traits<Machine>::PRIORITY_HEAP_SIZE);
	    Application::priority_heap()->allocated(0);
	    Application::heap()->allocated(0);
	}
	db<Init>(INF) << "done!\n\n";
    }
};

// Global object "init_application"  must be linked to the application (not 
// to the system) and there constructed at first.
Init_Application init_application;

__END_SYS
