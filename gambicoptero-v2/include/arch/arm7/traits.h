#ifndef __arm7_traits_h
#define __arm7_traits_h

// Mediators - Architecture - ARM7
__BEGIN_SYS

template <> struct Traits<ARM7>: public Traits<void>
{
    static const int BUSY_WAIT_10US_LOOP_COUNT = 14; // calibrated for ARMv4@24MHz
};

template <> struct Traits<ARM7_FPU>: public Traits<void>
{
	static const bool enabled = false;
};

template <> struct Traits<ARM7_TSC>: public Traits<void>
{
};

template <> struct Traits<ARM7_MMU>: public Traits<void>
{
};

__END_SYS

#endif
