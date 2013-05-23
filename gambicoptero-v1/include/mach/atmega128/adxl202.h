// EPOS ADXL202 Accelerometer Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __adxl202_h
#define __adxl202_h

#include <adc.h>

__BEGIN_SYS

class ADXL202
{
protected:
    typedef IO_Map<Machine> IO;
    typedef AVR8::Reg8 Reg8;

    static const unsigned long CLOCK = Traits<Machine>::CLOCK;

    enum {
	CHANNEL_X = 3,
	CHANNEL_Y = 4,
	PORT     = IO::PORTC,
	DDR      = IO::DDRC,
	ENABLE   = 0x10,
    };

    static const int G = 1000; // mg	

    static const int DEFAULT_ADC_1G_X       = 542;
    static const int DEFAULT_ADC_MINUS1G_X  = 435;
    static const int DEFAULT_ADC_0G_X       = 489;
    static const int DEFAULT_SENSITIVITY_X  = (G * 2)/(DEFAULT_ADC_1G_X - DEFAULT_ADC_MINUS1G_X);

    static const int DEFAULT_ADC_1G_Y       = 539;
    static const int DEFAULT_ADC_MINUS1G_Y  = 427;
    static const int DEFAULT_ADC_0G_Y       = 482;
    static const int DEFAULT_SENSITIVITY_Y  = (G * 2)/(DEFAULT_ADC_1G_Y - DEFAULT_ADC_MINUS1G_Y);


public:

    ADXL202(int unit = 0) : _adc_x(CHANNEL_X, CLOCK >> 7) , _adc_y(CHANNEL_Y, CLOCK >> 7) ,
			    _mean_x(DEFAULT_ADC_0G_X) , _sensitivity_x(DEFAULT_SENSITIVITY_X) ,
			    _mean_y(DEFAULT_ADC_0G_Y) , _sensitivity_y(DEFAULT_SENSITIVITY_Y) {}


    bool enable_y() {
		enable_sensor();
		return _adc_y.power(Traits<ADC>::FULL);
    }
    
    void disable_y() { 
		enable_sensor();
		_adc_y.power(Traits<ADC>::OFF);
    }


    int sample_y() { 
	enable_y();
	for(unsigned int i = 0; i < 0xffff; i++);
	int result = (get_y() - _mean_y) * _sensitivity_y;
	disable_y();
	return result;
    }

    int get_y() { return _adc_y.get(); }

    bool data_ready_y() { return _adc_y.finished(); }

    bool enable_x() {
	enable_sensor();
	return _adc_x.power(Traits<ADC>::FULL);
    }
    
    void disable_x() { 
	enable_sensor();
	_adc_x.power(Traits<ADC>::OFF);
    }


    int sample_x() { 
	enable_x();
	for(unsigned int i = 0; i < 0xffff; i++);
	int result = (get_x() - _mean_x) * _sensitivity_x;
	disable_x();
	return result;
    }

    int get_x() { return _adc_x.get(); }

    bool data_ready_x() { return _adc_x.finished(); }

    void calibrate() {

	int adc_1g, adc_minus_1g;

	//CPU::out8(Machine::IO::DDRA, 0xFF);

	// X
	enable_x();

	// Sensor (X,Y) = (0,0)
	
	//CPU::out8(Machine::IO::PORTA, ~1);

	for(unsigned int i = 0; i < 0xffff; i++)
	    for(unsigned char j = 0; j < 0xff; j++);

	// Read 0g 
	_mean_x = get_x();

	// Sensor (X,Y) = (1,0)
	
	//CPU::out8(Machine::IO::PORTA, ~2);

	for(unsigned int i = 0; i < 0xffff; i++)
	    for(unsigned char j = 0; j < 0xff; j++);	

	// Read 0g 	
	adc_1g = get_x();

	// Sensor (X,Y) = (-1,0)
	
	//CPU::out8(Machine::IO::PORTA, ~4);

	for(unsigned int i = 0; i < 0xffff; i++)
	    for(unsigned char j = 0; j < 0xff; j++);

	adc_minus_1g = get_x();

	// Calculate Sensitivity

	_sensitivity_x = (G * 2)/(adc_1g - adc_minus_1g);

	disable_x();



    }



private:

    static Reg8 port(){ return AVR8::in8(PORT); }
    static void port(Reg8 value){ AVR8::out8(PORT,value); }   
    static Reg8 ddr(){ return AVR8::in8(DDR); }
    static void ddr(Reg8 value){ AVR8::out8(DDR,value); }  

    static void enable_sensor() {
	ddr(ddr() | ENABLE);
	port(port() | ENABLE);
    }

    static void disable_sensor() {
	port(port() & ~ENABLE);
	ddr(ddr() & ~ENABLE);
    }

    ADC _adc_x;
    ADC _adc_y;

    int _mean_x;
    int _sensitivity_x;

    int _mean_y;
    int _sensitivity_y;




    
};





__END_SYS





#endif
