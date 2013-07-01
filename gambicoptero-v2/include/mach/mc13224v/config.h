// EPOS MC13224V Mediators Configuration

#ifndef __mc13224v_config_h
#define __mc13224v_config_h

#define __CPU_H				__HEADER_ARCH(cpu)
#define __FPU_H				__HEADER_ARCH(fpu)
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
#define __GPIO_PIN_H        __HEADER_MACH(gpio_pin)
#define __ADC_H             __HEADER_MACH(adc)
#define __BATTERY_H         __HEADER_MACH(battery)
#define __SENSOR_H          __HEADER_MACH(sensor)
#define __SPI_H             __HEADER_MACH(spi)
#define __ONE_WIRE_H         __HEADER_MACH(one_wire)
#define __I2C_H             __HEADER_MACH(i2c)

__BEGIN_SYS

typedef ARM7		CPU;
typedef ARM7_FPU	FPU;
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
typedef MC13224V_SPI            SPI;
typedef MC13224V_One_Wire       One_Wire;
typedef MC13224V_I2C            I2C;
typedef Serial_Display			Display;
typedef MC13224V_NIC			NIC;
typedef MC13224V_ADC			ADC;
typedef MC13224V_GPIO_Pin       GPIO_Pin;
typedef MC13224V_Battery_ADC	Battery_ADC;
typedef MC13224V_RTC        	RTC;
typedef MC13224V_Battery    	Battery;
typedef MC13224V_Temperature_Sensor   Temperature_Sensor;
typedef MC13224V_ADXL345        Accelerometer;


__END_SYS

#endif
