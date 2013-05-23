// EPOS MC13224V_Battery Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <battery.h>
#include <alarm.h>

#include <mach/mc13224v/buck_regulator.h>

__USING_SYS

OStream cout;

void delay()
{
    static const unsigned int delay = 5000000;
    cout << "You have " << delay / 1000000 << " seconds to do that.\n";
    Alarm::delay(delay);
}

int main()
{
    cout << "MC13224V_Battery and Buck_Regulator test suite\n";
    cout << "This is an interactive test. You will need an adjustable voltage source connected in the place of the battery.\n";
    cout << "Please follow the instructions and see the results\n\n";

    cout << "Performing a simple battery level read...";
    cout << "Your source is regulated to " << Battery::sys_batt().sample() << " millivolts.\n\n";

    cout << "Now adjust the voltage source to a value betwen 2V and 2.3V\n";
    delay();

    cout << "Buck regulator should be on bypass mode... ";
    if (MC13224V_Buck_Regulator::is_in_bypass())
        cout << "OK";
    else
        cout << "Failed!";
    cout << "\n\n";

    cout << "Now adjust the voltage source to a value betwen 2.7V and 3V\n";
    delay();

    cout << "Buck regulator should be active now... ";
    if (!MC13224V_Buck_Regulator::is_in_bypass())
        cout << "OK";
    else
        cout << "Failed!";
    cout << "\n\n";

    cout << "OK. I'm done.\n\n";

    return 0;
}
