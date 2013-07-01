#include <mach/mc13224v/aes_controller.h>

__BEGIN_SYS

AES_Controller *AES_Controller::_instance = 0;

AES_Controller *AES_Controller::get_instance()
{
    if (_instance == 0)
        _instance = new AES_Controller();

    return _instance;
}

bool AES_Controller::encrypt(const char *data, const char *key, char *encrypted_data, char *mac)
{
    _semaphore.p();

    _controller->set_key(key);
    _controller->set_ctr_mode();
    if (mac)
        _controller->set_cbc_mode();
    _controller->set_data(data);
    _controller->set_counter();

    encrypted_data[0] = 0;
    bool ret = _controller->encrypt(encrypted_data, mac);

    _semaphore.v();
    return ret;
}

bool AES_Controller::check_mac(const char *data, const char *key, const char *mac)
{
    _semaphore.p();

    _controller->set_key(key);
    _controller->set_cbc_mode();
    _controller->set_data(data);

    bool ret = _controller->check_mac(mac);

    _semaphore.v();
    return ret;
}

__END_SYS

