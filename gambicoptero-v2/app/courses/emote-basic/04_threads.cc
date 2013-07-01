// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <utility/ostream.h>
#include <sensor.h>
#include <battery.h>
#include <thread.h>
#include <periodic_thread.h>
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
} msg;

int temperature()
{
    Temperature_Sensor temp;
    msg.msg_id = 0x00;

    while(true)
    {
        Periodic_Thread::wait_next();
        msg.data.s = temp.sample();
        while (nic->send(NIC::BROADCAST, (NIC::Protocol) 1, (char*)&msg, 2) != 11)
            cout << "failed\n";
        cout << "tx done\n";
    }

    return -1;
}

int battery()
{
    Battery batt;
    msg.msg_id = 0x01;

    while(true)
    {
        Periodic_Thread::wait_next();
        msg.data.s = batt.sample();
        while (nic->send(NIC::BROADCAST, (NIC::Protocol) 1, (char*)&msg, 2) != 11)
            cout << "failed\n";
        cout << "tx done\n";
    }

    return -1;
}

int main() {
    nic = new NIC();

    msg.node_id = 0x0f;

    Periodic_Thread temp_thread(&temperature, 2000000);
    Periodic_Thread batt_thread(&battery, 5000000);

    int ret = 0;
    ret += temp_thread.join();
    ret += batt_thread.join();

    return ret;
}
