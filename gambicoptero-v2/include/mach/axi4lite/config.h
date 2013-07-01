// EPOS AXI4LITE Mediators Configuration

#ifndef __axi4lite_config_h
#define __axi4lite_config_h

#define __CPU_H				__HEADER_ARCH(cpu)
#define __FPU_H				__HEADER_ARCH(fpu)
#define __TSC_H				__HEADER_ARCH(tsc)
#define __MMU_H				__HEADER_ARCH(mmu)

#define __MACH_H			__HEADER_MACH(machine)
#define __NOC_H             __HEADER_MACH(noc)
#define __IC_H				__HEADER_MACH(ic)
#define __TIMER_H			__HEADER_MACH(timer)
#define __UART_H			__HEADER_MACH(uart)
#define __RTC_H				__HEADER_MACH(rtc)
#define __NIC_H				__HEADER_MACH(nic)

__BEGIN_SYS

typedef MIPS32				CPU;
typedef MIPS32_FPU			FPU;
typedef MIPS32_MMU			MMU;
typedef MIPS32_TSC			TSC;

typedef AXI4LITE			Machine;
typedef AXI4LITE_NOC        NOC;
typedef AXI4LITE_IC			IC;
typedef AXI4LITE_Timer  	Timer;
typedef AXI4LITE_RTC		RTC;
typedef AXI4LITE_UART		UART;
typedef Serial_Display		Display;
typedef AXI4LITE_NIC		NIC;

__END_SYS

#endif
