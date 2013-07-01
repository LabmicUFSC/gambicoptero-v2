// EPOS ATMega16_IC Test Program

#include <utility/ostream.h>
#include <rtc.h>
#include <machine.h>

__USING_SYS


int main()
{
    OStream cout;

    cout << "ATMega16_RTC test\n";

    ATMega16_RTC rtc;

    return 0;

}
