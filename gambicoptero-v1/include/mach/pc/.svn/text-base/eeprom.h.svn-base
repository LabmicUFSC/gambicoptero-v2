// EPOS PC EEPROM Mediator Common Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __pc_eeprom_h
#define __pc_eeprom_h

#include <eeprom.h>
#include "rtc.h"

__BEGIN_SYS

class PC_EEPROM: public EEPROM_Common, private MC146818
{
public:
    typedef EEPROM_Common::Address Address;

public:
    PC_EEPROM() {};

    unsigned char read(const Address & a) { return cmos_read(a); }
    void write(const Address & a, unsigned char d) { cmos_write(a, d); }

    int size() const { return cmos_size(); }
};

__END_SYS

#endif
