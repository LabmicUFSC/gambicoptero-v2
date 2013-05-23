// EPOS CRC Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __crc_h
#define __crc_h

#include <system/config.h>

__BEGIN_SYS

class CRC
{
public:

    static unsigned short crc16(char * ptr, int size) {
	unsigned short crc = 0;

	while(--size >= 0) {
	    crc ^= (unsigned short)(*ptr++) << 8;
	    int i = 8;
	    do{
		if(crc & 0x8000)
		    crc = (crc << 1) ^ 0x1021;
		else
		    crc = crc << 1;
	    } while(--i);
	}
	
	return crc;
    }

};


__END_SYS


#endif
