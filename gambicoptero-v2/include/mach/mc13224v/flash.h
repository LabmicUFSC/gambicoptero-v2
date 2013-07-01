// EPOS-- MC13224V Flash Mediator Declarations

#ifndef __mc13224v_flash_h
#define __mc13224v_flash_h

#include <system/config.h>
#include <flash.h>

__BEGIN_SYS

class MC13224V_Flash {
public:
    typedef Flash_Common::Address Address;

    enum AddressMap
    {
        CODE_SIZE = 0x18000,
        DATA_SIZE = 0x7000,
        RESERVED_SIZE = 0x1000,
        FLASH_BASE = 0x00000,
        FLASH_TOP  = 0x1ffff,
        CODE_BASE = FLASH_BASE,
        CODE_TOP = CODE_BASE + CODE_SIZE - 1,
        DATA_BASE = FLASH_BASE + CODE_SIZE,
        DATA_TOP = DATA_BASE + DATA_SIZE - 1,
        RESERVED_BASE = DATA_BASE + DATA_SIZE,
        RESERVED_TOP = FLASH_TOP
    };

//    static char read(const Address & a);
    static bool read(const Address & a, char * d, unsigned int s);
    static int write(Address a, char * d, unsigned int s);
    static bool erase(Address sector);
    static bool erase_all();
    static unsigned int get_block_size() { return _block_size; }
    static int address2sector(Address a) { return a / _block_size; }

    static void init();

    typedef char OP_Mode;
    enum {
        OFF = 0,
        FULL = 1,
        STANDBY = OFF,
        LIGHT = FULL
    };
    static OP_Mode power() { return _mode; }
    static void power(OP_Mode mode);

    static void lock_last_page() { nvm_setsvar(1); }
    static void unlock_last_page() { nvm_setsvar(0); }

private:
    MC13224V_Flash() {}
    ~MC13224V_Flash() {}

    static bool backup_reserved();
    static bool restore_reserved();

    static OP_Mode _mode;

//TODO: access to ROM routines MUST be private
public:
    static enum nvmInterface_t {
        gNvmInternalInterface_c = 0,
        gNvmExternalInterface_c,
        gNvmInterfaceMax_c
    } _interface;

    static enum nvmType_t {
        gNvmType_NoNvm_c = 0,
        gNvmType_SST_c,
        gNvmType_ST_c,
        gNvmType_ATM_c,
        gNvmType_Max_c
    } _type;

    static enum nvmErr_t {
        gNvmErrNoError_c = 0,
        gNvmErrInvalidInterface_c,
        gNvmErrInvalidNvmType_c,
        gNvmErrInvalidPointer_c,
        gNvmErrWriteProtect_c,
        gNvmErrVerifyError_c,
        gNvmErrAddressSpaceOverflow_c,
        gNvmErrBlankCheckError_c,
        gNvmErrRestrictedArea_c,
        gNvmErrNvmRegOff_c,
        gNvmErrMaxError_c
    } _error;

    static unsigned int _block_size;
    static char * _reserved;

    static const unsigned int NVM_DETECT_ADDR = 0x00006CB9;
    typedef nvmErr_t (*nvm_detect_func)(nvmInterface_t nvmInterface,
            nvmType_t* pNvmType);
    static nvm_detect_func nvm_detect;

    static const unsigned int NVM_ERASE_ADDR = 0x00006e05;
    typedef nvmErr_t (*nvm_erase_func)(nvmInterface_t nvmInterface, nvmType_t nvmType, unsigned int sectorBitfield);
    static nvm_erase_func nvm_erase;

    static const unsigned int NVM_READ_ADDR = 0x00006D69;
    typedef nvmErr_t (*nvm_read_func)(nvmInterface_t nvmInterface,
            nvmType_t nvmType, void *pDest, unsigned int address,
            unsigned int numBytes);
    static nvm_read_func nvm_read;

    static const unsigned int NVM_WRITE_ADDR = 0x00006ec5;
    typedef nvmErr_t (*nvm_write_func)(nvmInterface_t nvmInterface,
            nvmType_t nvmType, void *pSrc, unsigned int address,
            unsigned int numBytes);
    static nvm_write_func nvm_write;

    static const unsigned int NVM_SETSVAR_ADDR = 0x00007085;
    typedef void (*nvm_setsvar_func)(unsigned int zero_for_awesome);
    static nvm_setsvar_func nvm_setsvar;
};

__END_SYS

#endif

