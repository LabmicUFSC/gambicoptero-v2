// EPOS ATMega1281 Test Program

#include <utility/ostream.h>
#include <machine.h>

__USING_SYS

int main()
{
    OStream cout;

    cout << "ATMega1281 test\n";

    ATMega1281 machine;

    return 0;
}

