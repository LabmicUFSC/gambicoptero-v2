// EPOS MTS300 Sensor Board Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mts300_h
#define __mts300_h

#include <adc.h>

__BEGIN_SYS

class MTS300
{
protected:
    typedef IO_Map<Machine> IO;
    typedef AVR8::Reg8 Reg8;

    static const unsigned long CLOCK = Traits<Machine>::CLOCK;

    enum {
	CHANNEL  = 1,
	PORT     = IO::PORTE,
	DDR      = IO::DDRE,
	LIGHT_EN = 0x20,
	TEMP_EN  = 0x40,
    };

protected:
    MTS300() : _adc(CHANNEL, CLOCK >> 7) {}

public:
    bool enable() { return _adc.power(Traits<ADC>::FULL);  }
    void disable() { _adc.power(Traits<ADC>::OFF); }

    int sample() { return _adc.sample(); }

    int get() { return _adc.get(); }

    bool data_ready() { return _adc.finished(); }



    static Reg8 port(){ return AVR8::in8(PORT); }
    static void port(Reg8 value){ AVR8::out8(PORT,value); }   
    static Reg8 ddr(){ return AVR8::in8(DDR); }
    static void ddr(Reg8 value){ AVR8::out8(DDR,value); }   

private:
    ADC _adc;
};

class MTS300_Temperature: public MTS300
{
public:
    MTS300_Temperature(unsigned int unit = 0) {}
    ~MTS300_Temperature() { disable(); }

    bool enable() {
	enable_sensor();
	return MTS300::enable();
    }

    void disable() {
	disable_sensor();
	MTS300::disable();
    }

    int convert_int(int v);
    float convert_float(int v);


    int sample() { 
	enable_sensor();
	for(unsigned int i = 0; i < 0xfff; i++);
	while (!MTS300::enable());
	while (!MTS300::data_ready()); 
	int v = MTS300::get();
	MTS300::disable();
	disable_sensor();
	return convert_int(v);
    }



private:
    
    static void enable_sensor() {
	ddr(ddr() | (TEMP_EN & ~LIGHT_EN));
	port(port() | (TEMP_EN & ~LIGHT_EN));
    }

    static void disable_sensor() {
	port(port() & ~TEMP_EN);
	ddr(ddr() & ~TEMP_EN);
    }

};

class MTS300_Photo: public MTS300
{
public:
    MTS300_Photo(unsigned int unit = 0) {}
    ~MTS300_Photo() { disable(); }

    bool enable() {
	enable_sensor();
	return MTS300::enable();
    }

    void disable() {
	disable_sensor();
	MTS300::disable();
    }

    int convert_int(int v);
    float convert_float(int v);

    int sample() { 
	enable_sensor();
	for(unsigned int i = 0; i < 0xfff; i++);
	while (!MTS300::enable());
	while (!MTS300::data_ready()); 
	int v = MTS300::get();
	MTS300::disable();
	disable_sensor();
	return convert_int(v);
    }

private:
    
    static void enable_sensor() {
	ddr(ddr() | (LIGHT_EN & ~TEMP_EN));
	port(port() | (LIGHT_EN & ~TEMP_EN));
    }

    static void disable_sensor() {
	port(port() & ~LIGHT_EN);
	ddr(ddr() & ~LIGHT_EN);
    }
};

__END_SYS





#endif
