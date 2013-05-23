// EPOS CC1000 Radio Tranceiver Mediator

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __cc1000_h
#define __cc1000_h

#include "cc1000_registers.h"
#include "cc1000_parameters.h"
#include "cc1000_rssi.h"
#include <spi.h>

__BEGIN_SYS

class CC1000
{
private:
    typedef CC1000_Parameters Param;
    typedef CC1000_Registers Regs;
    typedef CC1000_RSSI RSSI;
    typedef unsigned int Microseconds;

    static const unsigned long CLOCK = Traits<Machine>::CLOCK;

public:

    CC1000() : _id(0xFF) {  }

    static void config(int frequency, int power){
	Param::select(frequency, power);
	init();
    }

    static void tx_mode(){
	Regs::main(Param::RXTX  | Param::F_REG | 
		   Param::RX_PD | Param::RESET_N);
	Regs::current(Param::current_tx());
	wait(250);
	Regs::pa_pow(Param::pa_pow());
	wait(20);
    }

    static void rx_mode(){
	Regs::main(Param::TX_PD | Param::RESET_N);
	Regs::current(Param::current_rx());
	Regs::pa_pow(0x00);
	wait(250);
    }

    static void enable() {
	Regs::main(Param::RX_PD | Param::TX_PD | 
		   Param::FS_PD | Param::BIAS_PD |
		   Param::RESET_N );
	wait(2000);
    }

    static void disable() {
	Regs::pa_pow(0x00);
	Regs::main(Param::RX_PD | Param::TX_PD | 
		   Param::FS_PD | Param::CORE_PD |
		   Param::BIAS_PD | Param::RESET_N );
    }

    void int_enable() {
	_spi.int_enable();
    }

    void int_disable() {
	_spi.int_disable();
    }

    unsigned char get() {
	return _spi.get();
    }

    void put(unsigned char c) {
	_spi.put(c);
    }

    int rssi() {
	return _rssi.sample();
    }

    unsigned char id() {
	return _id;
    }

    inline static void wait(Microseconds us) { 
	for(unsigned int i = 0; i <= us * CLOCK / 6000000; i++); 
    }
	
private:

    static void frequency() {
	Regs::freq_2a(Param::freq_2a());
	Regs::freq_1a(Param::freq_1a());
	Regs::freq_0a(Param::freq_0a());
	Regs::freq_2b(Param::freq_2b());	
	Regs::freq_1b(Param::freq_1b());
	Regs::freq_0b(Param::freq_0b());
	Regs::fsep1(Param::fsep1());
	Regs::fsep0(Param::fsep0());
	Regs::current(Param::current_rx());
	Regs::front_end(Param::front_end());
	Regs::pa_pow(Param::pa_pow());	
	Regs::pll(Param::pll());
	Regs::match(Param::match());
	
	calibrate();
    }

    static void calibrate(){

	//  CC1000 Datasheet page 26.

  	Regs::pa_pow(0x00);
	Regs::test_4(0x3f);

	Regs::main(Param::TX_PD | Param::RESET_N);

  	Regs::cal(Param::CAL_START | Param::CAL_WAIT | 0x06);

 	while (!(Regs::cal() & Param::CAL_COMPLETE));
	Regs::cal(Param::CAL_WAIT | 0x06);

	Regs::main(Param::RXTX  | Param::F_REG | 
		   Param::RX_PD | Param::RESET_N);

	Regs::current(Param::current_tx());
	Regs::pa_pow(0x00);

	Regs::cal(Param::CAL_START | Param::CAL_WAIT | 0x06);

	while (!(Regs::cal() & Param::CAL_COMPLETE));
	Regs::cal(Param::CAL_WAIT | 0x06);

    }

    static void init();
    


private:
    SPI _spi;
    RSSI _rssi;
    unsigned char _id;
    
};

__END_SYS

#endif


