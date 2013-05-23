#ifndef __ia32_traits_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __ia32_traits_h

__BEGIN_SYS

// IA32
template <> struct Traits<IA32>: public Traits<void>
{
};

template <> struct Traits<IA32_TSC>: public Traits<void>
{
};

template <> struct Traits<IA32_MMU>: public Traits<void>
{
};

__END_SYS

#endif
