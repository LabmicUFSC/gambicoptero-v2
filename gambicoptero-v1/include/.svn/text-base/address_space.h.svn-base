// EPOS Address_Space Abstraction Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __address_space_h
#define __address_space_h

#include <mmu.h>
#include <segment.h>

__BEGIN_SYS

class Address_Space: public MMU::Directory
{
private:
    typedef CPU::Phy_Addr Phy_Addr;
    typedef CPU::Log_Addr Log_Addr;
    typedef MMU::Directory Directory;

public:
    // For self reference constructor
    enum Self { SELF };

public:
    Address_Space() {
        db<Address_Space>(TRC) << "Address_Space() [Directory::pd=" 
                               << Directory::pd() << "]\n";
    }

    Address_Space(const Self & s)
    : Directory(reinterpret_cast<MMU::Page_Table *>(CPU::pdp())) {
        db<Address_Space>(TRC) << "Address_Space(SELF,pd=" << CPU::pdp() 
                               << ")\n";
    }

    ~Address_Space() {
        db<Address_Space>(TRC) << "~Address_Space() [Directory::pd=" 
                               << Directory::pd() << "]\n";
    }

    Log_Addr attach(const Segment & seg) {
        db<Address_Space>(TRC) << "Address_Space::attach(seg=" << &seg
                               << ")\n";

        return Directory::attach(seg);
    }

    Log_Addr attach(const Segment & seg, Log_Addr addr) {
        db<Address_Space>(TRC) << "Address_Space::attach(seg=" << &seg
                               << ",addr=" << addr << ")\n";

        return Directory::attach(seg, addr);
    }

    void detach(const Segment & seg) {
        db<Address_Space>(TRC) << "Address_Space::detach(seg=" << &seg 
                               << ")\n";

        Directory::detach(seg);
    }

    void activate() {
        db<Address_Space>(TRC) 
            << "Address_Space::activate() [Directory::pd=" 
            << Directory::pd() << "]\n";

        Directory::activate();
    }

    Phy_Addr physical(Log_Addr address) { 
        return Directory::physical(address);
    }
};

__END_SYS

#endif
