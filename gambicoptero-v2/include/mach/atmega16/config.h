// EPOS ATMega16 Mediators Configuration

#ifndef __atmega16_config_h
#define __atmega16_config_h

#define __CPU_H             __HEADER_ARCH(cpu)
#define __FPU_H				__HEADER_ARCH(fpu)
#define __TSC_H             __HEADER_ARCH(tsc)
#define __MMU_H             __HEADER_ARCH(mmu)

#define __MACH_H            __HEADER_MACH(machine)
#define __IC_H              __HEADER_MACH(ic)
#define __TIMER_H           __HEADER_MACH(timer)
#define __UART_H            __HEADER_MACH(uart)
#define __RTC_H             __HEADER_MACH(rtc)
#define __EEPROM_H          __HEADER_MACH(eeprom)
#define __SPI_H             __HEADER_MACH(spi)
#define __ADC_H             __HEADER_MACH(../avr_common/adc)

__BEGIN_SYS

typedef AVR8                CPU;
typedef AVR8_FPU			FPU;
typedef AVR8_MMU            MMU;
typedef AVR8_TSC            TSC;

typedef ATMega16            Machine;
typedef ATMega16_IC         IC;
typedef ATMega16_Timer_1    Scheduler_Timer;
typedef ATMega16_Timer_2    Timer_2;
typedef ATMega16_Timer_3    Alarm_Timer;
typedef Alarm_Timer         Timer;
typedef ATMega16_RTC        RTC;
typedef ATMega16_EEPROM     EEPROM;
typedef ATMega16_UART       UART;
typedef ATMega16_SPI        SPI;
typedef Serial_Display      Display;
typedef AVR_ADC             ADC;

__END_SYS

#endif
