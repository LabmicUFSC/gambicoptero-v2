// EPOS AXI4LITE_IC Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>
#include <ic.h>

__BEGIN_SYS

// Class attributes
AXI4LITE_IC::MM_Reg * AXI4LITE_IC::_regs = reinterpret_cast<AXI4LITE_IC::MM_Reg *>(AXI4LITE_IC::BASE_ADDRESS);
AXI4LITE_IC::Interrupt_Handler AXI4LITE_IC::_int_vector[AXI4LITE_IC::INTS];

void AXI4LITE_IC::int_no_handler(Interrupt_Id interrupt) {
	db<AXI4LITE_IC>(WRN) << "Intr " << interrupt << " occurred but had no handler associated !\n";
}

void AXI4LITE_IC::int_handler(Interrupt_Id i) {
	db<AXI4LITE_IC>(TRC) << "\n\n$AXI4LITE_IC::int_handler$\n\n";
	
	// Check pending and not masked interrupts
    unsigned int int_pending = _regs->pending & ~_regs->mask;

    // handle the first one set
    int int_no, int_bit;
    for (int_no=0, int_bit=1; int_no < INTS; int_no++, int_bit <<= 1){
      if (int_pending & int_bit){
	    //Ack Interrupt
	    _regs->pending = int_bit; 
        //dispatch specific handler
		void (*h)(unsigned int);
	    h = AXI4LITE_IC::int_vector(int_no);
	    h(int_no);
	    if (Traits<AXI4LITE_IC>::ATTEND_ONLY_ONE) {
			CPU::int_enable(); //Check why this is necessary !
			return;
		} 
      }
    }

    CPU::int_enable(); //Check why this is necessary !
	
}

__END_SYS
