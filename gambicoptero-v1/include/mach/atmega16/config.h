// EPOS ATMega16 Mediators Configuration

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega16_config_h
#define __atmega16_config_h

#define __CPU_H				__HEADER_ARCH(cpu)
#define __TSC_H				__HEADER_ARCH(tsc)
#define __MMU_H				__HEADER_ARCH(mmu)

#define __MACH_H			__HEADER_MACH(machine)
#define __IC_H				__HEADER_MACH(ic)
#define __TIMER_H			__HEADER_MACH(timer)
#define __UART_H			__HEADER_MACH(uart)
#define __RTC_H				__HEADER_MACH(rtc)
#define __EEPROM_H			__HEADER_MACH(eeprom)
#define __SPI_H				__HEADER_MACH(spi)
#define __NIC_H				__HEADER_MACH(nic)
#define __ADC_H				__HEADER_MACH(adc)

__BEGIN_SYS

typedef AVR8				CPU;
typedef AVR8_MMU			MMU;
typedef AVR8_TSC			TSC;

typedef ATMega16			Machine;
typedef ATMega16_IC			IC;
typedef ATMega16_Timer_1		Scheduler_Timer;
typedef ATMega16_Timer_2    		Timer_2;
typedef ATMega16_Timer_3    		Alarm_Timer;
typedef Alarm_Timer         		Timer;
typedef ATMega16_RTC			RTC;
typedef ATMega16_EEPROM			EEPROM;
typedef ATMega16_UART			UART;
typedef ATMega16_SPI			SPI;
typedef Serial_Display			Display;
typedef ATMega16_NIC			NIC;
typedef ATMega16_ADC			ADC;

__END_SYS

#endif
