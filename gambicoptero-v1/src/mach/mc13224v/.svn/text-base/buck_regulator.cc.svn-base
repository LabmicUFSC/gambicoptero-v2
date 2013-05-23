// EPOS-- MC13224V Buck Regulator Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/mc13224v/buck_regulator.h>
#include <mach/mc13224v/flash.h>
#include <machine.h>
#include <cpu.h>


__BEGIN_SYS

unsigned int MC13224V_Buck_Regulator::_vreg = 0x00000f20; // default register values after start-up

__END_SYS

__USING_SYS

// TODO: No support for external 1.8V regulator have been implemented!

void MC13224V_Buck_Regulator::enable()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::enable()\n";

    crm_vreg_cntl(crm_vreg_cntl() | 0x03); // enable buck - BUCK_EN and BUCK_SYNC_REC_EN bits
    crm_sys_cntl((crm_sys_cntl() & ~0x83) | 0x81); // buck as power source
    while(!enabled());
    adjust_buck_clock();
}

void MC13224V_Buck_Regulator::disable()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::disable()\n";

    crm_sys_cntl(crm_sys_cntl() & ~0x03); // supply from Vbatt
    crm_vreg_cntl(crm_vreg_cntl() & ~0x03); // buck disabled
}

bool MC13224V_Buck_Regulator::enabled()
{
//    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::enabled()\n";

    return ((!(crm_vreg_cntl() & 0x04)) && (crm_status() & 0x00020000));
}

void MC13224V_Buck_Regulator::SPIF_1P8V_enable()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::SPIF_1P8V_enable()\n";

    crm_sys_cntl(crm_sys_cntl() | 0x8);
}

void MC13224V_Buck_Regulator::SPIF_1P8V_disable()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::SPIF_1P8V_disable()\n";

    crm_sys_cntl(crm_sys_cntl() & ~0x8);
}


// VREG_CNTL is a 32-bits register, where the 12 first bits are used, and
// remaining are reserved:
// 12-31: reserved
// 08-11: BUCK_CLKDIV[3:0], default 0xF => USED FOR VOLTAGE SCALING
//    07: VREG_1P8V_EN, default 0 = NVM regulator disabled
// 05-06: VREG_1P5V_SEL[1:0], default 0x1 = 20 mA source
// 03-04: VREG_1P5V_EN[1:0], default 0x0 = OFF
//    02: BUCK_BYPASS_EN, default 0 = OFF - no bypass
//    01: BUCK_SYNC_REC_EN, default 0 = OFF - no sync regulator
//    00: BUCK_EN, default 0 = OFF


// Divider Code Word for Buck Regulator Clock
// Refer to MC1322xRM (Rev 1.4):
//    - page 5-54, Table 5-25: description
//    - page 5-57, Table 5-28: configuration
// Basically, the divider should be such that the buck frequency stays as close
// as possible to 1.6 MHz (dividing the main clock).
// This function sets the best clock candidate based on the current system
// frequency and according to datasheet specifications. Thus, this should be
// called IMMEDIATELY AFTER the system frequency has been changed.
// According to datasheet, frequency must not be between 13 and 26 MHz and,
// thus, buck divider must be between 8 and 16.
void MC13224V_Buck_Regulator::adjust_buck_clock()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::adjust_buck_clock()\n";

    const unsigned int clock = Traits<Machine>::CLOCK;
    unsigned char div = 0x0f;

    if(clock < 1359000)       div = 0x08; // low efficency here
    else if(clock < 1519000)  div = 0x09;
    else if(clock < 1679000)  div = 0x0a;
    else if(clock < 1839000)  div = 0x0b;
    else if(clock < 1999000)  div = 0x0c;
    else if(clock < 2159000)  div = 0x0d;
    else if(clock < 2319000)  div = 0x0e;
    else if(clock < 2479000)  div = 0x0f;
    else if(clock <= 2600000) div = 0x00; // low efficency here

    crm_vreg_cntl((crm_vreg_cntl() & 0xfffff000) | div);
}

void MC13224V_Buck_Regulator::NVM_1P8V_enable()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::NVM_1P8V_enable()\n";

    crm_vreg_cntl(crm_vreg_cntl() | 0x80);
    SPIF_1P8V_enable();
    // Memory must NOT be used until voltage is stable. There is a risk of
    // damaging the flash memory (permanently).
    while(!NVM_1P8V_enabled());
}

void MC13224V_Buck_Regulator::NVM_1P8V_disable()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::NVM_1P8V_disable()\n";

    crm_vreg_cntl(crm_vreg_cntl() & 0xffffff7f);
}

bool MC13224V_Buck_Regulator::NVM_1P8V_enabled()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::NVM_1P8V_enabled()\n";

    return (crm_status() & 0x00040000);
}

void MC13224V_Buck_Regulator::Radio_1P5V_select(MC13224V_Buck_Regulator::RadioCurrentDrain drain)
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::Radio_1P5V_select()\n";

    crm_vreg_cntl((crm_vreg_cntl() & 0xffffff9f) | drain);
}

// Enabling/Disabling the 1P5V Regulator only works with MCU in IDLE mode, i.e.,
// when transceiver is not active.

