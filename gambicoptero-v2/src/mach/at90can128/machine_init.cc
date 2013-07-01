// EPOS AT90CAN128 Initialization

#include <machine.h>
#include <system.h>

extern "C" { void __epos_library_app_entry(void); }

__BEGIN_SYS

void AT90CAN128::init()
{
    db<Init, AT90CAN128>(TRC) << "AT90CAN128::init()\n";

    System::info()->lmm.app_entry =
    reinterpret_cast<unsigned int>(&__epos_library_app_entry);

    IC::disable();

    // Initialize the hardware
    if(Traits<AT90CAN128_CAN>::enabled)
        CAN::init();

    return;
}

__END_SYS
