// EPOS AVRMCU_CAN Test Program

#include <mach/at90can128/can.h>
#include <utility/ostream.h>

__USING_SYS

int main()
{
    OStream cout;

    cout << "AT90CAN128_CAN test\n";

    AT90CAN128_CAN can;

    return 0;
}
