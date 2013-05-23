#include <alarm.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <machine.h>
#include <nic.h>
#include <uart.h>

__USING_SYS

const unsigned char SINK_ID = 0x01;
const unsigned int DATA_SIZE = 64;

NIC * nic;
OStream cout;

char msg[DATA_SIZE];

void write_to_eeprom(NIC::Statistics *stats) {
    EEPROM eeprom;

    NIC::Address address(1);
    unsigned char *ptr = reinterpret_cast<unsigned char*> (&address);
    for (unsigned int i = 0; i < sizeof(NIC::Address); ++i) {
        eeprom.write(i, ptr[i]);
    }

    ptr = reinterpret_cast<unsigned char*> (stats);
    for (unsigned int i = 0; i < sizeof(NIC::Statistics); ++i) {
        eeprom.write(i + sizeof(NIC::Address), ptr[i]);
    }
}

void sensor(unsigned char id) {

    cout << "sensor id = " << id << "\n";

    CPU::out8(Machine::IO::PORTB, (1 << 5));

    for (int z = 0; z < 30; z++) {

        nic->send(SINK_ID, (NIC::Protocol) 0, &msg, sizeof(msg));
        /*
        while (nic->send(SINK_ID, 0, &msg, sizeof(msg)) != CMAC::TX_OK)
            ;
            */

        for (int x = 0; x < 0xffff; x++);
        for (int x = 0; x < 0xffff; x++);
    }

    NIC::Statistics stats = nic->statistics();
//    write_to_eeprom(&stats);
    CPU::out8(Machine::IO::PORTB, (1 << 7));
    cout << stats << "\n";

    while (true)
        ;
}

void button_int() {
    cout << "Statistics:\n";
    cout << nic->statistics() << "\n";
}

int sink() {

    IC::int_vector(IC::IRQ_IRQ6, button_int);
    IC::int_vector(IC::IRQ_IRQ7, button_int);

    typedef IO_Map<Machine> IO;

    int irqNumber = 7;

    for (int i = 0; i < 2; i++) {
        CPU::out8(IO::DDRE, CPU::in8(IO::DDRE) & ~(1 << irqNumber));
        CPU::out8(IO::PORTE, CPU::in8(IO::PORTE) | (1 << irqNumber));
        unsigned char ui8ShiftCount = (irqNumber - 4) << 1;
        // Clear previous settings of corresponding interrupt sense control
        CPU::out8(IO::EICRB, CPU::in8(IO::EICRB) & ~(3 << ui8ShiftCount));
        // Setup corresponding interrupt sence control
        CPU::out8(IO::EICRB, CPU::in8(IO::EICRB)
                | ((3 & 0x03) << ui8ShiftCount));
        // Clear the INTn interrupt flag
        CPU::out8(IO::EIFR, CPU::in8(IO::EIFR) & ~(1 << irqNumber));
        // Enable external interrupt request
        CPU::out8(IO::EIMSK, CPU::in8(IO::EIMSK) | (1 << irqNumber));
        irqNumber = 6;
    }

    NIC::Protocol prot;
    NIC::Address src;
    int i = 0;

    cout << "Sink\n";

    int num = 0;
    while (true) {
        nic->receive(&src, &prot, &msg, sizeof(msg));
    }
}

void alarm_test()
{
    cout << "alarm_test\n";
    static int i = 5;
    CPU::out8(Machine::IO::PORTB, (1 << i));
    cout << TSC::time_stamp() << "\n";
    if(++i == 8) i = 5;
}

int main() {
    while(1){
    Alarm::delay(2000000);
    OStream out;
    out << "OIOIOIOI";
}

   // nic = new NIC;
  //  sensor(2);
//    sensor(3);
//    sink();
}
