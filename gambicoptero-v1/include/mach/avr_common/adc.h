// EPOS AVR ADC Common Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <tsc.h>
#include <adc.h>
#include <traits.h>

#ifndef __avr_adc_h
#define __avr_adc_h

__BEGIN_SYS

class AVR_ADC
{
protected:
    AVR_ADC() {}

protected:
    typedef IO_Map<Machine> IO;

    typedef AVR8::Reg8 Reg8;
    typedef AVR8::Reg16 Reg16;

    typedef Traits<AVR_ADC> _Traits;

    static const unsigned long CLOCK = Traits<Machine>::CLOCK;

public:
    typedef TSC::Hertz Hertz;

    enum {
        // ADMUX
        REFS1 = 7,
        REFS0 = 6,
        ADLAR = 5,
        MUX4  = 4,
        MUX3  = 3,
        MUX2  = 2,
        MUX1  = 1,
        MUX0  = 0,
        // ADCSRA
        ADEN  = 7,
        ADSC  = 6,
        ADFR  = 5,
        ADIF  = 4,
        ADIE  = 3,
        ADPS2 = 2,
        ADPS1 = 1,
        ADPS0 = 0,
    };

public:
    AVR_ADC(unsigned char channel, unsigned char reference,
	    unsigned char trigger, Hertz frequency)
    {
    	if(!_in_use) _op_mode = _Traits::OFF;
    	config(channel, reference, trigger, frequency);
//    	if(!_in_use) power(_Traits::FULL);
    }

    ~AVR_ADC()
    {
    	if(!_in_use) power(_Traits::OFF);
		admux(0);
		adcsra(0);
    }

    void config(unsigned char channel, unsigned char reference,
		unsigned char trigger, Hertz frequency)
    {
    	_admux = (channel << MUX0) | (reference << REFS0);

    	unsigned char ps = 7;
    	while(((CLOCK >> ps) < frequency) && (ps > 0)) ps--;

    	_adcsra = (trigger << ADFR) | (ps << ADPS0);
    }

    void config(unsigned char * channel, unsigned char * reference,
		unsigned char * trigger, Hertz * frequency)
    {
		*channel = (_admux >> MUX0) & 0x07;
		*reference = (_admux >> REFS0) & 0x03;
		*trigger = (_adcsra >> ADFR) & 0x01;
		*frequency = CLOCK << ((_adcsra >> ADPS0) & 0x07);
    }

    int sample()
    {
		while (!enable());
		while (!finished());
		int result = adchl();
		disable();
		return result;
    }

    int get() { return adchl(); }

    bool finished() { return (adcsra() & (1 << ADIF)); }

    void reset();

    void int_enable() { adcsra(adcsra() | (1 << ADIE)); }
    void int_disable() { adcsra(adcsra() & ~(1 << ADIE)); }

    bool power(Traits<ADC>::Power_Modes mode)
    {
    	return do_power(this, mode);
    }

private:

    static bool do_power(AVR_ADC * target, Traits<ADC>::Power_Modes mode)
    {
//    	static bool interrupts_on = false;

    	switch(mode)
    	{
    	case _Traits::OFF:
    		if(_op_mode == _Traits::OFF) return true;
    	case _Traits::STANDBY:
    		if(_op_mode == _Traits::STANDBY)
			{
    			if(mode == _Traits::OFF) _op_mode = mode;
    			return true;
			}
//    		interrupts_on = adcsra() & (1 << ADIE);
//    		if(interrupts_on) int_disable();
    		target->disable();
    		break;

    	case _Traits::LIGHT:
    		if(_op_mode == _Traits::LIGHT) return true;
    	case _Traits::FULL:
    		if(_op_mode == _Traits::FULL)
			{
    			if(mode == _Traits::LIGHT) _op_mode = mode;
    			return true;
			}
    		if(!target->enable()) return false;
//    		if(interrupts_on) int_enable();
    		break;
    	}
    	_op_mode = mode;
    	return true;
    }

    bool enable()
    {
		if(_in_use) return false;
		_in_use = true;
		config();
		adcsra(adcsra() | (1 << ADEN) | (1 << ADSC));
		return true;
    }
    
    void disable()
    {
		_in_use = false;
		adcsra(adcsra() & ~(1 << ADEN) & ~(1 << ADSC));
    }

    static Reg8 admux(){ return AVR8::in8(IO::ADMUX); }
    static void admux(Reg8 value){ AVR8::out8(IO::ADMUX,value); }   
    static Reg8 adcsra(){ return AVR8::in8(IO::ADCSRA); }
    static void adcsra(Reg8 value){ AVR8::out8(IO::ADCSRA,value); }       
    static Reg16 adchl(){ return AVR8::in16(IO::ADCL); }
    static void adchl(Reg16 value){ AVR8::out16(IO::ADCL,value); } 

    void config()
    {
		admux(_admux);
		adcsra(_adcsra);
    }

    Reg8 _admux;
    Reg8 _adcsra;
    static bool _in_use;
	static _Traits::Power_Modes _op_mode;
};

__END_SYS

#endif
