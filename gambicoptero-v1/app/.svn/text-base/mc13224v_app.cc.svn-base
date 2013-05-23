#include <machine.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <alarm.h>
#include <sensor.h>

__USING_SYS

const unsigned char SINK_ID  = 0x01;
const unsigned int DATA_SIZE = 32;

char msg[DATA_SIZE];

NIC * nic;
OStream cout;

void turn_on_led1() {
    unsigned int *GPIO_BASE = (unsigned int*)0x80000000;
    *GPIO_BASE = 1 << 23;
}

void turn_on_led2() {
    unsigned int *GPIO_BASE = (unsigned int*)0x80000000;
    *GPIO_BASE = 1 << 24;
}

void sensor(unsigned char id) {
    turn_on_led1();
    cout << "Sensor id = " << (int) id << "\n";

    Temperature_Sensor * temp = new Temperature_Sensor();

    for (unsigned int i = 0; i < DATA_SIZE; i++) {
        msg[i] = i;
    }

    msg[0] = id;

    char c = 0;

    while (true) {
        msg[1] = c++;
        msg[2] = temp->sample();

        int r;
        while ((r = nic->send(NIC::BROADCAST, (NIC::Protocol) 1, &msg, sizeof(msg))) != 11)
            cout << "failed " << r << "\n";

        cout << "tx done\n";

        for (volatile unsigned long i = 0; i <= 50 * (Traits<Machine>::CLOCK / 1000UL); i++);
    }
}

void sink() {
    NIC::Protocol prot;
    NIC::Address src;

    turn_on_led2();
    cout << "Sink\n";

    while (true) {
        while(!(nic->receive(&src, &prot, &msg, sizeof(msg)) > 0))
            cout << "failed\n";

        cout << "\n##########\n";
        cout << "Sender id: "   << (int) msg[0] << "\n";
        cout << "Msg number: "  << (int) msg[1] << "\n";
        cout << "Temperature: " << (int) msg[2] << " C\n";
        cout << "Protocol:"     << (int) prot   << "\n";
    }
}

int main() {
    nic = new NIC();

    sink();
//    sensor(1);
}

