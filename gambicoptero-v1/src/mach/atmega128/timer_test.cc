// EPOS ATMega128_Timer Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <timer.h>

__USING_SYS

int main()
{
    OStream cout;

    cout << "ATMega128_Timer test\n";

    ATMega128_Timer_3 timer;

    return 0;
}
