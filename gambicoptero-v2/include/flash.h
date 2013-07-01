// EPOS Flash Mediator Common Package

#ifndef __flash
#define __flash

#include <system/config.h>

__BEGIN_SYS

class Flash_Common
{
protected:
    Flash_Common() {}

public:
    typedef unsigned int Address;
};

__END_SYS

#ifdef __FLASH_H
#include __FLASH_H
#endif

#endif
