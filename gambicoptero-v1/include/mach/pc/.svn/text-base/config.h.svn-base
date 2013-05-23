// EPOS PC Mediators Configuration

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __pc_config_h
#define __pc_config_h

#define __CPU_H				__HEADER_ARCH(cpu)
#define __TSC_H				__HEADER_ARCH(tsc)
#define __MMU_H				__HEADER_ARCH(mmu)

#define __PCI_H				__HEADER_MACH(pci)
#define __RTC_H				__HEADER_MACH(rtc)
#define __EEPROM_H			__HEADER_MACH(eeprom)
#define __NIC_H				__HEADER_MACH(nic)
#define __DISPLAY_H			__HEADER_MACH(display)
#define __MACH_H			__HEADER_MACH(machine)
#define __IC_H				__HEADER_MACH(ic)
#define __TIMER_H			__HEADER_MACH(timer)
#define __UART_H			__HEADER_MACH(uart)

__BEGIN_SYS

typedef IA32				CPU;
typedef IA32_MMU			MMU;
typedef IA32_TSC			TSC;

typedef PC				Machine;
typedef PC_PCI				PCI;
typedef PC_IC				IC;
typedef PC_Timer			Timer;
typedef PC_RTC				RTC;
typedef PC_EEPROM			EEPROM;
typedef PC_UART				UART;
typedef IF<Traits<Serial_Display>::enabled, Serial_Display, PC_Display>::Result
					Display;
typedef PC_Ethernet			NIC;

__END_SYS

#endif
