// EPOS ATMega128 Test Program

#include <utility/ostream.h>
#include <machine.h>

__USING_SYS

int main()
{
    OStream cout;

    cout << "ATMega128 test\n";

    ATMega128 machine;

    return 0;
}
