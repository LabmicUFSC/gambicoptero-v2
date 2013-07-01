// EPOS AVR8_TSC Test Program
//
// Author: fauze
// Documentation: $EPOS/doc/tsc			Date: 27 Jan 2004

#include <utility/ostream.h>
#include <tsc.h>

__USING_SYS

int main()
{
    OStream cout;

    cout << "AVR8_TSC test\n";

    AVR8_TSC tsc;

    return 0;
}
