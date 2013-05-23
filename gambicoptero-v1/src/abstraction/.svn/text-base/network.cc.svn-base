// EPOS Network Abstraction Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <nic.h>
#include <network.h>
#include <arp.h>

__BEGIN_SYS

// Class attributes
Network::ARP::Table Network::_arpt;
const Network::Address Network::BROADCAST = ~0;

// Methods
int Network::send(const Address & to, const void * data, unsigned int size)
{
    if(size > _nic.mtu())
	db<Network>(WRN) << "Network::send: frame size exceeds MTU!\n";

    MAC_Address dst = arp(to);
    if(!dst)
	return -1;
    
    return _nic.send(dst, PROT_ELP, data, size);
}

int Network::receive(Address * from, void * data, unsigned int size)
{
    int stat;
    MAC_Address src;
    Protocol prot;

    do {
	if(size > _nic.mtu())
	    db<Network>(WRN) << "Network::receive: frame size exceeds MTU!\n";

	stat = _nic.receive(&src, &prot, data, size);

    //	if(stat > 0)
    //	    *from = _arp_tab.rarp(src);

	*from = BROADCAST;

    } while(prot != PROT_ELP);

    return stat;
}

Network::MAC_Address Network::arp(const Address & la)
{
    for(unsigned int i = 0; i < Traits<Network>::ARP_TRIES; i++) {
	MAC_Address pa = _arpt.search(la);
	if(pa) {
	    db<Network>(TRC) << "Networl::arp(la=" << la << ") => "
			     << pa << "\n";
		
	    return pa;
	}

	Condition * cond = _arpt.insert(la);
	ARP::Packet request(ARP::REQUEST, _nic.address(), address(),
			    NIC::BROADCAST, la);
	_nic.send(NIC::BROADCAST, PROT_ARP, &request, sizeof(ARP::Packet));
	db<Network>(INF) << "Networl::arp:request sent!\n";
	
	Condition_Handler handler(cond);
	Alarm alarm(Traits<Network>::ARP_TIMEOUT, &handler, 1);
	cond->wait();
    } 

    db<Network>(TRC) << "Networl::arp(la=" << la << ") => not found!\n";

    return 0;
}

Network::Address Network::rarp(const MAC_Address & pa)
{
    for(unsigned int i = 0; i < Traits<Network>::ARP_TRIES; i++) {
	Address la = 0;
	if(la) {
	    db<Network>(TRC) << "Network::rarp(pa=" << pa << ") => "
			     << la << "\n";
		
	    return la;
	}

	Condition * cond = _arpt.insert(la);
	ARP::Packet request(ARP::RARP_REQUEST, pa, la, pa, la);
	_nic.send(NIC::BROADCAST, PROT_RARP, &request, sizeof(ARP::Packet));
	db<Network>(INF) << "Network::rarp:request sent!\n";
	
	Condition_Handler handler(cond);
	Alarm alarm(Traits<Network>::ARP_TIMEOUT, &handler, 1);
	cond->wait();
    } 

    db<Network>(TRC) << "Network::rarp(pa=" << pa << ") => not found!\n";

    return 0;
}

void Network::update(NIC_Common::Observed * o, int p)
{
    db<Network>(TRC) << "Network::update(o=" << o 
		     << ",p=" << hex << p << dec << ")\n";

    NIC::Address src;
    NIC::Protocol prot;
    Ethernet::PDU data;
    int size = _nic.receive(&src, &prot, data, sizeof(Ethernet::PDU));
    if(!size)
	return;

    if(prot == PROT_ARP) {
	ARP::Packet packet = *reinterpret_cast<ARP::Packet *>(data);
	db<Network>(INF) << "Network::update:ARP_Packet=" << packet << "\n";

	if((packet.op() == ARP::REQUEST) && (packet.tpa() == address())) {
	    ARP::Packet reply(ARP::REPLY, _nic.address(), address(),
			     packet.sha(), packet.spa());
	    db<Network>(INF) << "Network::update:ARP_Packet=" << reply << "\n";
	    _nic.send(packet.sha(), PROT_ARP, &reply, sizeof(ARP::Packet));
	    
	    db<Network>(INF) << "Network::update: ARP request answered!\n";
	} else if((packet.op() == ARP::REPLY)
		  && (packet.tha() == _nic.address())) {
	    db<Network>(INF) << "Network::update:ARP reply received!\n";

	    _arpt.update(packet.spa(), packet.sha());
	}
    } else
	db<Network>(WRN) << "Network::update:unknown packet type ("
			 << prot << ")\n";
}

__END_SYS
