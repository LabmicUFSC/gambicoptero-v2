// EPOS Chronometer Abstraction Test Program

#include <utility/ostream.h>
#include <chronometer.h>
#include <alarm.h>

__USING_SYS;

int main()
{
    OStream cout;

    cout << "Chronometer test\n";

    Chronometer timepiece;

    cout << "Chronometer start.\n";
    timepiece.start();
    Alarm::delay(1500000);
    timepiece.stop();
    cout << "Chronometer stop.\n";

    cout << "\nElapsed time = " << timepiece.read() << " us\n";

    return 0;
}
