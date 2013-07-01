#ifndef __mips32_traits_h
#define __mips32_traits_h

__BEGIN_SYS

// IA32
template <> struct Traits<MIPS32>: public Traits<void>
{
};

template <> struct Traits<MIPS32_FPU>: public Traits<void>
{
	static const bool enabled = false;
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
