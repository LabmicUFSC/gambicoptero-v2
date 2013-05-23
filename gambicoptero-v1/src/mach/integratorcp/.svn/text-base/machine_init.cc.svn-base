#include <mach/integratorcp/machine.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <mach/integratorcp/memory_map.h>

extern "C" int __epos_library_app_entry();

__BEGIN_SYS

// Since we dont need a setup we will build the system info here
void IntegratorCP::init()
{
    db<Init, IntegratorCP>(TRC) << "Machine::init()\n";

    System_Info<IntegratorCP> * si;
    
    si = reinterpret_cast<System_Info<IntegratorCP> *>(Memory_Map<IntegratorCP>::SYS_INFO);

    CPU::int_disable();

    // this struct should have meaning info if we change to setup-based init
    si->lm.has_stp = false;
    si->lm.has_ini = false;
    si->lm.has_sys = false;
    si->lm.has_app = true;
    si->lm.has_ext = false;

    si->lm.stp_entry = 0;
    si->lm.stp_code  = 0;
    si->lm.stp_code_size = 0;
    si->lm.stp_data = 0;
    si->lm.stp_data_size = 0;

    si->lm.ini_entry = 0;
    si->lm.ini_code  = 0;
    si->lm.ini_code_size = 0;
    si->lm.ini_data = 0;
    si->lm.ini_data_size = 0;

    si->lm.sys_entry = 0;
    si->lm.sys_code  = 0;
    si->lm.sys_code_size = 0;
    si->lm.sys_data = 0;
    si->lm.sys_data_size = 0;
    si->lm.sys_stack = 0;
    si->lm.sys_stack_size = 0;

    si->lm.app_entry = (unsigned int)&__epos_library_app_entry;
}

__END_SYS
