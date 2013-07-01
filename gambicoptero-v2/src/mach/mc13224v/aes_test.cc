#include <mach/mc13224v/aes_controller.h>

__USING_SYS

OStream cout;

void test_cipher()
{
    AES_Controller *controller = AES_Controller::get_instance();

    char data[16] = {"abcdefg"};
    char key[16] = {"123456"};
    char encrypted[16] = {0};
    char decrypted[16] = {0};
    char mac[16] = {0};

    controller->encrypt(data, key, encrypted, mac);
    cout << "Encrypted text: " << encrypted << "\n";
    cout << "Encrypted MAC: " << mac << "\n";

    controller->decrypt(encrypted, key, decrypted);
    cout << "Decrypted text: " << decrypted << "\n";
    cout << "Decrypted MAC: " << mac << "\n";

    bool ret = controller->check_mac(decrypted, key, mac);
    cout << "Check mac (OK): " << ret << "\n";

    ret = controller->check_mac("abcdefh", key, mac);
    cout << "Check mac (X): " << ret << "\n";
}

int main()
{
    test_cipher();
    return 0;
}

