// EPOS PLASMA_IC Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/plasma/machine.h>
#include <mach/plasma/ic.h>


__BEGIN_SYS

// Class attributes
PLASMA_IC::Interrupt_Handler PLASMA_IC::_int_vector[PLASMA_IC::INTS];

void PLASMA_IC::int_no_handler(Interrupt_Id interrupt) {
	db<PLASMA>(WRN) << "Intr " << interrupt << " occurred but had no handler associated !\n";
}

void PLASMA_IC::int_handler(Interrupt_Id i) {
	
    unsigned int bit = *reinterpret_cast<unsigned int*>(IC_STATUS_ADDRESS) & *reinterpret_cast<unsigned int*>(IC_MASK_ADDRESS);
    switch (bit) {
	case 1: 
	   i = 0;
	   break;
	case 2: 
	   i = 1;
	   break;
	case 4: 
	   i = 2;
	   break;
	case 8: 
	   i = 3;
	   break;
	case 16: 
	   i = 4;
	   break;
	case 32: 
	   i = 5;
	   break;
	default: 
	   i = 7;
	   break;
    }

    //dispatch specific handler
	void (*h)(unsigned int);
    h = PLASMA_IC::int_vector(i);
    h(i);
    *(unsigned int*)Traits<PLASMA_Timer>::WRITE_ADDRESS = 0x00; // Ack Interrupt

}

__END_SYS
