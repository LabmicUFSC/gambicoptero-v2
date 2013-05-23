#include <utility/ostream.h>
#include <display.h>
#include <machine.h>
#include <alarm.h>

__USING_SYS

OStream cout;

int main() {
    cout << "Hello. I'm EPOS.\n";

    CPU::out32(Machine::IO::GPIO_BASE, 0x0000);
    while(1) {
        cout << "Working...\n";

        CPU::out32(Machine::IO::GPIO_BASE, 1 << 23);
        Alarm::delay(20000);
        CPU::out32(Machine::IO::GPIO_BASE, 1 << 24);
        Alarm::delay(20000);
        CPU::out32(Machine::IO::GPIO_BASE, 1 << 25);
        Alarm::delay(20000);
    }
}
