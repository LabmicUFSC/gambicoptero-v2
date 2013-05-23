// EPOS ATMega128 NIC Mediator Test

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <nic.h>

__USING_SYS

OStream cout;

int main()
{
    NIC nic;
    NIC::Address src, dst;
    NIC::Protocol prot;
    char data[nic.mtu()];

    for(int i = 0; i < 10; i++)
	nic.send(NIC::BROADCAST, 0x8888, "alguem ai?\n", 12);
    for(int i = 0; i < 10; i++) 
	nic.receive(&src, &prot, data, nic.mtu());

    NIC::Statistics stat = nic.statistics();
    cout << "Statistics\n"
	 << "Tx Packets: " << stat.tx_packets << "\n"
	 << "Tx Bytes:   " << stat.tx_bytes << "\n"
	 << "Rx Packets: " << stat.rx_packets << "\n"
	 << "Rx Bytes:   " << stat.rx_bytes << "\n";
}
