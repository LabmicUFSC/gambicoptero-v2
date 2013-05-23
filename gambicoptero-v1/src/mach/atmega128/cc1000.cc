// EPOS CC1000 Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/avr_common/cc1000/cc1000.h>

__BEGIN_SYS

int CC1000_Parameters::_freq;
int CC1000_Parameters::_dbm;

CC1000_Registers::Reg8 CC1000_Registers::read(CC1000_Registers::Register addr){
    CPU::int_disable();
    set_dir(PALE | PCLK | PDATA);
    select_address(addr);
    unsigned char data = receive_byte();
    CPU::int_enable();
    return data;
}

void CC1000_Registers::write(CC1000_Registers::Register addr, 
			     CC1000_Registers::Reg8 data){
    CPU::int_disable();
    set_dir(PALE | PCLK | PDATA);
    select_address(addr,true);
    send_byte(data);
    CPU::int_enable();
}

void volatile CC1000_Registers::send_byte(unsigned char data){
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

CC1000_Registers::Reg8 volatile CC1000_Registers::receive_byte(){
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

void CC1000::init() {

    // CC1000 Datasheet page 29.

    Regs::main(Param::RX_PD | Param::TX_PD | 
	       Param::FS_PD | Param::BIAS_PD);

    Regs::main(Param::RX_PD | Param::TX_PD | 
	       Param::FS_PD | Param::BIAS_PD |
	       Param::RESET_N );

    wait(2000);	

    Regs::pa_pow(Param::pa_pow());
    Regs::lock(Param::lock());
    Regs::modem2(Param::modem2());
    Regs::modem1(Param::modem1());
    Regs::modem0(Param::modem0());	
    Regs::fsctrl(Param::fsctrl());

    frequency();

}

__END_SYS



