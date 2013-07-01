// EPOS AVR8 MMU Mediator Implementation

#include <mmu.h>

__BEGIN_SYS

// Class attributes
AVR8_MMU::List AVR8_MMU::_free;

AVR8_MMU::Phy_Addr AVR8_MMU::alloc(unsigned int bytes) {
    Phy_Addr phy(false);
    if(bytes) {
        List::Element * e = _free.search_decrementing(bytes);
	if(e)
	    phy = e->object() + e->size();
	else
	    db<AVR8_MMU>(WRN) << "AVR8_MMU::alloc() failed!\n";
    }
    db<AVR8_MMU>(TRC) << "AVR8_MMU::alloc(bytes=" << bytes << ") => "
		      << (void *)phy << "\n";
    return phy;
}

void AVR8_MMU::free(Phy_Addr addr, int n) {
    db<AVR8_MMU>(TRC) << "AVR8_MMU::free(addr=" << (void *)addr 
		      << ",n=" << n << ")\n";

    if(addr && n) {
        List::Element * e = new (addr) List::Element(addr, n);
	List::Element * m1, * m2;
	_free.insert_merging(e, &m1, &m2);
    }
}

__END_SYS
