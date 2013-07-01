#ifndef __integrator_traits_h
#define __integrator_traits_h

__BEGIN_SYS

template <> struct Traits<IntegratorCP>: public Traits<void>
{
	static const unsigned int MAX_CPUS = 1;
	static const unsigned int CLOCK = 24000000;
	static const unsigned int APPLICATION_STACK_SIZE = 4096;
	static const unsigned int APPLICATION_HEAP_SIZE  = 8*1024*1024;
	static const unsigned int SYSTEM_HEAP_SIZE = APPLICATION_HEAP_SIZE / 2;

	static const unsigned int PRIORITY_HEAP_BASE_ADDR = 0;
	static const unsigned int PRIORITY_HEAP_SIZE = 4;
	static const unsigned int PRIORITY_HEAP_TOP_ADDR = PRIORITY_HEAP_BASE_ADDR + PRIORITY_HEAP_SIZE - 1;
};

template <> struct Traits<IntegratorCP_NIC>: public Traits<void>
{
    typedef LIST<IntegratorCP_NIC> NICS;
};
__END_SYS

#endif

