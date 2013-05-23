// EPOS SHT11 Sensor Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __sht11_h
#define __sht11_h

#include <tsc.h>

__BEGIN_SYS

class SHT11 {
    protected:
        typedef IO_Map<Machine> IO;
        typedef AVR8::Reg8 Reg8;

        enum {
            PORT = IO::PORTG,
            DDR  = IO::DDRG,
            PIN  = IO::PING,
            GND  = 0x01,
            DATA = 0x04,
            SCK  = 0x02
        };

        // sensor opcodes
        enum {
            MEASURE_TEMPERATURE = 0x03,
            MEASURE_HUMIDITY    = 0x05,
            WRITE_STATUS_REG    = 0x06,
            READ_STATUS_REG     = 0x07,
            RESET               = 0x1E
        };

        enum {
            NO_ACK = 0,
            ACK    = 1
        };

    protected:
        SHT11() { enable(); }

        static AVR8::Reg8 port(){ return AVR8::in8(PORT); }
        static void port(AVR8::Reg8 value){ AVR8::out8(PORT,value); }
        static AVR8::Reg8 ddr(){ return AVR8::in8(DDR); }
        static void ddr(AVR8::Reg8 value){ AVR8::out8(DDR,value); }
        static AVR8::Reg8 pin(){ return AVR8::in8(PIN); }
        static void pin(AVR8::Reg8 value){ AVR8::out8(PIN,value); }

        static void set_sck0(){ port(port() & ~SCK); }
        static void set_sck1(){ port(port() | SCK); }
        static void make_sck_output(){ ddr(ddr() | SCK); }

        static void set_data0(){ port(port() & ~DATA); }
        static void set_data1(){ port(port() | DATA); }
        static void make_data_input(){ ddr(ddr() & ~DATA); }
        static void make_data_output(){ ddr(ddr() | DATA); }

        static void pull_up(){ port(port() | DATA); }

        void connection_reset();

        unsigned char measure(unsigned int *p_value, unsigned char mode);

        unsigned char read_byte(unsigned char ack);

        unsigned char soft_reset();

        void transmission_start();

        unsigned char write_byte(unsigned char value);

    public:
        bool enable() {
            if (_in_use) return false;
            _in_use = true;

            ddr(ddr() & ~GND);
            port(port() & ~GND);

            ddr(ddr() | (DATA + SCK));
            port(port() | (DATA + SCK));

            soft_reset();

            return true;
        }

        void disable() {
            _in_use = false;

            ddr(ddr() & ~(DATA + SCK));
            port(port() & ~(DATA + SCK));
        }

        int sample() { return 0; }

        int get() { return 0; }

        bool data_ready() { return (pin() & DATA); }

    private:
        bool _in_use;
};

class SHT11_Temperature: public SHT11 {
    public:
        SHT11_Temperature(unsigned int unit = 0) {}
        ~SHT11_Temperature() { disable(); }

        int convert_int(int value);

        float convert_float(int value);

        int sample() {
            enable();
            unsigned int temperature;
            measure(&temperature, MEASURE_TEMPERATURE);
            disable();

            return convert_int(temperature);
        }
};

class SHT11_Humidity: public SHT11 {
    public:
        SHT11_Humidity(unsigned int unit = 0) {}
        ~SHT11_Humidity() { disable(); }

        unsigned int convert_int(int value);

        float convert_float(int value);

        int sample() {
            enable();
            unsigned int humidity;
            measure(&humidity, MEASURE_HUMIDITY);
            disable();

            return convert_int(humidity);
        }
};

__END_SYS

#endif

