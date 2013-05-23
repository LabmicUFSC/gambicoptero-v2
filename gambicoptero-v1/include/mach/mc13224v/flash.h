// EPOS-- MC13224V Flash Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mc13224v_flash_h
#define __mc13224v_flash_h

__BEGIN_SYS

class MC13224V_Flash {
private:
    static const unsigned int NVM_DETECT_ADDR = 0x00006CB9;
    static const unsigned int NVM_READ_ADDR = 0x00006D69;
    static const unsigned int NVM_SETSVAR_ADDR = 0x00007085;
    static const unsigned int NVM_ERASE_ADDR = 0x00006e05;
    static const unsigned int CRM_SET_SPIF_VOLTAGE_LEVEL_ADDR = 0x00003e8d;

    MC13224V_Flash() {
    }
    ~MC13224V_Flash() {
    }

public:
    enum nvmType_t {
        gNvmType_NoNvm_c,
        gNvmType_SST_c,
        gNvmType_ST_c,
        gNvmType_ATM_c,
        gNvmType_Max_c
    };

    enum nvmErr_t {
        gNvmErrNoError_c = 0,
        gNvmErrInvalidInterface_c,
        gNvmErrInvalidNvmType_c,
        gNvmErrInvalidPointer_c,
        gNvmErrWriteProtect_c,
        gNvmErrVerifyError_c,
        gNvmErrAddressSpaceOverflow_c,
        gNvmErrBlankCheckError_c,
        gNvmErrRestrictedArea_c,
        gNvmErrMaxError_c
    };

    enum nvmInterface_t {
        gNvmInternalInterface_c = 0,
        gNvmExternalInterface_c,
        gNvmInterfaceMax_c
    };

    enum crmErr_t {
        gCrmErrNoError_c = 0,
        gCrmErrCopCntlWP_c,
        gCrmErrXtalCntlWP_c,
        gCrmErrInvalidParameters_c,
        gCrmErrCalInProgress_c,
        gCrmErrNoCalInProgress_c,
        gCrmErrNo3V3Battery_c,
        gCrmErrRingOscOff_c,
        gCrmErrIgnoredInActualPowerMode_c,
        gCrmErrInvalidPowerSource_c,
        gCrmErrBuckNotEnabledNorBypassed_c
    };

    enum crmVoltageLevel_t {
        g3P3VLevel_c = 0,
        g1P8VLevel_c
    };

    typedef nvmErr_t (*nvm_detect_func)(nvmInterface_t nvmInterface,
            nvmType_t* pNvmType);
    static nvm_detect_func nvm_detect;

    typedef nvmErr_t (*nvm_read_func)(nvmInterface_t nvmInterface,
            nvmType_t nvmType, void *pDest, unsigned int address,
            unsigned int numBytes);
    static nvm_read_func nvm_read;

    typedef void (*nvm_setsvar_func)(unsigned int zero_for_awesome);
    static nvm_setsvar_func nvm_setsvar;

    typedef nvmErr_t (*nvm_erase_func)(nvmInterface_t nvmInterface, nvmType_t nvmType, unsigned int sectorBitfield);
    static nvm_erase_func nvm_erase;

    typedef void (*crm_set_spif_voltage_level_func)(
            crmVoltageLevel_t spiFVoltageLevel);
    static crm_set_spif_voltage_level_func CRM_SetSPIFVoltageLevel;
};

__END_SYS

#endif

