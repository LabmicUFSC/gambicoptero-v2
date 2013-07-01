// EPOS AVR8 Test Program
//
// Author: root
// Documentation: $EPOS/doc/cpu			Date: 27 Jan 2004

#include <utility/ostream.h>
#include <cpu.h>

__USING_SYS

int main()
{
    OStream cout;

    cout << "AVR8 test\n";

    AVR8 cpu;

    return 0;
}