void MC13224V_Buck_Regulator::Radio_1P5V_txrx_enable()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::Radio_1P5V_txrx_enable()\n";

    //check if PLL was enabled - MC1322xRM (Rev. 1.4) page 5-55 table 5-25
    if (crm_vreg_cntl() & RADIO_REGULATOR_TXRX_PLL)
    {
        Radio_1P5V_mode(RADIO_REGULATOR_OFF);
        while(Radio_1P5V_enabled()); //wait until turned off
    }
    Radio_1P5V_mode(RADIO_REGULATOR_TXRX);
    while(!Radio_1P5V_enabled()); //wait until turned on
}

void MC13224V_Buck_Regulator::Radio_1P5V_txrx_pll_enable()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::Radio_1P5V_txrx_pll_enable()\n";

    Radio_1P5V_mode(RADIO_REGULATOR_TXRX_PLL);
    while(!Radio_1P5V_enabled()); //wait until turned on
}

void MC13224V_Buck_Regulator::Radio_1P5V_disable()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::Radio_1P5V_disable()\n";

    Radio_1P5V_mode(RADIO_REGULATOR_OFF);
}

bool MC13224V_Buck_Regulator::Radio_1P5V_enabled()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::Radio_1P5V_enabled()\n";

    return (crm_status() & 0x00080000);
}

bool MC13224V_Buck_Regulator::Radio_1P5V_txrx_enabled()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::Radio_1P5V_txrx_enabled()\n";

    return (crm_vreg_cntl() & 0x08);
}

bool MC13224V_Buck_Regulator::Radio_1P5V_txrx_pll_enabled()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::Radio_1P5V_txrx_pll_enabled()\n";

    return (crm_vreg_cntl() & 0x18);
}

void MC13224V_Buck_Regulator::Radio_1P5V_mode(RadioRegulatorMode mode)
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::Radio_1P5V_mode(" << mode << ")\n";

    crm_vreg_cntl((crm_vreg_cntl() & 0xffffff9f) | mode);
}

// will cause other regulators to be OFF for up to 1.5 ms
void MC13224V_Buck_Regulator::enter_bypass()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::enter_bypass()\n";

    bool nvm  = NVM_1P8V_enabled();

    if (nvm) NVM_1P8V_disable();

    crm_vreg_cntl((crm_vreg_cntl() & ~0x03) | 0x04);
    while(!is_in_bypass());

    if (nvm) NVM_1P8V_enable();
}

void MC13224V_Buck_Regulator::leave_bypass()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::leave_bypass()\n";

    if(!is_in_bypass()) return;

    bool txrx = Radio_1P5V_txrx_enabled();
    bool pll  = Radio_1P5V_txrx_pll_enabled();
    bool nvm  = NVM_1P8V_enabled();

    NVM_1P8V_disable();
    Radio_1P5V_disable();
    disable();

    crm_vreg_cntl(crm_vreg_cntl() & ~0x04);
    enable();

    if (pll) Radio_1P5V_txrx_pll_enable();
    else if (txrx) Radio_1P5V_txrx_enable();
    if (nvm) NVM_1P8V_enable();
}

bool MC13224V_Buck_Regulator::is_in_bypass()
{
//    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::is_in_bypass()\n";

    return (((crm_vreg_cntl() & 0x07) == 0x04) && (crm_status() & 0x00020000));
}

void MC13224V_Buck_Regulator::VREG_wakeup_reconfigure()
{
    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::VREG_wakeup_reconfigure()\n";

    crm_vreg_cntl(_vreg);
    if (_vreg & 0x80) // Enabled NVM
        while(!NVM_1P8V_enabled()); // Needs to stabilize voltage
    if (_vreg & 0x08) // Enabled Radio
        while(!Radio_1P5V_enabled()); // Needs to stabilize voltage
}

unsigned int MC13224V_Buck_Regulator::crm_sys_cntl()
{
//    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::crm_sys_cntl()\n";

    return CPU::in32(Machine::IO::CRM_SYS_CNTL);
}

void MC13224V_Buck_Regulator::crm_sys_cntl(const unsigned int v)
{
//    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::crm_sys_cntl()" << v << "\n";

    CPU::out32(Machine::IO::CRM_SYS_CNTL, v);
}

unsigned int MC13224V_Buck_Regulator::crm_vreg_cntl()
{
//    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::crm_vreg_cntl()\n";

    return CPU::in32(Machine::IO::CRM_VREG_CNTL);
}

void MC13224V_Buck_Regulator::crm_vreg_cntl(const unsigned int v)
{
//    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::crm_vreg_cntl()" << v << "\n";

    _vreg = v;
    CPU::out32(Machine::IO::CRM_VREG_CNTL, v);
}

unsigned int MC13224V_Buck_Regulator::crm_status()
{
//    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::crm_status()\n";

    return CPU::in32(Machine::IO::CRM_STATUS);
}

void MC13224V_Buck_Regulator::crm_status(const unsigned int v)
{
//    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::crm_status()" << v << "\n";

    CPU::out32(Machine::IO::CRM_STATUS, v);
}

unsigned int MC13224V_Buck_Regulator::crm_mod_status()
{
//    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::crm_mod_status()\n";

    return CPU::in32(Machine::IO::CRM_MOD_STATUS);
}

void MC13224V_Buck_Regulator::crm_mod_status(const unsigned int v)
{
//    db<MC13224V_Buck_Regulator>(TRC) << "MC13224V_Buck_Regulator::crm_mod_status()" << v << "\n";

    CPU::out32(Machine::IO::CRM_MOD_STATUS, v);
}
