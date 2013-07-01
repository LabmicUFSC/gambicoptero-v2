// EPOS ATMega16 Test Program

#include <utility/ostream.h>
#include <machine.h>

__USING_SYS

int main()
{
    OStream cout;

    cout << "ATMega16 test\n";

    ATMega16 machine;

    return 0;
}
