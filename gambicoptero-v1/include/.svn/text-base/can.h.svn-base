// EPOS CAN Mediator Common Package

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __can_h
#define __can_h

#include <system/config.h>
#include <system.h>
//#include <mutex.h>

__BEGIN_SYS

class CAN_Common
{
protected:
    CAN_Common() {}
    
public:

    enum
    {
        //can configuration
        MAX_MOBS      = 15, // AVR message objects 0..14
        REGISTER_MOBS = 8,  // mobs per register(CANIE/CANEN)
        MAX_DATASIZE  = 8,  // Message data size(bytes)
        
        //mob status return codes
        MOB_TX_COMPLETED = 0,
        MOB_RX_COMPLETED = 1,
        MOB_BUSY         = 2,
        MOB_DISABLED     = 3,
        
        //mob rx/rx_buffer types
        MOB_COMPARE_NO_BITS  = 0,
        MOB_COMPARE_IDE_BIT  = 1,
        MOB_COMPARE_RTR_BIT  = 2,
        MOB_COMPARE_ALL_BITS = 3,
    
        //mob error return codes(power of 2)
        MOB_NO_ERROR    = 0,
        MOB_ERROR_DLC   = 1,
        MOB_ERROR_BIT   = 2,
        MOB_ERROR_STUFF = 4,
        MOB_ERROR_CRC   = 8,
        MOB_ERROR_FORM  = 16,
        MOB_ERROR_ACK   = 32,
        
        //general function return codes
        OK              = -1,
        ERROR           = -2,
        NO_DATA         = -3,
        NOT_FREE        = -4,
        INVALID_ID      = -5,
        INVALID_SIZE    = -6,
        INVALID_INDEX   = -7,
        NO_PRIORITY_MOB = -8,
        
        //canstatus return codes, must be power of 2(can return more than one at a time)
        CAN_OVERLOAD_TX = 1,   // sending overload frame
        CAN_TXING       = 2,   // tx busy
        CAN_RXING       = 4,   // rx busy
        CAN_BUSOFF      = 8,   // bus off mode
        CAN_ENABLED     = 16,  // can enabled
        CAN_ERROR_PASV  = 32   // error passive mode
    };
};

__END_SYS

#ifdef __CAN_H
#include __HEADER_MACH(can)
#endif

#endif
