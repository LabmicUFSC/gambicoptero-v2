// EPOS CC1000 Radio Tranceiver Mediator

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __cc1000_h
#define __cc1000_h

#include <nic.h>
#include <spi.h>

__BEGIN_SYS

class CC1000
{
private:
    typedef IO_Map<Machine> IO;

    typedef unsigned char Mask;
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;
    typedef CPU::Reg32 Reg32;
    typedef CPU::Reg64 Reg64;

    enum STATE {
	IDLE,
	SYNC,
	RECV
    };

public:
    class Registers
    {
    private:

	// CC1000 Microcontroler Interface Registers
	enum {
	    // These values will be traits
	    CONFIG_PORT     = IO::PORTD, 
	    CONFIG_PORT_DIR = IO::DDRD,
	    CONFIG_PORT_PIN = IO::PIND
	};

	// CC1000 Microcontroler Interface Register Bits
	enum {
	    PDATA       = 0x80,
	    PCLK        = 0x40,
	    PALE        = 0x10
	};

	// CC1000 Register Addresses
	enum Register {
	    MAIN        = 0x00,
	    FREQ_2A     = 0x01,
	    FREQ_1A     = 0x02,
	    FREQ_0A     = 0x03,
	    FREQ_2B     = 0x04,
	    FREQ_1B     = 0x05,
	    FREQ_0B     = 0x06,
	    FSEP1       = 0x07,
	    FSEP0       = 0x08,
	    CURRENT     = 0x09,
	    FRONT_END   = 0x0A,
	    PA_POW      = 0x0B,
	    PLL         = 0x0C,
	    LOCK        = 0x0D,
	    CAL         = 0x0E,
	    MODEM2      = 0x0F,
	    MODEM1      = 0x10,
	    MODEM0      = 0x11,
	    MATCH       = 0x12,
	    FSCTRL      = 0x13,
	    PRESCALER   = 0x1C,
	    TEST6       = 0x40,
	    TEST5       = 0x41,
	    TEST4       = 0x42,
	    TEST3       = 0x43,
	    TEST2       = 0x44,
	    TEST1       = 0x45,
	    TEST0       = 0x46
	};

    public:

	// CC1000 Register Bit Definitions
	enum Register_Bits {
	    // MAIN
	    RXTX	  = 0x80,
	    F_REG         = 0x40,
	    RX_PD	  = 0x20,
	    TX_PD	  = 0x10,
	    FS_PD	  = 0x08,
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

	enum Register_Parameters{

	};

	enum Default_Register_Parameters {
	    // These will be traits
	    MAIN_DEFAULT       = RX_PD | TX_PD | FS_PD | BIAS_PD,
	    // 914.9988 MHz 
	    FREQ_A_DEFAULT     = 0x7C0000,
	    FREQ_B_DEFAULT     = 0x7BF9AE,
	    FSEP_DEFAULT       = 0x0238,
	    CURRENT_RX_DEFAULT = 0x8C,     // VCO = 1450 uA, LO = 2mA,   PA = 4mA
	    CURRENT_TX_DEFAULT = 0xFD,     // VCO = 2550 uA, LO = 0.5mA, PA = 4mA(1?)
	    FRONT_END_DEFAULT  = 0x32,     // BUF_CURR = 690uA, RSSI Act,LNA_CURR = 1.8mA 
	    PA_POW_DEFAULT     = 0xFF,
	    PLL_DEFAULT        = 0x40,     // Divide by 8
	    LOCK_DEFAULT       = 0x10,     // Manchester Violation
	    CAL_DEFAULT        = CAL_START | CAL_WAIT | 0x06,
	    MODEM_DEFAULT      = 0xA16F55, // 38.4kBaud, Manchester, 14.74Mhz
	    MATCH_DEFAULT      = 0x10,
	    FSCTRL_DEFAULT     = 0x01,
	    PRESCALER_DEFAULT  = 0x00,
	    TEST_DEFAULT_H     = 0x00003F,
	    TEST_DEFAULT_L     = 0x00000000
	};

    private:

	// Microcontroler Interface Methods

