// EPOS AT90CAN128 Mediators Configuration

#ifndef __at90can128_config_h
#define __at90can128_config_h

#define __CPU_H				__HEADER_ARCH(cpu)
#define __FPU_H				__HEADER_ARCH(fpu)
#define __TSC_H				__HEADER_ARCH(tsc)
#define __MMU_H				__HEADER_ARCH(mmu)

#define __MACH_H			__HEADER_MACH(machine)
#define __IC_H				__HEADER_MACH(ic)
#define __TIMER_H			__HEADER_MACH(timer)
#define __UART_H			__HEADER_MACH(uart)
#define __RTC_H             		__HEADER_MACH(rtc)
#define __CAN_H             		__HEADER_MACH(can)

__BEGIN_SYS

typedef AVR8				CPU;
typedef AVR8_FPU			FPU;
typedef AVR8_MMU			MMU;
typedef AVR8_TSC			TSC;

typedef AT90CAN128          		Machine;
typedef AT90CAN128_IC       		IC;
typedef AT90CAN128_RTC      		RTC;
typedef AT90CAN128_Timer_1  		Scheduler_Timer;
typedef AT90CAN128_Timer_2  		Timer_2;
typedef AT90CAN128_Timer_3  		Alarm_Timer;
typedef Alarm_Timer         		Timer;
typedef AT90CAN128_CAN      		CAN;
typedef Serial_Display      		Display;
typedef AT90CAN128_UART     		UART;
typedef AT90CAN128_ADC         		ADC;

__END_SYS

#endif
