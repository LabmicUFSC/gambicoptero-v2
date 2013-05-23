#ifndef __cc1000_parameters_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __cc1000_parameters_h

__BEGIN_SYS

extern "C" {

    static const char /* __attribute__((__progmem__)) */ _PA_POW_TABLE[26] =  {
	0x02, 0x02, 0x03, 0x03, 0x04, 0x05, 0x05, 0x06, 0x07, 0x08,
	0x09, 0x0b, 0x0c, 0x0d, 0x0f, 0x40, 0x50, 0x50, 0x60, 0x70,
	0x80, 0x90, 0xb0, 0xc0, 0xf0, 0xff
    };

    static const char /* __attribute__((__progmem__)) */ _FREQUENCY_TABLE[1][18] =  {
	{ 0x7c, 0x00, 0x00, 0x7b, 0xf9, 0xae, 0x02, 0x38, 0x8c, 
          0xfd, 0x32, 0x40, 0x10, 0xa1, 0x6f, 0x55, 0x10, 0x01   }
    };

}

class CC1000_Parameters {

private:

    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;
    typedef CPU::Reg32 Reg32;

    enum Register {
	FREQ_2A     = 0,
	FREQ_1A     = 1,
	FREQ_0A     = 2,
	FREQ_2B     = 3,
	FREQ_1B     = 4,
	FREQ_0B     = 5,
	FSEP1       = 6,
	FSEP0       = 7,
	CURRENT_RX  = 8,
	CURRENT_TX  = 9,
	FRONT_END   = 10,
	PLL         = 11,
	LOCK        = 12,
	MODEM2      = 13,
	MODEM1      = 14,
	MODEM0      = 15,
	MATCH       = 16,
	FSCTRL      = 17,
    };

public:		


	
    // CC1000 Register Bit Definitions
    enum Register_Bits {
	// MAIN
	RXTX	      = 0x80,
	F_REG         = 0x40,
	RX_PD	      = 0x20,
	TX_PD	      = 0x10,
	FS_PD	      = 0x08,
	CORE_PD       = 0x04,
	BIAS_PD       = 0x02,
	RESET_N       = 0x01,
	// CURRENT Bits
	VCO_CURRENT_3 = 0x80,
	VCO_CURRENT_2 = 0x40,
	VCO_CURRENT_1 = 0x20,
	VCO_CURRENT_0 = 0x10,
	LO_DRIVE_1    = 0x08,
	LO_DRIVE_0    = 0x04,
	PA_DRIVE_1    = 0x02,
	PA_DRIVE_0    = 0x01,
	// FRONT_END Bits
	BUF_CURRENT   = 0x20,
	LNA_CURRENT_1 = 0x10,
	LNA_CURRENT_0 = 0x08,
	IF_RSSI_1     = 0x04,
	IF_RSSI_0     = 0x02,
	XOSC_BYPASS   = 0x01,
	// PLL Bits
	EXT_FILTER    = 0x80,
	REF_DIV_3     = 0x40,
	REF_DIV_2     = 0x20,
	REF_DIV_1     = 0x10,
	REF_DIV_0     = 0x08,
	ALARM_DISABLE = 0x04,
	ALARM_H       = 0x02,
	ALARM_L       = 0x01,
	// LOCK Bits
	LOCK_SELECT_3 = 0x80,
	LOCK_SELECT_2 = 0x40,
	LOCK_SELECT_1 = 0x20,
	LOCK_SELECT_0 = 0x10,
	PLL_LOCK_ACC  = 0x08,
	PLL_LOCK_LNG  = 0x04,
	LOCK_INSTANT  = 0x02,
	LOCK_CONT     = 0x01,
	// CAL Bits
	CAL_START     = 0x80,
	CAL_DUAL      = 0x40,
	CAL_WAIT      = 0x20,
	CAL_CURRENT   = 0x10,
	CAL_COMPLETE  = 0x08,
	CAL_ITERATE_2 = 0x04,
	CAL_ITERATE_1 = 0x02,
	CAL_ITERATE_0 = 0x01
    };

public:
    static void select(int f, int p) { 
	_freq = f; 
	_dbm = p + 20;
    }

    // CC1000 Register Access Methods

    static Reg8 freq_2a()    { return _FREQUENCY_TABLE[_freq][FREQ_2A]; }
    static Reg8 freq_1a()    { return _FREQUENCY_TABLE[_freq][FREQ_1A]; }
    static Reg8 freq_0a()    { return _FREQUENCY_TABLE[_freq][FREQ_0A]; }
    static Reg8 freq_2b()    { return _FREQUENCY_TABLE[_freq][FREQ_2B]; }
    static Reg8 freq_1b()    { return _FREQUENCY_TABLE[_freq][FREQ_1B]; }
    static Reg8 freq_0b()    { return _FREQUENCY_TABLE[_freq][FREQ_0B]; }
    static Reg8 fsep1()      { return _FREQUENCY_TABLE[_freq][FSEP1]; }
    static Reg8 fsep0()      { return _FREQUENCY_TABLE[_freq][FSEP0]; }
    static Reg8 current_rx() { return _FREQUENCY_TABLE[_freq][CURRENT_RX]; }
    static Reg8 current_tx() { return _FREQUENCY_TABLE[_freq][CURRENT_TX]; }
    static Reg8 front_end()  { return _FREQUENCY_TABLE[_freq][FRONT_END];  }
    static Reg8 pa_pow()     { return _PA_POW_TABLE[_dbm]; }
    static Reg8 pll()        { return _FREQUENCY_TABLE[_freq][PLL];    }
    static Reg8 lock()       { return _FREQUENCY_TABLE[_freq][LOCK];   }
    static Reg8 modem2()     { return _FREQUENCY_TABLE[_freq][MODEM2]; }
    static Reg8 modem1()     { return _FREQUENCY_TABLE[_freq][MODEM1]; }
    static Reg8 modem0()     { return _FREQUENCY_TABLE[_freq][MODEM0]; }
    static Reg8 match()      { return _FREQUENCY_TABLE[_freq][MATCH];  }
    static Reg8 fsctrl()     { return _FREQUENCY_TABLE[_freq][FSCTRL]; }

private:
    static int _freq;
    static int _dbm;


};



__END_SYS

#endif
