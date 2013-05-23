// EPOS-- MC13224V ADC Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mc13224v_adc_h
#define __mc13224v_adc_h

#include <system/memory_map.h>
#include <tsc.h>
#include <cpu.h>
#include <adc.h>

__BEGIN_SYS

template<int unit> class MC13224V_ADC_Unit: public ADC_Common
{
protected:
    typedef IO_Map<Machine> IO;
    typedef TSC::Hertz Hertz;
    typedef ARM7::Reg8 Reg8;
    typedef ARM7::Reg16 Reg16;
    typedef ARM7::Reg32 Reg32;
    typedef Traits<ADC> _Traits;
 
    static const Hertz DEFAULT_FREQUENCY = Traits<Machine>::CLOCK>>9;
    static const int shift = ((unit+1)%2); // 0 for unit 1, and 1 for unit 2
    
    enum {
		OVERRIDE_ADC_UNIT_ON = 0x100 << shift,
		OVERRIDE_ADC_OTHER_UNIT_ON = 0x300 >> shift,
		CONTROL_MASTER_ON = 0x1,
		MODE_OVERRIDE = 0x1,
		
		BATTERY_CHANNEL = 8
	};

public:
	MC13224V_ADC_Unit(unsigned char channel = SINGLE_ENDED_ADC0, 
					  Hertz frequency = DEFAULT_FREQUENCY) {
		// Must aways produce < or = 300000Hz
        clock_divider(calculate_clock_divider(300000));
        // Usually results 1000000Hz
        prescale(calculate_prescale(1000000));
        // ON TIME must be >= 10us
        on_time(calculate_on_time(10));
        // CONVERT TIME must be >= 20us, depends on prescale aready configured
        convert_time(calculate_convert_time((double)1000000/(frequency)));
        
        // Setting the ADC to manual mode
        mode(MODE_OVERRIDE);
        
        // Turning this unit on, setting this unit channel and 
        // turning the master device on
        turn_unit_on();
        set_unit_channel(channel);
        turn_master_on();
		
		// Will wait for stabilization. I don't know if it's
		// really necessary, but we should wait >= ON TIME.
        unsigned int x = 16800;
        while(x--);
	}

	MC13224V_ADC_Unit(unsigned char channel, unsigned char reference,
		  unsigned char trigger, Hertz frequency) {
	}

	void config(unsigned char channel, unsigned char reference,
		unsigned char trigger, Hertz frequency) {
	}

	void config(unsigned char * channel, unsigned char * reference,
		unsigned char * trigger, Hertz * frequency) {
	}
    
	int sample() {
		return 0;
	}
	
	int get(){
		return ad_result();
	}

	bool finished() {
		return false;
	}

	void reset() {
	}

	bool enable() { return true; }
	void disable() {}
	
	bool power(_Traits::Power_Modes mode) {
        switch(mode){
            case _Traits::OFF:
				override(override() & ~(OVERRIDE_ADC_UNIT_ON));
				if(!(override() & OVERRIDE_ADC_OTHER_UNIT_ON)){
					control(control() & ~CONTROL_MASTER_ON);
				}
				return true;
            break;
            case _Traits::LIGHT:
            break;
            case _Traits::STANDBY:
            break;
            case _Traits::FULL:
				control(CONTROL_MASTER_ON);
				override(override() | OVERRIDE_ADC_UNIT_ON);
				//setup
				return true;
            break;
        }
        
        return false;
    }
    
    _Traits::Power_Modes power(){
		if(override() & OVERRIDE_ADC_UNIT_ON){
			return _Traits::FULL;
		}
		return _Traits::OFF;
	}

private:
	
	unsigned short calculate_clock_divider(Hertz adc_clock){
		return ((double)Traits<Machine>::CLOCK/adc_clock);
	}
	
	unsigned short calculate_prescale(Hertz adc_prescale_clock){
		return ((double)Traits<Machine>::CLOCK/adc_prescale_clock) - 1;
	}
	
	unsigned short calculate_on_time(unsigned int on_time){
		return ((double)1/(Traits<Machine>::CLOCK/(prescale()+1)))*on_time*1000000;
	}
	
	unsigned short calculate_convert_time(unsigned int convert_time){
		return ((double)1/(Traits<Machine>::CLOCK/(prescale()+1)))*convert_time*1000000;
	}

	void turn_unit_on() {
		override(override() | OVERRIDE_ADC_UNIT_ON);
	}
	
	void set_unit_channel(unsigned char channel) {
		override((override() & ~(0xF << shift*4)) | (channel << shift*4)); // clear and set
	}
	
	void turn_master_on(){
		// (control() | CONTRO_MASTER_ON) was breaking the samples
		control(CONTROL_MASTER_ON);
	}

	static Reg16 clock_divider () { 
		return ARM7::in16(IO::ADC_CLOCK_DIVIDER); 
	}
	static void clock_divider(Reg16 value) {
		ARM7::out16(IO::ADC_CLOCK_DIVIDER, value); 
	}   

	static Reg16 prescale () { 
		return ARM7::in16(IO::ADC_PRESCALE); 
	}
	static void prescale(Reg16 value) {
		ARM7::out16(IO::ADC_PRESCALE, value); 
	}   

	static Reg16 on_time () { 
		return ARM7::in16(IO::ADC_ON_TIME); 
	}
	static void on_time(Reg16 value) {
		ARM7::out16(IO::ADC_ON_TIME, value); 
	}   

	static Reg16 convert_time () { 
		return ARM7::in16(IO::ADC_CONVERT_TIME); 
	}
	static void convert_time(Reg16 value) {
		ARM7::out16(IO::ADC_CONVERT_TIME, value); 
	}   

	static Reg16 mode () { 
		return ARM7::in16(IO::ADC_MODE); 
	}
	static void mode(Reg16 value) {
		ARM7::out16(IO::ADC_MODE, value); 
	}   

	static Reg16 override () { 
		return ARM7::in16(IO::ADC_OVERRIDE); 
	}
	static void override(Reg16 value) {
		ARM7::out16(IO::ADC_OVERRIDE, value); 
	}   

	static Reg16 control () { 
		return ARM7::in16(IO::ADC_CONTROL); 
	}
	static void control(Reg16 value) {
		ARM7::out16(IO::ADC_CONTROL, value); 
	}   

	static Reg16 ad_result () { 
		if(unit == 1)
			return ARM7::in16(IO::ADC_AD1_RESULT); 
		else
			return ARM7::in16(IO::ADC_AD2_RESULT); 
	}
	static void ad_result(Reg16 value) {
		if(unit == 1)
			ARM7::out16(IO::ADC_AD1_RESULT, value);
		else
			ARM7::out16(IO::ADC_AD2_RESULT, value);
	}   
};

class MC13224V_ADC: public MC13224V_ADC_Unit<2>
{
public:
	MC13224V_ADC(unsigned char channel, Hertz frequency = MC13224V_ADC_Unit<2>::DEFAULT_FREQUENCY):
		MC13224V_ADC_Unit<2>(channel, frequency) { }	
};

class MC13224V_Battery_ADC: public MC13224V_ADC_Unit<1>
{
public:
	MC13224V_Battery_ADC(Hertz frequency): 
		MC13224V_ADC_Unit<1>(BATTERY_CHANNEL, frequency) { }
	MC13224V_Battery_ADC(): 
		MC13224V_ADC_Unit<1>(BATTERY_CHANNEL, DEFAULT_FREQUENCY) { }
};


__END_SYS

#endif


