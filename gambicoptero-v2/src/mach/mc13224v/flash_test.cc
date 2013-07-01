// EPOS MC13224V_Flash Test Program

#include <utility/ostream.h>
#include <uart.h>
#include <flash.h>

__USING_SYS

OStream cout;
UART uart;

// This is an interactive test. You will need a serial terminal in a PC connected to the board.


void get_string(char * buffer, unsigned int size)
{
    unsigned int i;
    for(i = 0; i < size; i++)
    {
        buffer[i] = uart.get();
        if(buffer[i] == '\n') break;
    }
    for(; i < size; i++)
        buffer[i] = ' ';
}

int main()
{
    char buffer[33];
    bool ret = false;
    Flash::Address a = Flash::DATA_BASE;

    cout << "MC13224V_Flash test suite\n";
    cout << "WARNING!!!! This test will change contents of the flash memory! Information on the flash memory WILL be lost!\n";

    cout << "Follow instructions and see results\n\n";

    cout << "This is what is written in the first 32 bytes of the last page:\n";
    ret = Flash::read(a, buffer, 32);
    if(!ret)
    {
        cout << "Reading from flash memory failed!\n";
        return -1;
    }
    buffer[32] = '\0';
    cout << buffer << "\n\n";

    cout << "Please enter a string to be written to the flash memory (end line to finish): ";
    get_string(buffer, 32);
    buffer[32] = '\0';
    cout << "String entered: " << buffer << "\n\n";

    cout << "Writing string to flash... ";
    ret = Flash::write(a, buffer, 32);
    if(!ret)
    {
        //Maybe sector needs to be erased
        ret = Flash::erase(Flash::address2sector(a));
        if(ret) ret = Flash::write(a, buffer, 32);
    }
    if(!ret)
    {
        cout << "Wrting to flash memory failed!\n";
        return -1;
    }
    cout << "done!\n\n";

    cout << "We are done by now. Try reseting and checking if the string you entered is shown at the begining of this test.\n";

    while(1);

    return 0;
}
