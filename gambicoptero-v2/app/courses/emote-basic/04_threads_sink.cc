// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <utility/ostream.h>
#include <nic.h>

__USING_SYS

NIC * nic;
OStream cout;

struct Message {
    char node_id;
    char msg_id;
    union {
        short s;
        char c[2];
    } data;
};


void sink() {
    NIC::Protocol prot;
    NIC::Address src;
    Message msg;

    cout << "Sink\n";

    while (true) {
        while(!(nic->receive(&src, &prot, &msg, sizeof(msg)) > 0));

        cout << "\n##########\n";
        cout << "Sender id: "   << (int) msg.node_id << "\n";
        if(msg.msg_id == 0x00)
            cout << "Temperature: " << msg.data.s << " C\n";
        else if(msg.msg_id == 0x01)
            cout << "Battery: " << msg.data.s << " %\n";
    }
}

int main() {
    nic = new NIC();

    sink();

    return 0;
}
