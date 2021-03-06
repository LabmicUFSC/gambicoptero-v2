// EPOS AXI4LITE_IC Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>

__BEGIN_SYS

// Class initialization
void AXI4LITE_IC::init()
{
    db<AXI4LITE_IC>(TRC) << "AXI4LITE_IC::init()\n";

    MIPS32::int_disable();
    disable(); //Disable all interrupts

	_regs->edge_enable = EDGE_ENABLE;
    _regs->polarity = POLARITY; 
	// Clear all pending INTs
	_regs->pending = ~0;
	
	// Set all interrupt handlers to "No Handler"
    for(unsigned int i = 0; i < INTS; i++)
        int_vector(i, int_no_handler);
		
	CPU::Reg32 address;
    ASMV("la    %0, interrupt_service_routine" : "=r" (address) : );
    db<AXI4LITE>(TRC) << "Address of interrupt dispatcher is " << (void*)address << "\n";
	
	CPU::Reg32 instruction;
    // 1st instruction: or $26, $0, $0  --> 00 00 D0 24
    instruction = 0x0000D024;
    *(unsigned int*)(CPU_JMP_ADDRESS + 0x00)= (unsigned int)instruction;
    // 2nd instruction: lui $26, hi parte of address  --> 3C 1A cte cte
    instruction = 0x3C1A0000 | (address >> 16);
    *(unsigned int*)(CPU_JMP_ADDRESS + 0x04) = (unsigned int)instruction;
    //3th instruction: ori $26, $26, lo part of address  --> 37 5A cte cte
    instruction = 0x375A0000 | (address & 0x0000FFFF);
    *(unsigned int*)(CPU_JMP_ADDRESS + 0x08) = (unsigned int)instruction;
    //4th instruction: jr $26  --> 03 40 00 08
    instruction = 0x03400008;
    *(unsigned int*)(CPU_JMP_ADDRESS + 0x0C) = (unsigned int)instruction;
    //5th instruction: nop  --> 00 00 00 00 (Delay branch)
    instruction = 0x00000000;
    *(unsigned int*)(CPU_JMP_ADDRESS + 0x10) = (unsigned int)instruction;

    MIPS32::int_enable();
}

__END_SYS
