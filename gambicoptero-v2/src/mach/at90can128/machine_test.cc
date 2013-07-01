// EPOS AT90CAN128 Test Program

#include <utility/ostream.h>
#include <machine.h>

__USING_SYS

int main()
{
    OStream cout;

    cout << "AT90CAN128 test\n";

    AT90CAN128 machine;

    return 0;
}