	static Reg8 port(){ return CPU::in8(CONFIG_PORT); }
	static void port(Reg8 value){ CPU::out8(CONFIG_PORT,value); }
	static Reg8 ddr(){ return CPU::in8(CONFIG_PORT_DIR); }
	static void ddr(Reg8 value){ CPU::out8(CONFIG_PORT_DIR,value); }
	static Reg8 pin(){ return CPU::in8(CONFIG_PORT_PIN); }
	static void set(Mask mask){ port(port() | (Reg8)mask); }
	static void unset(Mask mask){ port(port() & ~(Reg8)mask); }
	static void set_dir(Mask mask){ ddr(ddr() | (Reg8)mask); }
	static void unset_dir(Mask mask){ ddr(ddr() & ~(Reg8)mask); }  

	// CC1000 Control Interface Methods

	static void volatile send_byte(unsigned char data){
	    for(int i = 0; i < 8; i++) {
		if(data & 0x80)
		    set(PDATA);
		else
		    unset(PDATA);
		// Cycle PCLK to write the bit
		unset(PCLK);
		set(PCLK);
		data <<= 1;
	    }
	}

	static Reg8 volatile receive_byte(){
	    unset_dir(PDATA); 
	    unsigned char data = 0;
	    for (int i = 0; i < 8; i++) { // msb first
		unset(PCLK);
		if(pin() & PDATA)
		    data = (data<<1)|0x01;
		else
		    data = (data<<1)&0xfe;
		set(PCLK);
	    }
	    return data;
	}

	static void volatile select_address(Register addr, bool write = false){
	    unset(PALE);  // Program Address Latch Enable
	    send_byte(((Reg8)addr<<1) | write);
	    set(PALE);
	}

	static Reg8 read(Register addr){
	    CPU::int_disable();
	    set_dir(PALE | PCLK | PDATA);
	    select_address(addr);
	    unsigned char data = receive_byte();
	    CPU::int_enable();
	    return data;
	}

	static void write(Register addr, Reg8 data){
	    CPU::int_disable();
	    set_dir(PALE | PCLK | PDATA);
	    select_address(addr,true);
	    send_byte(data);
	    CPU::int_enable();
	}

    public:


	// CC1000 Register Access Methods

