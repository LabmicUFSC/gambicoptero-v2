// EPOS PC Interrupt Controller Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <cpu.h>
#include <ic.h>

__BEGIN_SYS

void PC_IC::init()
{
    db<Init, IC>(TRC) << "IC::init()\n";

    CPU::int_disable();

    // Set all IDT entries to proper int_dispatch() offsets
    IA32::IDT_Entry * idt =
	reinterpret_cast<IA32::IDT_Entry *>(Memory_Map<PC>::IDT);
    for(unsigned int i = 0; i < IA32::IDT_ENTRIES; i++)
	if(i < IC::INTS)
	    idt[i] = IA32::IDT_Entry(IA32::GDT_SYS_CODE, 
				     Log_Addr(int_dispatch) + i * 16,
				     IA32::SEG_IDT_ENTRY);
	else
	    idt[i] = IA32::IDT_Entry(IA32::GDT_SYS_CODE, 
				     Log_Addr(int_dispatch) + INTS,
				     IA32::SEG_IDT_ENTRY);
    
    // Set all interrupt handlers to int_not()
    for(unsigned int i = 0; i < INTS; i++)
 	_int_vector[i] = int_not;

    // Reset some important exception handlers
    _int_vector[IA32::EXC_PF]
	= reinterpret_cast<Interrupt_Handler>(exc_pf);
    _int_vector[IA32::EXC_DOUBLE]
	= reinterpret_cast<Interrupt_Handler>(exc_pf);
    _int_vector[IA32::EXC_GPF]
	= reinterpret_cast<Interrupt_Handler>(exc_gpf);
    _int_vector[IA32::EXC_NODEV]
	= reinterpret_cast<Interrupt_Handler>(exc_fpu);

    remap();
    disable();

    CPU::int_enable();
}

__END_SYS
