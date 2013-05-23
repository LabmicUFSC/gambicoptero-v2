// EPOS IA32_TSC Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <tsc.h>

using namespace System;

int main()
{
    OStream cout;

    cout << "IA32_TSC test\n";

    IA32_TSC tsc;

    return 0;
}
