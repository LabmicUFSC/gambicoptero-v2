

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <mach/pc/ic.h>
#include <machine.h>

extern "C" { void _exit(int s); }

__BEGIN_SYS

// Class attributes
APIC::Log_Addr APIC::_base;
PC_IC::Interrupt_Handler PC_IC::_int_vector[PC_IC::INTS];

// Class methods
void PC_IC::int_not(unsigned int i)
{
    db<PC>(WRN) << "\nInt " << i
		<< " occurred, but no handler installed\n";
}

void PC_IC::exc_not(unsigned int i,
		    Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags)
{
    db<PC>(WRN) << "\nAn exception has occurred for which no handler"
		<< " is installed [err=" << (void *)error
		<< ",ctx={cs=" << (void *)cs
		<< ",ip=" << (void *)eip
		<< ",fl=" << (void *)eflags
		<< "}]\n";

    db<PC>(WRN) << "The running thread will now be terminated!\n";
    _exit(-1);
}

void PC_IC::exc_pf(unsigned int i,
		   Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags)
{  
    db<PC>(WRN) << "\nPage fault [address=" << (void *)CPU::cr2()
		<< ",err={";
    if(error & (1 << 0))
	db<PC>(WRN) << "P";
    if(error & (1 << 1))
	db<PC>(WRN) << "W";
    if(error & (1 << 2))
	db<PC>(WRN) << "S";
    if(error & (1 << 3))
	db<PC>(WRN) << "R";
    db<PC>(WRN) << "},ctx={cs=" << (void *)cs
		<< ",ip=" << (void *)eip
		<< ",fl=" << (void *)eflags
		<< "}]\n";

    db<PC>(WRN) << "The running thread will now be terminated!\n";
    _exit(-1);
}

void PC_IC::exc_gpf(unsigned int i,
		    Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags)
{  
    db<PC>(WRN) << "\nGeneral protection fault [err=" << (void *)error
		<< ",ctx={cs=" << (void *)cs
		<< ",ip=" << (void *)eip
		<< ",fl=" << (void *)eflags
		<< "}]\n";

    db<PC>(WRN) << "The running thread will now be terminated!\n";
    _exit(-1);
}

void PC_IC::exc_fpu(unsigned int i,
		    Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags) {  
    db<PC>(WRN) << "\nFPU fault [err=" << (void *)error
		<< ",ctx={cs=" << (void *)cs
		<< ",ip=" << (void *)eip
		<< ",fl=" << (void *)eflags
		<< "}]\n";

    db<PC>(WRN) << "The running thread will now be terminated!\n";
    _exit(-1);
}

// APIC class methods
void APIC::ipi_start(Log_Addr entry)
{
    unsigned int vector = (entry >> 12) & 0xff;

    // Broadcast STARTUP IPI to all APs excluding self twice
    write(ICR0_31, ICR_OTHERS | ICR_LEVEL | ICR_ASSERT | ICR_STARTUP | vector);
    while((read(ICR0_31) & ICR_PENDING));
    i8255::ms_delay(10); // ~ 10ms delay

    write(ICR0_31, ICR_OTHERS | ICR_LEVEL | ICR_ASSERT | ICR_STARTUP | vector);
    while((read(ICR0_31) & ICR_PENDING));

    // Give other CPUs a time to wake up (> 100ms)
    i8255::ms_delay(100);
};

__END_SYS
