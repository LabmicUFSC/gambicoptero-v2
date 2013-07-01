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
    Network::Address me(10,0,1,1);
    Network::Address server(10,0,1,15);
    Network::Address from;
    Network::Protocol prot;
    net.address(me);

    char data[1500];

    for(int i = 0; i < 10; i++) {
        net.send(server, "alguem ai?\n", 12);
        cout << "Sent to server: " << server << "\n";
    }
    for(int i = 0; i < 10; i++) {
        net.receive(&from, data, 1500);
        cout << "Received from server: " << from << " -> " << data << "\n";
    }
    Alarm::delay(10000000);
    for(int i = 0; i < 10; i++) {
        net.send(server, "obrigado!!\n", 12);
        cout << "Sent to server: " << server << "\n";
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

