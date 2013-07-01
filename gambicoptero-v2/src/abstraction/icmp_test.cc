#include <system/config.h>

#ifdef __NIC_H

#include <icmp.h>
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
	
	for (int seq=0;seq < 100;++seq)
	{
		ICMP::Packet pkt(ICMP::ECHO,0,123123,seq);
		icmp.send(IP::Address(10,0,2,2),pkt);
		Alarm::delay(100000);
	}

    return 0;
}
#else

int main() { return 0; }

#endif

