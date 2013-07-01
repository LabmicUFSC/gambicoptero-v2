// EPOS ATMega128 Mediator Initialization

#include <machine.h>
#include <system.h>

extern "C" { void __epos_library_app_entry(void); }

__BEGIN_SYS

void ATMega128::init()
{
    db<Init, ATMega128>(TRC) << "ATMega128::init()\n";

    System::info()->lmm.app_entry =
	reinterpret_cast<unsigned int>(&__epos_library_app_entry);

    IC::disable();

    // Initialize the hardware
    if(Traits<ATMega128_NIC>::enabled)
	ATMega128_NIC::init();

    return;

}

__END_SYS

