#ifndef __integrator_config_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __integrator_config_h

__BEGIN_SYS

typedef ARM7      CPU;
typedef ARM7_MMU  MMU;
typedef ARM7_TSC  TSC;

#define __CPU_H  __HEADER_ARCH(cpu)
#define __TSC_H  __HEADER_ARCH(tsc)
#define __MMU_H  __HEADER_ARCH(mmu)

#define __MACH_H     __HEADER_MACH(machine)
#define __IC_H       __HEADER_MACH(ic)
#define __TIMER_H    __HEADER_MACH(timer)
#define __UART_H     __HEADER_MACH(uart)
#define __NIC_H      __HEADER_MACH(nic)
#define __RTC_H      __HEADER_MACH(rtc)

typedef IntegratorCP          Machine;
typedef IntegratorCP_IC       IC;
typedef IntegratorCP_Timer    Timer;
typedef IntegratorCP_UART     UART;
typedef Serial_Display        Display;
typedef IntegratorCP_NIC      NIC;
typedef IntegratorCP_RTC      RTC;

class TSC_Timer;

__END_SYS

#endif
