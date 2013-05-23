// EPOS Memory Segment Abstraction Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __segment_h
#define __segment_h

#include <mmu.h>

__BEGIN_SYS

class Segment: public MMU::Chunk
{
private:
    typedef MMU::Chunk Chunk;

public:
    typedef MMU::Flags Flags;
    typedef CPU::Phy_Addr Phy_Addr;

public:
    Segment(unsigned int bytes, Flags flags = Flags::APP)
	: Chunk(bytes, flags)
    {
	    db<Segment>(TRC) << "Segment(bytes=" << bytes 
			     << ",flags=" << flags 
			     << ") [Chunk::_pt=" << Chunk::pt() << "]\n";
    }
    Segment(Phy_Addr phy_addr, unsigned int bytes, Flags flags = Flags::APP)
	: Chunk(phy_addr, bytes, flags | Flags::IO)
    // The MMU::IO flag signalizes the MMU that the attached memory shall
    // not be released when the chunk is deleted
    {
	db<Segment>(TRC) << "Segment(bytes=" << bytes 
			 << ",phy_addr=" << phy_addr
			 << ",flags=" << flags
			 << ") [Chunk::_pt=" << Chunk::pt() << "]\n";
    }
    ~Segment() {
	db<Segment>(TRC) << "~Segment() [Chunk::_pt=" << Chunk::pt() << "]\n";
    }

    unsigned int size() const { return Chunk::size(); }
    Phy_Addr phy_address() const { return Chunk::phy_address(); }

    int resize(int amount) {
	db<Segment>(TRC) << "Segment::resize(amount=" << amount << ")\n";

	return Chunk::resize(amount);
    }
};

__END_SYS

#endif
