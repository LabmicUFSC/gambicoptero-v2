#ifndef __aes_controller_h
#define __aes_controller_h

#include <system/config.h>
#include <semaphore.h>
#include <mach/mc13224v/asm_controller.h>
#include <utility/malloc.h>

__BEGIN_SYS

class AES_Controller
{
private:
     AES_Controller() {
         _controller = ASM_Controller::get_instance();
     }

public:
    static AES_Controller *get_instance();

    bool encrypt(const char *data, const char *key, char *encrypted_data, char *mac = 0);

    bool decrypt(const char *data, const char *key, char *decrypted_data) {
        return encrypt(data, key, decrypted_data);
    }

    bool check_mac(const char *data, const char *key, const char *mac);

private:
    static AES_Controller *_instance;

    ASM_Controller *_controller;
    Semaphore _semaphore;
};

__END_SYS

#endif

