// EPOS PC PCI Mediator

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <pci.h>
#include <system.h>

__BEGIN_SYS

void PC_PCI::init()
{
    _phy_io_mem = System::info()->pmm.io_mem_base;

    db<Init, PCI>(TRC) << "PCI::init(pmm.io_mem=" << _phy_io_mem 
			  << ")\n";

    CPU::int_disable();

    CPU::out8(0xcfb, 0x01);
    Reg32 tmp = CPU::in32(CONFADDR);
    CPU::out32(CONFADDR, 0x80000000);
    if(CPU::in32(CONFADDR) != 0x80000000) {
	db<Init, PC_PCI>(WRN) << "PC_PCI::init: failed!\n";
    }
    CPU::out32(CONFADDR, tmp);

    CPU::int_enable();
}

__END_SYS
