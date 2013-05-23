

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#ifndef __channel_h
#define __channel_h

#include <udp.h>
#include <ip.h>
#include <nic.h>
#include <arp.h>

__BEGIN_SYS

template <typename Protocol>
class Channel{
private:
	static NIC *_nic;

public:
	typedef typename Protocol::Address Address;

	Channel() :
		_protocol(new Protocol) {
		
		if(!_nic) _nic = new NIC;
		if(!_arp) _arp = new ARP(*_nic, _protocol->self());
	}

	Channel(Address &a) :
		_protocol(new Protocol(a)) {
		
		if(!_nic) _nic = new NIC;
		if(!_arp) _arp = new ARP(*_nic, _protocol->self());
	}

	~Channel(){
		if(_protocol) delete _protocol;
	}

	int send(const Address &dst,const void *ptr, unsigned int size){
		PDU pdu(_protocol->self(), dst, ptr, size);
		
		const NIC::Address &mac = _arp->arp(dst);

		if(mac == ARP::NULL_ADDR) return 0;

		unsigned int mtu_size = _nic->mtu();
		if(pdu.size() <= mtu_size){
			db<Channel>(INF) << "Channel::send() " << pdu.size() << " sent!\n";
			_nic->send(mac, Protocol::ID, &pdu, pdu.size());
		} else {
			db<Channel>(INF) << "Channel::send() Message too large: Fragmentation needed\n";
			PDU frag;
			for(int i = 0; Protocol::get_fragment(i, mtu_size, &pdu, &frag); i++)
				_nic->send(mac, Protocol::ID, &frag, frag.size());
		}
		return size;
	}

	int receive(Address &src, void *ptr, int size){
		return _protocol->receive(src, ptr, size);
	}

private:
	typedef typename Protocol::PDU PDU;
	Protocol *_protocol;
	static ARP *_arp;
};

template <typename T>
NIC* Channel<T>::_nic = 0;

template <typename T>
ARP* Channel<T>::_arp = 0;

__END_SYS

#endif
