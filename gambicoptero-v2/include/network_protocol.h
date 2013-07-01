#ifndef __network_protocol_h
#define __network_protocol_h

#include <nic.h>

#ifdef __NIC_H

#include <machine.h>
#include <service.h>

__BEGIN_SYS

class Network_Common: public NIC_Common::Observer, public NIC_Common::Observed
{
public:
    class Address: public NIC_Common::Address<4> {
    public:
        Address() {}
        Address(unsigned char a[4]) 
            : NIC_Common::Address<4>(a[3],a[2],a[1],a[0]) {}
        Address(unsigned long a)
            : NIC_Common::Address<4>(a & 0xff,
                                    (a>>8) & 0xff,
                                    (a>>16) & 0xff,
                                    (a>>24) & 0xff)
        {}
        Address(unsigned char a0, unsigned char a1,
                unsigned char a2, unsigned char a3)
            : NIC_Common::Address<4>(a3, a2, a1, a0)
        {}

        operator int() { return *reinterpret_cast<int *>(this); }
        operator int() const { return *reinterpret_cast<const int *>(this); }
        unsigned char id() { return _address[3]; }
    } __attribute__((packed,__may_alias__));

    typedef NIC::Protocol Protocol;
    enum {
        M_ARP    = NIC::ARP,
        M_RARP   = NIC::RARP,
        P_ELP    = NIC::ELP,
        P_IP     = NIC::IP,
        P_ROUTER = NIC::ROUTER
    };

    typedef NIC::Statistics Statistics;

    typedef NIC_Common::Observer Observer;
    typedef NIC_Common::Observed Observed;

    class Header;

public:
    Network_Common(unsigned int unit):
        _nic(unit),
        _address((unsigned long) Traits<Network>::ADDRESS)
    { }

    ~Network_Common() { }
        
    virtual int send(const Address & to, const char * data, unsigned int size) = 0;

    virtual int receive(Address * from, char * data, unsigned int size) = 0;

    virtual void update(Observed * o, int p) = 0;

    void reset() { _nic.reset(); }

    const Address & address() { return _address; }

    void address(const Address & address) { _address = address; }

    const Statistics & statistics() { return _nic.statistics(); }

    void protocol(Protocol prot) { _prot = prot; }

protected:
    NIC _nic;

    Protocol _prot;
    Address _address;
};

__END_SYS

#endif /* __NIC_H */

#endif /* __network_protocol_h */

