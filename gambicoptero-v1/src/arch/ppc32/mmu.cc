// EPOS PPC32 MMU Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <arch/ppc32/mmu.h>

__BEGIN_SYS

// Class attributes
PPC32_MMU::List PPC32_MMU::_free;

PPC32_MMU::Phy_Addr PPC32_MMU::alloc(unsigned int bytes) {
    Phy_Addr phy(false);
    if(bytes) {
        List::Element * e = _free.search_decrementing(bytes);
        if(e)
            phy = e->object() + e->size();
        else
            db<PPC32_MMU>(WRN) << "PPC32_MMU::alloc() failed!\n";
    }
    db<PPC32_MMU>(TRC) << "PPC32_MMU::alloc(bytes=" << bytes << ") => "
                      << (void *)phy << "\n";
    return phy;
}

void PPC32_MMU::free(Phy_Addr addr, int n) {
    db<PPC32_MMU>(TRC) << "PPC32_MMU::free(addr=" << (void *)addr 
                      << ",n=" << n << ")\n";

    if(addr && n) {
        List::Element * e = new (addr) List::Element(addr, n);
        List::Element * m1, * m2;
        _free.insert_merging(e, &m1, &m2);
    }
}

__END_SYS
