// EPOS ATMega16_Timer Test Program

// This work is licensed under the Creative Commons 
// Attribution-NonCommercial-NoDerivs License. To view a copy of this license, 
// visit http://creativecommons.org/licenses/by-nc-nd/2.0/ or send a letter to 
// Creative Commons, 559 Nathan Abbott Way, Stanford, California 94305, USA.


#include <utility/ostream.h>
#include <timer.h>

__USING_SYS

int main()
{
    OStream cout;

    cout << "ATMega16_Timer test\n";

    ATMega16_Timer_1 timer;

    return 0;
}
