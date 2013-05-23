// EPOS PC Network Abstraction Test

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <network.h>

__USING_SYS

OStream cout;

int main()
{
    Network net;
    Network::Address to(10,0,1,1);
    Network::Address from;
    Network::Protocol prot;
    char data[1500];

    for(int i = 0; i < 10; i++)
	net.send(to, "alguem ai?\n", 12);
    for(int i = 0; i < 10; i++) 
	net.receive(&from, data, 1500);

    Network::Statistics stat = net.statistics();
    cout << "Statistics\n"
	 << "Tx Packets: " << stat.tx_packets << "\n"
	 << "Tx Bytes:   " << stat.tx_bytes << "\n"
	 << "Rx Packets: " << stat.rx_packets << "\n"
	 << "Rx Bytes:   " << stat.rx_bytes << "\n";
};
