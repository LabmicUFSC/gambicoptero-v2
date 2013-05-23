// EPOS PC_Timer Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <machine.h>
#include <display.h>
#include <timer.h>

__USING_SYS

OStream cout;

void handler()
{
    static int elapsed;

    int lin, col;
    Display::position(&lin, &col);
    Display::position(0, 60 + Machine::cpu_id() * 2);
    Display::putc((elapsed++ % 10) + 48);
    Display::position(lin, col);
}

int main()
{
    cout << "PC_Timer test\n";

    PC_Timer timer(1000, &handler, PC_Timer::SCHEDULER, false);
    
    for(int i = 0; i < 10000; i++);
    cout << "count = " << timer.read() << "\n";
    for(int i = 0; i < 10000; i++);
    cout << "count = " << timer.read() << "\n";
    for(int i = 0; i < 10000; i++);
    cout << "count = " << timer.read() << "\n";
    for(int i = 0; i < 10000; i++);
    cout << "count = " << timer.read() << "\n";
    for(int i = 0; i < 10000; i++);
    cout << "count = " << timer.read() << "\n";
    for(int i = 0; i < 10000; i++);
    cout << "count = " << timer.read() << "\n";
    
    cout << "The End!\n";

    return 0;
}
