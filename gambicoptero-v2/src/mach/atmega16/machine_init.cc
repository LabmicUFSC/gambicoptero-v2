// EPOS ATMega16 Mediator Initialization

#include <machine.h>
#include <system.h>

extern "C" { void __epos_library_app_entry(void); }

__BEGIN_SYS

void ATMega16::init()
{
    db<Init, ATMega16>(TRC) << "ATMega16::init()\n";

    System::info()->lmm.app_entry =
        reinterpret_cast<unsigned int>(&__epos_library_app_entry);

    return;

}

__END_SYS

