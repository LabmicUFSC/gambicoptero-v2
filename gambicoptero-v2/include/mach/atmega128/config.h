// EPOS ATMega128 Mediators Configuration

#ifndef __atmega128_config_h
#define __atmega128_config_h

#define __CPU_H				__HEADER_ARCH(cpu)
#define __FPU_H				__HEADER_ARCH(fpu)
#define __TSC_H				__HEADER_ARCH(tsc)
#define __MMU_H				__HEADER_ARCH(mmu)

#define __MACH_H			__HEADER_MACH(machine)
#define __IC_H				__HEADER_MACH(ic)
#define __TIMER_H			__HEADER_MACH(timer)
#define __UART_H			__HEADER_MACH(uart)
#define __RTC_H				__HEADER_MACH(rtc)
#define __EEPROM_H			__HEADER_MACH(eeprom)
#define __FLASH_H			__HEADER_MACH(flash)
#define __SPI_H				__HEADER_MACH(spi)
#define __NIC_H				__HEADER_MACH(nic)
#define __ADC_H				__HEADER_MACH(adc)
#define __SENSOR_H			__HEADER_MACH(sensor)

__BEGIN_SYS

typedef AVR8				CPU;
typedef AVR8_FPU			FPU;
typedef AVR8_MMU			MMU;
typedef AVR8_TSC			TSC;

typedef ATMega128			Machine;
typedef ATMega128_IC			IC;
typedef ATMega128_Timer_1		Scheduler_Timer;
typedef ATMega128_Timer_2		Timer_2;
typedef ATMega128_Timer_3		Alarm_Timer;
typedef Alarm_Timer             	Timer;
typedef ATMega128_RTC			RTC;
typedef ATMega128_EEPROM		EEPROM;
typedef ATMega128_Flash			Flash;
typedef ATMega128_UART			UART;
typedef ATMega128_SPI			SPI;
typedef Serial_Display			Display;
typedef ATMega128_NIC			NIC;
typedef ATMega128_ADC			ADC;
typedef ATMega128_Temperature_Sensor	Temperature_Sensor;
typedef ATMega128_Photo_Sensor		Photo_Sensor;
typedef ATMega128_Accelerometer		Accelerometer;

__END_SYS

#endif
