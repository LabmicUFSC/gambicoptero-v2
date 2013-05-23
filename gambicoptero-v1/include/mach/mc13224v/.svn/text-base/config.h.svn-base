// EPOS MC13224V Mediators Configuration

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mc13224v_config_h
#define __mc13224v_config_h

#define __CPU_H				__HEADER_ARCH(cpu)
#define __TSC_H				__HEADER_ARCH(tsc)
#define __MMU_H				__HEADER_ARCH(mmu)

#define __PCI_H				__HEADER_MACH(pci)
#define __RTC_H				__HEADER_MACH(rtc)
#define __EEPROM_H			__HEADER_MACH(eeprom)
#define __NIC_H				__HEADER_MACH(nic)
#define __MACH_H			__HEADER_MACH(machine)
#define __IC_H				__HEADER_MACH(ic)
#define __TIMER_H			__HEADER_MACH(timer)
#define __UART_H			__HEADER_MACH(uart)
#define __FLASH_H			__HEADER_MACH(flash)
#define __ADC_H             __HEADER_MACH(adc)
#define __BATTERY_H         __HEADER_MACH(battery)
#define __SENSOR_H          __HEADER_MACH(sensor)

__BEGIN_SYS

typedef ARM7		CPU;
typedef ARM7_MMU	MMU;
typedef ARM7_TSC	TSC;

typedef MC13224V		    	Machine;
typedef MC13224V_IC		    	IC;
typedef MC13224V_Flash 			Flash;
typedef MC13224V_Timer_0		Alarm_Timer;
typedef MC13224V_Timer_1		Scheduler_Timer;
typedef MC13224V_Timer_2    	Timer_CMAC;
typedef MC13224V_Timer_3        TSC_Timer;
typedef Alarm_Timer		    	Timer;
typedef MC13224V_UART			UART;
typedef Serial_Display			Display;
typedef MC13224V_NIC			NIC;
typedef MC13224V_ADC			ADC;
typedef MC13224V_Battery_ADC	Battery_ADC;
typedef MC13224V_RTC        	RTC;
typedef MC13224V_Battery    	Battery;
typedef MC13224V_Temperature_Sensor   Temperature_Sensor;
typedef MC13224V_Accelerometer Accelerometer;


__END_SYS

#endif

