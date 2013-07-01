// EPOS ATMega16_UART Test Program

//#include <utility/ostream.h>
#include <uart.h>


__USING_SYS

int main()
{
    OStream cout;

    cout << "ATMega16_UART test\n";

    ATMega16_UART uart;

    return 0;

}
