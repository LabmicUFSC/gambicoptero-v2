#ifndef __cc1000_registers_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __cc1000_registers_h

#include <cpu.h>

__BEGIN_SYS

class CC1000_Registers
{
private:

    typedef unsigned char Mask;
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;
    typedef CPU::Reg32 Reg32;

    // CC1000 Microcontroler Interface Registers
    enum {
	CONFIG_PORT     = 0x12, 
	CONFIG_PORT_DIR = 0x11,
	CONFIG_PORT_PIN = 0x10
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

    static void volatile send_byte(unsigned char data);

    static Reg8 volatile receive_byte();

    static void volatile select_address(Register addr, bool write = false){
	unset(PALE);  // Program Address Latch Enable
	send_byte(((Reg8)addr<<1) | write);
	set(PALE);
    }

    static Reg8 read(Register addr);

    static void write(Register addr, Reg8 data);

public:

    // CC1000 Register Access Methods


    static Reg8 main(){ return read(MAIN); }
    static void main(Reg8 value){ write(MAIN,value); }
    static Reg8 freq_2a(){ return read(FREQ_2A); }
    static void freq_2a(Reg8 value){ write(FREQ_2A,value); }
    static Reg8 freq_1a(){ return read(FREQ_1A); }
    static void freq_1a(Reg8 value){ write(FREQ_1A,value); }
    static Reg8 freq_0a(){ return read(FREQ_0A); }
    static void freq_0a(Reg8 value){ write(FREQ_0A,value); }
    static Reg8 freq_2b(){ return read(FREQ_2B); }
    static void freq_2b(Reg8 value){ write(FREQ_2B,value); }
    static Reg8 freq_1b(){ return read(FREQ_1B); }
    static void freq_1b(Reg8 value){ write(FREQ_1B,value); }
    static Reg8 freq_0b(){ return read(FREQ_0B); }
    static void freq_0b(Reg8 value){ write(FREQ_0B,value); }
    static Reg8 fsep1(){ return read(FSEP1); }
    static void fsep1(Reg8 value){ write(FSEP1,value); }
    static Reg8 fsep0(){ return read(FSEP0); }
    static void fsep0(Reg8 value){ write(FSEP0,value); }
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
    static Reg8 modem2(){ return read(MODEM2); }
    static void modem2(Reg8 value){ write(MODEM2,value); }
    static Reg8 modem1(){ return read(MODEM1); }
    static void modem1(Reg8 value){ write(MODEM1,value); }
    static Reg8 modem0(){ return read(MODEM0); }
    static void modem0(Reg8 value){ write(MODEM0,value); }
    static Reg8 match(){ return read(MATCH); }
    static void match(Reg8 value){ write(MATCH,value); }
    static Reg8 fsctrl(){ return read(FSCTRL); }
    static void fsctrl(Reg8 value){ write(FSCTRL,value); }
    static Reg8 prescaler(){ return read(PRESCALER); }
    static void prescaler(Reg8 value){ write(PRESCALER,value); }
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

__END_SYS

#endif