	static Reg8 main(){ return read(MAIN); }
	static void main(Reg8 value){ write(MAIN,value); }
	static Reg32 freq_a(){
	    Reg32 value = 0;
	    value |= ((Reg32)read(FREQ_2A))<<16;
	    value |= ((Reg32)read(FREQ_1A))<<8;
	    value |= read(FREQ_0A);
	    return value;
	}
	static void freq_a(Reg32 value){
	    write(FREQ_2A,(Reg8)(value>>16));
	    write(FREQ_1A,(Reg8)(value>>8));
	    write(FREQ_0A,(Reg8)(value));
	}
	static Reg32 freq_b(){
	    Reg32 value = 0;
	    value |= ((Reg32)read(FREQ_2B))<<16;
	    value |= ((Reg32)read(FREQ_1B))<<8;
	    value |= read(FREQ_0B);
	    return value;
	}
	static void freq_b(Reg32 value){
	    write(FREQ_2B,(Reg8)(value>>16));
	    write(FREQ_1B,(Reg8)(value>>8));
	    write(FREQ_0B,(Reg8)(value));
	}
	static Reg16 fsep(){
	    Reg32 value = 0;
	    value |= ((Reg16)read(FSEP1))<<8;
	    value |= read(FSEP0);
	    return value;	
	}
	static void fsep(Reg16 value){
	    write(FSEP1,(Reg8)(value>>8));
	    write(FSEP0,(Reg8)(value));
	}
	static Reg8 current(){ return read(CURRENT); }
	static void current(Reg8 value){ write(CURRENT,value); }
	static Reg8 front_end(){ return read(FRONT_END); }
	static void front_end(Reg8 value){ write(FRONT_END,value); }
	static Reg8 pa_pow(){ return read(PA_POW); }
	static void pa_pow(Reg8 value){ write(PA_POW,value); }
	static Reg8 pll(){ return read(PLL); }
	static void pll(Reg8 value){ write(PLL,value); }
	static Reg8 lock(){ return read(LOCK); }
	static void lock(Reg8 value){ write(LOCK,value);}
	static Reg8 cal(){ return read(CAL); }
	static void cal(Reg8 value){ write(CAL,value); }
	static Reg32 modem(){
	    Reg32 value = 0;
	    value |= ((Reg32)read(MODEM2))<<16;
	    value |= ((Reg32)read(MODEM1))<<8;
	    value |= read(MODEM0);
	    return value;
	}
	static void modem(Reg32 value){
	    write(MODEM2,(Reg8)(value>>16));
	    write(MODEM1,(Reg8)(value>>8));
	    write(MODEM0,(Reg8)(value));
	}
	static Reg8 match(){ return read(MATCH); }
	static void match(Reg8 value){ write(MATCH,value); }
	static Reg8 fsctrl(){ return read(FSCTRL); }
	static void fsctrl(Reg8 value){ write(FSCTRL,value); }
	static Reg8 prescaler(){ return read(PRESCALER); }
	static void prescaler(Reg8 value){ write(PRESCALER,value); }
	static Reg64 test(){ Reg32 value = 0;
	    value |= ((Reg64)read(TEST6))<<48;
	    value |= ((Reg64)read(TEST5))<<40;
	    value |= ((Reg64)read(TEST4))<<32;
	    value |= ((Reg64)read(TEST3))<<24;
	    value |= ((Reg64)read(TEST2))<<16;
	    value |= ((Reg64)read(TEST1))<<8;
	    value |= read(TEST0);
	    return value;
	}
	static void test(Reg64 value){
	    write(TEST6,(Reg8)(value>>48));
	    write(TEST5,(Reg8)(value>>40));
	    write(TEST4,(Reg8)(value>>32));
	    write(TEST3,(Reg8)(value>>24));
	    write(TEST2,(Reg8)(value>>16));
	    write(TEST1,(Reg8)(value>>8));
	    write(TEST0,(Reg8)(value));
	}
	static Reg8 test_0(){ return read(TEST0); }
	static void test_0(Reg8 value){ write(TEST0,value); }
	static Reg8 test_1(){ return read(TEST1); }
	static void test_1(Reg8 value){ write(TEST1,value); }
	static Reg8 test_2(){ return read(TEST2); }
	static void test_2(Reg8 value){ write(TEST2,value); }
	static Reg8 test_3(){ return read(TEST3); }
	static void test_3(Reg8 value){ write(TEST3,value); }
	static Reg8 test_4(){ return read(TEST4); }
	static void test_4(Reg8 value){ write(TEST4,value); }
	static Reg8 test_5(){ return read(TEST5); }
	static void test_5(Reg8 value){ write(TEST5,value); }
	static Reg8 test_6(){ return read(TEST6); }
	static void test_6(Reg8 value){ write(TEST6,value); }

    };

    static void default_frequency(){
	Registers::freq_a(Registers::FREQ_A_DEFAULT);
	Registers::freq_b(Registers::FREQ_B_DEFAULT);
	Registers::fsep(Registers::FSEP_DEFAULT);
	Registers::current(Registers::CURRENT_RX_DEFAULT);
	Registers::front_end(Registers::FRONT_END_DEFAULT);
	Registers::pa_pow(Registers::PA_POW_DEFAULT);	
	Registers::pll(Registers::PLL_DEFAULT);
	Registers::match(Registers::MATCH_DEFAULT);	
	calibrate();
    }

    static void calibrate(){
  	Registers::pa_pow(0x00);
	Registers::test_4(0x3f);
	Registers::main(Registers::TX_PD | Registers::RESET_N);
  	Registers::cal(Registers::CAL_DEFAULT);	
	while (!(Registers::cal() & Registers::CAL_COMPLETE));
	Registers::cal(Registers::CAL_WAIT | 0x06);

	Registers::main(Registers::RXTX  | Registers::F_REG | 
			Registers::RX_PD | Registers::RESET_N);
	Registers::current(Registers::CURRENT_TX_DEFAULT);
	Registers::pa_pow(0x00);
  	Registers::cal(Registers::CAL_DEFAULT);
	while (!(Registers::cal() & Registers::CAL_COMPLETE));
	Registers::cal(Registers::CAL_WAIT | 0x06);
    }

