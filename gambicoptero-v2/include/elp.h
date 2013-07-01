// ELP - Network Abstraction

#ifndef __elp_h
#define __elp_h

#include <network_protocol.h>

#ifdef __network_protocol_h

#include <machine.h>

__BEGIN_SYS

class ELP: public Network_Common
{
public:
    using Network_Common::_nic;
    using Network_Common::_prot;
    using Network_Common::_address;

    typedef
        Service<Traits<Network>::ARP>::Network_Service<NIC,ELP>
        Network_Service;

public:
    ELP(unsigned int unit = 0):
        Network_Common(unit),
        BROADCAST(Traits<Network>::BROADCAST),
        _network_service(&_nic, this)
    {
        db<Network>(TRC)
            << "ELP(unit=0)\n";

        _network_service.update(BROADCAST, NIC::BROADCAST);

        _nic.attach(this, M_ARP);
        _nic.attach(this, M_RARP);
    }

    ~ELP()
    {
        db<Network>(TRC)
            << "~ELP()\n";
    }

    int send(const Address & to, const char * data, unsigned int size)
    {
        if(size > _nic.mtu())
            db<Network>(WRN) << "ELP::send: frame size exceeds MTU!\n";

        NIC::Address dst = _network_service.resolve(to,(const char *) 0);

        if(!dst)
            return -1;

        return _nic.send(dst, P_ELP, data, size);
    }

    int receive(Address * from, char * data, unsigned int size)
    {
        int stat;
        NIC::Address src;
        Protocol prot = 0;

        do {
            if(size > _nic.mtu())
                db<Network>(WRN) << "ELP::receive: frame size exceeds MTU!\n";

            stat = _nic.receive(&src, &prot, data, size);

            //	if(stat > 0)
            //	    *from = _network_service.resolve(src);

            *from = BROADCAST;

        } while(prot != P_ELP);

        return stat;
    }

    void update(Observed * o, int p)
    {
        db<Network>(TRC) << "ELP::update(o=" << o
            << ",p=" << hex << p << dec << ")\n";

        NIC::Address src;
        Protocol prot = 0;
        NIC::PDU data;

        int size = _nic.receive(&src, &prot, data, sizeof(NIC::PDU));
        if(!size)
            return;

        _network_service.received(src, prot, data, size);
    }

    const Address & address() { return _address; }

    void address(const Address & address) {
        _address = address;
        _network_service.address(address);
    }

private:
    Address BROADCAST;
    Network_Service _network_service;
};

__END_SYS

#endif /* __network_protocol_h */

#endif /* __elp_h */

