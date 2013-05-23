// EPOS System Initializer

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>
#include <system.h>

extern "C" { void __epos_library_app_entry(void); }

__BEGIN_SYS

class Init_System
{
public:
    Init_System() {
	db<Init>(TRC) << "\nInit_System(CPU=" << Machine::cpu_id() << ")\n";

	Machine::smp_barrier();

	// Only the boot CPU runs INIT_SYSTEM fully
	if(Machine::cpu_id() != 0) {
	    // Wait until the boot CPU has initialized the machine
	    Machine::smp_barrier();
	    // For IA-32, timer is CPU-local. What about other SMPs?
	    Timer::init();
	    Machine::smp_barrier();
	    return;
	}
	
	// Initialize the processor
	db<Init>(INF) << "Initializing the CPU: \n";
	CPU::init();
	db<Init>(INF) << "done!\n\n";

	// If EPOS is a library then adjust the application entry point (that
	// was set by SETUP) based on the ELF SYSTEM+APPLICATION image
	System_Info<Machine> * si = System::info();
	if(!si->lm.has_sys)
	    si->lmm.app_entry =
		reinterpret_cast<unsigned int>(&__epos_library_app_entry);

	// Initialize System's heap
	db<Init>(INF) << "Initializing system's heap \n";
	System::heap()->
	    free(MMU::alloc(MMU::pages(Traits<Machine>::SYSTEM_HEAP_SIZE)),
		 Traits<Machine>::SYSTEM_HEAP_SIZE);
	db<Init>(INF) << "done!\n\n";

	// Initialize the machine
	db<Init>(INF) << "Initializing the machine: \n";
	Machine::init();
	db<Init>(INF) << "done!\n\n";

	Machine::smp_barrier(); // signalizes "machine ready" to other CPUs
	Machine::smp_barrier(); // wait for them to fihish Machine::init()

	// Initialize system abstractions 
	db<Init>(INF) << "Initializing system abstractions: \n";
	System::init();
	db<Init>(INF) << "done!\n\n";

	// Initialization continues at init_first
    }
};

// Global object "init_system" must be constructed first.
Init_System init_system;

__END_SYS
