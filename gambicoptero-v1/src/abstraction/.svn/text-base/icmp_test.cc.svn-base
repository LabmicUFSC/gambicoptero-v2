#include <icmp.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <alarm.h>

__USING_SYS

/*
 * Simple PING test application
 */
OStream cout;


class ReplyObserver : public ICMP::Observer
{
public:
	void update(ICMP::Observed * ob,long type,IP::Address from,IP::Address to,
		    void* data,unsigned int size) {
		ICMP::Packet * pkt = (ICMP::Packet*)data;
		cout << "Echo reply received from " << from 
		     << " sequence number " << pkt->sequence() << endl;
	}
};

int main()
{	
	IP ip(0);
	
	//QEMU IP settings
	ip.set_address(IP::Address(10,0,2,15));
	ip.set_gateway(IP::Address(10,0,2,2));
	ip.set_netmask(IP::Address(255,255,255,0));
	
	
	ICMP icmp(&ip);

	ReplyObserver obs;
	icmp.attach(&obs, ICMP::ECHO_REPLY);
	
	for (int seq=0;seq < 4;++seq)
	{
		ICMP::Packet pkt(ICMP::ECHO,0,123123,seq);
		icmp.send(IP::Address(10,0,2,2),pkt);
		Alarm::delay(100000);
	}
}

