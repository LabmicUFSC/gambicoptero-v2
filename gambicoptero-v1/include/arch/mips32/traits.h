#ifndef __mips32_traits_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __mips32_traits_h

__BEGIN_SYS

// IA32
template <> struct Traits<MIPS32>: public Traits<void>
{
};

template <> struct Traits<MIPS32_TSC>: public Traits<void>
{
	// THIS SHOULD MAP THE ADDRESS OF THE TSC COUNTER ON THE RESPECTIVELY MACHINE
	// Default values are 0x20000060 (Plasma), 0x80000800 (AXI4LITE)
	static const unsigned int ADDRESS = 0x80000800;
};

template <> struct Traits<MIPS32_MMU>: public Traits<void>
{
};

__END_SYS

#endif
