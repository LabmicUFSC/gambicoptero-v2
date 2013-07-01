// EPOS PC Network Abstraction Test

#include <system/config.h>
#include <alarm.h>

#ifdef __NIC_H

#include <network.h>

__USING_SYS

OStream cout;

int main()
{
    cout << "\n";
    cout << "Start\n\n";

    Network net;
    Network::Address client(10,0,1,1);
    Network::Address from;
    Network::Protocol prot;

    char data[1500];

    for(int i = 0; i < 10; i++) {
        net.receive(&from, data, 1500);
        cout << "Received from client: " << from << " -> " << data << "\n";
    }
    Alarm::delay(10000000);
    for(int i = 0; i < 10; i++) {
        net.send(client, "eu estou!!\n", 12);
        cout << "Sent to client: " << client << "\n";
    }
    for(int i = 0; i < 10; i++) {
        net.receive(&from, data, 1500);
        cout << "Received from client: " << from << " -> " << data << "\n";
    }
    Alarm::delay(10000000);

    Network::Statistics stat = net.statistics();
    cout << "Statistics\n"
        << "Tx Packets: " << stat.tx_packets << "\n"
        << "Tx Bytes:   " << stat.tx_bytes << "\n"
        << "Rx Packets: " << stat.rx_packets << "\n"
        << "Rx Bytes:   " << stat.rx_bytes << "\n";

    Alarm::delay(10000000);
};

#else

int main() { return 0; }

#endif

