// EPOS-- MC13224V Buck Regulator Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mc13224v_buck_regulator_h
#define __mc13224v_buck_regulator_h

#include <system/config.h>

__BEGIN_SYS

class MC13224V_Buck_Regulator
{
private:
    MC13224V_Buck_Regulator();

public:
    // methods for manipulating SYS_CNTL
    static void enable();
    static void disable();
    static bool enabled();
    static void SPIF_1P8V_enable();
    static void SPIF_1P8V_disable();

    // methods for manipulating VREG
    static void adjust_buck_clock();

    static void NVM_1P8V_enable();
    static void NVM_1P8V_disable();
    static bool NVM_1P8V_enabled();

    enum RadioCurrentDrain
    {
        RADIO_PRE_TRANSCEIVER_STATE = 0x00, // 2'b00 - 4 mA
        RADIO_NORMAL_TRANSCEIVER    = 0x20, // 2'b01 - 20 mA (default)
        RADIO_6DB_TRANSCEIVER       = 0x60  // 2'b11 - 40 mA
        // See MC1322xRM (Rev. 1.4) page 5-57 table 5-27.
    };
    static void Radio_1P5V_select(RadioCurrentDrain drain);
    static void Radio_1P5V_txrx_enable();
    static void Radio_1P5V_txrx_pll_enable();
    static void Radio_1P5V_disable();
    static bool Radio_1P5V_enabled();
    static bool Radio_1P5V_txrx_enabled();
    static bool Radio_1P5V_txrx_pll_enabled();

    static void enter_bypass();
    static void leave_bypass();
    static bool is_in_bypass();

    static void VREG_wakeup_reconfigure();
private:
    // VREG_CNTL contents are not retained in sleep mode, then we need to store
    // status here and reconfigure it on wake-up.
    static unsigned int _vreg;

    enum RadioRegulatorMode
    {
        RADIO_REGULATOR_OFF      = 0x00, // 2'b00 - Both regulators OFF
        RADIO_REGULATOR_TXRX     = 0x08, // 2'b01 - Rx/Tx regulator ON - 200us warm-up delay
        RADIO_REGULATOR_TXRX_PLL = 0x18  // 2'b11 - Rx/Tx and PLL regulators ON - 600us warm-up delay
        // See MC1322xRM (Rev. 1.4) page 5-57 table 5-26.
    };
    static void Radio_1P5V_mode(RadioRegulatorMode mode);

    inline static unsigned int crm_sys_cntl();
    inline static void crm_sys_cntl(const unsigned int v);
    inline static unsigned int crm_vreg_cntl();
    inline static void crm_vreg_cntl(const unsigned int v);
    inline static unsigned int crm_status();
    inline static void crm_status(const unsigned int v);
    inline static unsigned int crm_mod_status();
    inline static void crm_mod_status(const unsigned int v);
};

__END_SYS

#endif
