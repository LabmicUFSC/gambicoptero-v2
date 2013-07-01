#ifndef __asm_controller_h
#define __asm_controller_h

#include <system/config.h>
#include <cpu.h>
#include <machine.h>
#include <utility/malloc.h>
#include <utility/string.h>

__BEGIN_SYS

typedef IO_Map<Machine> IO;

class ASM_Controller
{
private:
    ASM_Controller() {
        initialize();
    }

public:
    static ASM_Controller *get_instance();

    void initialize();

    void set_key(const char *key);
    void set_data(const char *data);
    void set_counter();

    void set_ctr_mode() {
        //Put ASM in "Counter" encryption mode
        CPU::out32(IO::ASM_CONTROL1, CPU::in32(IO::ASM_CONTROL1) | (1 << CTR));
    }

    void set_cbc_mode() {
        //Put ASM in "CBC-MAC" encryption mode
        CPU::out32(IO::ASM_CONTROL1, CPU::in32(IO::ASM_CONTROL1) | (1 << CBC));
    }

    bool encrypt(char *encrypted_data, char *mac);
    bool check_mac(const char *mac);

private:
    enum Register_Bits
    {
        // ASM_CONTROL0
        CLEAR_IRQ = 31,
        LOAD_MAC  = 26,
        CLEAR     = 25,
        START     = 24,

        // ASM_CONTROL1
        MASK_IRQ    = 31,
        SELF_TEST   = 26,
        CTR         = 25,
        CBC         = 24,
        BYPASS      = 2,
        NORMAL_MODE = 1,
        ON          = 0,

        // ASM_STATUS
        TEST_PASS = 25,
        DONE      = 24,
    };

    static bool bit_is_set(unsigned int val, unsigned int bit) {
        return (((val & (1 << bit)) >> bit) == 1);
    }

    static ASM_Controller *_instance;
};

__END_SYS

#endif

