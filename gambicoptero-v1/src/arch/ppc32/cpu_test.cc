// EPOS PPC32 Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <cpu.h>

__USING_SYS

int main()
{
    OStream cout;
    cout << "PPC32 test\n";

    PPC32 cpu;

    {
        volatile bool lock = false;
        if(cpu.tsl(lock))
            cout << "tsl(): doesn't function properly!\n";
        else
            if(cpu.tsl(lock))
                cout << "tsl(): ok\n";
            else
                cout << "tsl(): doesn't function properly!\n";
    }
    {
        volatile int number = 100;
        volatile int tmp;
        if((tmp = cpu.finc(number)) != 100)
            cout << "finc(): doesn't function properly (n=" << tmp
                 << ", should be 100)!\n";
        else
            if((tmp = cpu.finc(number)) != 101)
                cout << "finc(): doesn't function properly (n=" << tmp 
                     << ", should be 101)!\n";
            else
                cout << "finc(): ok\n";
    }
    {
        volatile int number = 100;
        volatile int tmp;
        if((tmp = cpu.fdec(number)) != 100)
            cout << "fdec(): doesn't function properly (n=" << tmp
                 << ", should be 100)!\n";
        else
            if((tmp = cpu.fdec(number)) != 99)
                cout << "fdec(): doesn't function properly (n=" << tmp 
                     << ", should be 99)!\n";
            else
                cout << "fdec(): ok\n";
    }


    cout << "PPC32 test finished\n";
    return 0;
}
