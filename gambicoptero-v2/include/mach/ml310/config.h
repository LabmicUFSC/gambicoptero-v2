// EPOS ML310 Mediators Configuration

#ifndef __ml310_config_h
#define __ml310_config_h

#define __CPU_H				__HEADER_ARCH(cpu)
#define __FPU_H				__HEADER_ARCH(fpu)
#define __TSC_H				__HEADER_ARCH(tsc)
#define __MMU_H				__HEADER_ARCH(mmu)

#define __MACH_H			__HEADER_MACH(machine)
#define __IC_H				__HEADER_MACH(ic)
#define __TIMER_H			__HEADER_MACH(timer)
#define __UART_H			__HEADER_MACH(uart)
#define __RTC_H				__HEADER_MACH(rtc)
#define __NIC_H				__HEADER_MACH(nic)

__BEGIN_SYS

typedef PPC32				CPU;
typedef PPC32_FPU			FPU;
typedef PPC32_MMU			MMU;
typedef PPC32_TSC			TSC;

typedef ML310				Machine;
typedef ML310_PCI			PCI;
typedef ML310_IC			IC;
typedef ML310_Timer			Timer;
typedef ML310_RTC			RTC;
typedef ML310_UART	 		UART;
typedef Serial_Display			Display;
typedef ML310_NIC			NIC;

__END_SYS

#endif
