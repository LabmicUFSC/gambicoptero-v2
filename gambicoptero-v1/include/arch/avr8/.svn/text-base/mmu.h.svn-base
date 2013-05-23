// EPOS AVR8 MMU Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __avr8_mmu_h
#define __avr8_mmu_h

#include <system/memory_map.h>
#include <utility/string.h>
#include <utility/list.h>
#include <utility/debug.h>
#include <cpu.h>
#include <mmu.h>

__BEGIN_SYS

class AVR8_MMU: public MMU_Common<0, 0, 0>
{
private:
    typedef Simple_Grouping_List<unsigned char> List;

    static const unsigned int PHY_MEM = Memory_Map<Machine>::PHY_MEM;

public:
    //Dummy DMA_Buffer (AVR CPU's does not make DMA)
    class DMA_Buffer {};

    // Page Flags
    typedef MMU_Common<0, 0, 0>::Flags AVR8_Flags;

    // Page_Table
    class Page_Table {};

    // Chunk (for Segment)
    class Chunk
    {
    public:
        Chunk() {}
        Chunk(unsigned int bytes, Flags flags)
	    : _phy_addr(alloc(bytes)), _bytes(bytes), _flags(flags) {}
	Chunk(Phy_Addr phy_addr, unsigned int bytes, Flags flags)
	    : _phy_addr(phy_addr), _bytes(bytes), _flags(flags) {}
	~Chunk() { free(_phy_addr, _bytes); }

	unsigned int pts() const { return 0; }
	Flags flags() const { return _flags; }
	Page_Table * pt() const { return 0; }
	unsigned int size() const { return _bytes; }
	Phy_Addr phy_address() const { return _phy_addr; } // always CT

	int resize(unsigned int amount) { return 0; } // no resize with CT

    private:
        Phy_Addr _phy_addr;
        unsigned int _bytes;
        Flags _flags;
    };

    // Page Directory
    typedef Page_Table Page_Directory;

    // Directory (for Address_Space)
    class Directory 
    {
    public:
	Directory() {}
	Directory(Page_Directory * pd) {}
	~Directory() {}
	
	Page_Table * pd() const { return 0; }

	void activate() { }

	Log_Addr attach(const Chunk & chunk) { return chunk.phy_address(); }
	Log_Addr attach(const Chunk & chunk, Log_Addr addr) {
	    return (addr == chunk.phy_address())? addr : Log_Addr(false);
	}
 	void detach(const Chunk & chunk) {}
 	void detach(const Chunk & chunk, Log_Addr addr) {}

	Phy_Addr physical(Log_Addr addr) { return addr;	}
    };

public:
    AVR8_MMU() {}

    static void flush_tlb() {}
    static void flush_tlb(Log_Addr addr) {}

    static Phy_Addr alloc(unsigned int bytes = 1);
    static Phy_Addr calloc(unsigned int bytes = 1) {
	Phy_Addr phy = alloc(bytes);
	memset(phy, bytes, 0);
	return phy;	
    }
    static void free(Phy_Addr addr, int n = 1);

    static Page_Directory * volatile current() {
	return reinterpret_cast<Page_Directory * volatile>(CPU::pdp());
    }

    static Phy_Addr physical(Log_Addr addr) { return addr; }

    static void init();

private:
    static List _free;
};

__END_SYS

#endif
