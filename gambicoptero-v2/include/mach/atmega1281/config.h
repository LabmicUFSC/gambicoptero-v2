// EPOS ATMega1281 Mediators Configuration

#ifndef __atmega1281_config_h
#define __atmega1281_config_h

#define __CPU_H             __HEADER_ARCH(cpu)
#define __FPU_H				__HEADER_ARCH(fpu)
#define __TSC_H             __HEADER_ARCH(tsc)
#define __MMU_H             __HEADER_ARCH(mmu)

#define __MACH_H            __HEADER_MACH(machine)
#define __IC_H              __HEADER_MACH(ic)
#define __TIMER_H           __HEADER_MACH(timer)
#define __UART_H            __HEADER_MACH(uart)
#define __USART_H           __HEADER_MACH(usart)
#define __RTC_H             __HEADER_MACH(rtc)
#define __EEPROM_H          __HEADER_MACH(eeprom)
#define __FLASH_H           __HEADER_MACH(flash)
#define __SPI_H             __HEADER_MACH(spi)
#define __NIC_H             __HEADER_MACH(nic)
#define __ADC_H             __HEADER_MACH(../avr_common/adc)
#define __SENSOR_H          __HEADER_MACH(sensor)
#define __RADIO_H           __HEADER_MACH(radio)
#define __BATTERY_H         __HEADER_MACH(battery)

__BEGIN_SYS

typedef AVR8                CPU;
typedef AVR8_FPU			FPU;
typedef AVR8_MMU            MMU;
typedef AVR8_TSC            TSC;

typedef ATMega1281              Machine;
typedef ATMega1281_IC           IC;
typedef ATMega1281_Timer_1      Timer_CMAC;
typedef ATMega1281_Timer_2      Scheduler_Timer;
typedef ATMega1281_Timer_3      Alarm_Timer;
typedef Alarm_Timer             Timer;
typedef ATMega1281_RTC          RTC;
typedef ATMega1281_EEPROM       EEPROM;
typedef ATMega1281_Flash        Flash;
typedef ATMega1281_UART         UART;
typedef ATMega1281_USART        USART;
typedef ATMega1281_SPI          SPI;
typedef Serial_Display          Display;
typedef ATMega1281_NIC          NIC;
typedef AVR_ADC                 ADC;
typedef ATMega1281_Temperature_Sensor   Temperature_Sensor;
typedef ATMega1281_Humidity_Sensor      Humidity_Sensor;
typedef ATMega1281_Radio        Radio;
typedef ATMega1281_Battery      Battery;

__END_SYS

#endif
