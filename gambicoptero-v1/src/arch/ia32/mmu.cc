// EPOS IA32 MMU Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <arch/ia32/mmu.h>

__BEGIN_SYS

// Class attributes
IA32_MMU::List IA32_MMU::_free;
IA32_MMU::Page_Directory * IA32_MMU::_master;

__END_SYS