    static void tx_mode(){
	Registers::main(Registers::RXTX  | Registers::F_REG | 
			Registers::RX_PD | Registers::RESET_N);
	Registers::current(Registers::CURRENT_TX_DEFAULT);
	// wait 250 
	for(unsigned int i = 0; i < 0xffff; i++);
	Registers::pa_pow(Registers::PA_POW_DEFAULT);
	// wait 20
	for(unsigned int i = 0; i < 0xff; i++);
    }

    static void rx_mode(){
	Registers::main(Registers::TX_PD | Registers::RESET_N);
	Registers::current(Registers::CURRENT_RX_DEFAULT);
	Registers::pa_pow(0x00);
	// wait 250 
	for(unsigned int i = 0; i < 0xffff; i++);	
    }

public:
    CC1000() { default_init(); }

    static void default_init() {
	// CC1000 Datasheet page 29.

	Registers::main(Registers::MAIN_DEFAULT);
	
	Registers::main(Registers::MAIN_DEFAULT | Registers::RESET_N);

	// delay(2000);
	for(unsigned int i = 0; i < 0xffff; i++);

	Registers::lock(Registers::LOCK_DEFAULT);
	Registers::modem(Registers::MODEM_DEFAULT);	
	Registers::fsctrl(Registers::FSCTRL_DEFAULT);

	default_frequency();
	
	//	Registers::cal(Registers::CAL_DEFAULT);
	//	Registers::prescaler(Registers::PRESCALER_DEFAULT);
	//	Registers::fsctrl(Registers::FSCTRL_DEFAULT);

    }

    char get() {
	rx_mode();

	state = IDLE;

	rx_buf_msb = rx_buf_lsb = 0;

	preamble_count = 0;

	sync_count = 0;

	while(1) {

	    unsigned char d = _spi.get();

	    switch(state) {
	    case IDLE:
		if ((d == 0xAA) || (d == 0x55)) {
		    preamble_count++;
		    if(preamble_count > 2) {
			preamble_count = sync_count = 0;
			rx_bit_offset = 0;
			state = SYNC;
		    }
		} else {
		    preamble_count = 0;
		}
		break;
	    case SYNC:
		if ((d == 0xAA) || (d == 0x55)) {
		    rx_buf_msb = d;
		}
		else {
		    switch(sync_count) {
		    case 0:
			rx_buf_lsb = d;
			break;
		    case 1:
		    case 2: {
			unsigned int tmp = (rx_buf_msb << 8) | rx_buf_lsb;
			rx_buf_msb = rx_buf_lsb;
			rx_buf_lsb = d;
			for (int i = 0; i < 8; i++) {
			    tmp <<= 1;
			    if (d & 0x80)
				tmp |= 0x01;
			    d <<= 1;
			    if (tmp == 0xCC33) {
				state = RECV;
				rx_bit_offset = 7 - i;
				break;
			    }
			}
			break;
		    }
		    default:
			preamble_count = 0;
			state = IDLE;
			break;
		    }
		    sync_count++;
		}
		break;
	    case RECV:
		rx_buf_msb = rx_buf_lsb;
		rx_buf_lsb = d;
		return (0x00ff & ((rx_buf_msb << 8) | rx_buf_lsb) >> rx_bit_offset);
	    }

	}
    }

    void put(char c) {	
	tx_mode();
	for (int i = 0; i < 5; i++)
		_spi.put(~0x55);
	_spi.put(~0xCC);
	_spi.put(~0x33);
	_spi.put(~c);
	_spi.put(~0x55);
    }


private:
    SPI _spi;

    STATE state;
    unsigned char rx_buf_lsb;
    unsigned char rx_buf_msb;
    unsigned char preamble_count;
    unsigned char rx_bit_offset;
    unsigned char sync_count;

};

__END_SYS

#endif
