#ifndef __mc13224v_traits_h
#define __mc13224v_traits_h

#include <system/config.h>
#include <system/types.h>

// Mediators - Machine - MC13224V

__BEGIN_SYS

class MC13224V_Common;

template <> struct Traits<MC13224V_Common>: public Traits<void>
{
};

template <> struct Traits<MC13224V>: public Traits<MC13224V_Common>
{
	static const unsigned int MAX_CPUS = 1;
	static const unsigned int CLOCK = 24000000;
	static const unsigned int APPLICATION_STACK_SIZE = 1024;
	static const unsigned int APPLICATION_HEAP_SIZE = 3072;
	static const unsigned int SYSTEM_HEAP_SIZE = 2*APPLICATION_HEAP_SIZE
	           + 4096; // so flash can work
	static const bool SMP = false;

	static const unsigned int PRIORITY_HEAP_BASE_ADDR = 0;
	static const unsigned int PRIORITY_HEAP_SIZE = 4;
	static const unsigned int PRIORITY_HEAP_TOP_ADDR = PRIORITY_HEAP_BASE_ADDR + PRIORITY_HEAP_SIZE - 1;

	/*
	 * EPOSMote Versions:
     *  - EPOSMoteI Alfa:   0x1a;
     *  - EPOSMoteI Beta:   0x1b;
     *  - EPOSMoteI Final:  0x1f;
     *  - EPOSMoteII Alfa:  0x2a;
     *  - EPOSMoteII Beta:  0x2b;
     *  - EPOSMoteII Final: 0x2f;
	 */
    static const unsigned int emote_version = 0x2f;
	static const bool flash_erase_checking = true;
//    static const unsigned int flash_erase_checking_pin = 11; // EPOSMoteII-ARM7 Beta
    static const unsigned int flash_erase_checking_pin = 24; // EPOSMoteII-ARM7
};

template <> struct Traits<MC13224V_Battery>: public Traits<MC13224V_Common>
{
    static const bool enabled = true;
    static const bool buck_enabled = false;
    static const unsigned int buck_voltage_threshold = 2500; // in mV
    static const unsigned int frequency = 1; // in Hz
};

template <> struct Traits<MC13224V_IC>: public Traits<MC13224V_Common>
{
    static const bool enabled = true;
    static const bool debugged = false;
};

template <> struct Traits<MC13224V_Flash>: public Traits<MC13224V_Common>
{
    static const bool enabled = true;
};

template <> struct Traits<MC13224V_UART>: public Traits<MC13224V_Common>
{
};

template <> struct Traits<MC13224V_SPI>: public Traits<MC13224V_Common>
{
};

template <> struct Traits<MC13224V_One_Wire>: public Traits<MC13224V_Common>
{
};

template <> struct Traits<MC13224V_I2C>: public Traits<MC13224V_Common>
{
    static const bool enabled = true;
    static const bool debugged = true;

    static const bool BROADCAST = false;
    static const unsigned char FREQUENCY_DIVIDER = 0x20;
};

template <> struct Traits<MC13224V_Timer_0>: public Traits<MC13224V_Common>
{
    static const int FREQUENCY = 1000; // default system timer, in Hz
};

template <> struct Traits<MC13224V_Timer_1>: public Traits<MC13224V_Common>
{
	static const int FREQUENCY = 1000;
};

template <> struct Traits<MC13224V_Timer_2>: public Traits<MC13224V_Common>
{
	static const int FREQUENCY = 1000;
};

template <> struct Traits<MC13224V_Timer_3>: public Traits<MC13224V_Common>
{
	static const int FREQUENCY = 1000;
};

template <> struct Traits<MC13224V_RTC>: public Traits<MC13224V_Common>
{
	static const unsigned int EPOCH_DAYS = 1;
};

template <> struct Traits<MC13224V_Display>: public Traits<MC13224V_Common>
{
	static const bool on_serial = true;
};

template <> struct Traits<MC13224V_NIC>: public Traits<void>
{
    static const bool enabled = true;

    typedef LIST<MC13224V_Radio> NICS;

    static const unsigned int RADIO_UNITS = NICS::Count<MC13224V_Radio>::Result;
};

template <> struct Traits<MC13224V_ADC>: public Traits<void>
{
    enum Power_Modes
    {
        FULL        = 0,
        LIGHT       = 1,
        STANDBY     = 2,
        OFF         = 3
    };
    static const int RESOLUTION = 12; //bits
};

template <> struct Traits<MC13224V_Temperature_Sensor>: public Traits<void>
{
    typedef LIST<ERTJ1VG103FA> SENSORS;

    enum {
        KELVIN = 0,
        CELCIUS = 1
    };
    static const int UNITS = CELCIUS;
    static const int ERROR = 3.7f;
};

template <> struct Traits<MC13224V_Accelerometer>: public Traits<void> 
{
    typedef LIST<MC13224V_ADXL345> SENSORS;

};


__END_SYS

#endif

