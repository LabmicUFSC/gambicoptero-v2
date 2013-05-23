#ifndef __arp_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __arp_h

#include <utility/malloc.h>
#include <utility/hash.h>
#include <alarm.h>
#include <condition.h>
#include <network_service.h>

__BEGIN_SYS

template<typename Link_Layer, typename Network_Layer>
class ARP: public Network_Service_Common<Link_Layer, Network_Layer>
{
public:
    typedef Network_Service_Common<Link_Layer, Network_Layer> Base;

    using Base::_nic;
    using Base::_network;
    using Base::_my_nic_address;

    typedef typename Base::Link_Address Link_Address;
    typedef typename Base::Network_Address Network_Address;

    typedef typename Base::Protocol Protocol;

    ARP(Link_Layer* nic, Network_Layer* network):
        Base(nic, network)
    { }

//private:
    class Entry
    {
    public:
        Entry(const Network_Address & la,
                Link_Address pa = Link_Layer::BROADCAST):
            _phy_addr(pa), _link(this, la)
        { }

        Link_Address _phy_addr;
        Condition _condition;
        typename Simple_Hash<Entry, 8>::Element _link;
    };

    // ARP Operations
    typedef unsigned short Oper;
    enum {
        REQUEST      = 1,
        REPLY        = 2,
        RARP_REQUEST = 3,
        RARP_REPLY   = 4
    };

    class Table: public Simple_Hash<Entry, 8>
    {
    private:
        typedef Simple_Hash<Entry, 8> Base;

    public:
        typedef typename Base::Element Element;

    public:
        Condition * insert(const Network_Address & la)
        {
            Element * el = Base::search_key(la);
            Entry * e;

            if(el)
                e = el->object();
            else {
                e = new(kmalloc(sizeof(Entry))) Entry(la);
                Base::insert(&e->_link);
            }

            return &e->_condition;
        }

        void update(const Network_Address & la, const Link_Address & pa)
        {
            Element * el = Base::search_key(la);
            Entry * e;

            if(el) {
                e = el->object();
                e->_phy_addr = pa;
                e->_condition.broadcast();
            } else {
                e = new(kmalloc(sizeof(Entry))) Entry(la, pa);
                Base::insert(&e->_link);
            }
        }

        Link_Address search(const Network_Address & la)
        {
            Element * el = Base::search_key(la);

            if(el)
                return el->object()->_phy_addr;
            else
                return 0;
        }
    };

    class Packet
    {
    public:
        // Hardware Types (HTYPE)
        enum { HTYPE_ETHERNET = 1 };

    public:
        Packet(Oper op, Link_Address sha, Network_Address spa,
                Link_Address tha, Network_Address tpa):
            _htype(CPU::htons(HTYPE_ETHERNET)),
            _ptype(CPU::htons(Network_Layer::PROT_IP)),
            _hlen(sizeof(Link_Address)),
            _plen(sizeof(Network_Address)),
            _oper(CPU::htons(op)),
            _sha(sha), _spa(spa), _tha(tha), _tpa(tpa) {}

        void op(const Oper & o) { _oper = CPU::htons(o); }
        void sha(const Link_Address & a) { _sha = a; }
        void spa(const Network_Address & a) { _spa = a; }
        void tha(const Link_Address & a) { _tha = a; }
        void tpa(const Network_Address & a) { _tpa = a; }

        Oper op() const { return CPU::ntohs(_oper); }
        const Link_Address & sha() const { return _sha; }
        const Network_Address & spa() const { return _spa; }
        const Link_Address & tha() const { return _tha; }
        const Network_Address & tpa() const { return _tpa; }

        friend Debug & operator<<(Debug & db, const Packet & p)
        {
            db  << "{htype="  << CPU::ntohs(p._htype)
                << ",ptype="  << hex << CPU::ntohs(p._ptype) << dec
                << ",hlen="   << p._hlen
                << ",plen="   << p._plen
                << ",opcode=" << CPU::ntohs(p._oper)
                << ",sha="    << p._sha
                << ",spa="    << p._spa
                << ",tha="    << p._tha
                << ",tpa="    << p._tpa << "}";

            return db;
        }

    private:
        unsigned short      _htype; // Hardware Type
        unsigned short      _ptype; // Protocol Type
        unsigned char       _hlen;  // Hardware Address Length
        unsigned char       _plen;  // Protocol Address Length
        unsigned short      _oper;  // Operation
        Link_Address        _sha; // Sender Hardware Address (48 bits)
        Network_Address     _spa; // Sender Protocol Address (32 bits)
        Link_Address        _tha; // Target Hardware Address (48 bits)
        Network_Address     _tpa; // Target Protocol Address (32 bits)
    };

public:
    void update(const Network_Address& la, const Link_Address& pa)
    {
        _arpt.update(la, pa);
    }

    Link_Address resolve(const Network_Address& addr, SegmentedBuffer * pdu)
    {
        for(unsigned int i = 0; i < Traits<Network>::ARP_TRIES; i++) {
            Link_Address pa = _arpt.search(addr);
            if(pa) {
                db<Network_Layer>(TRC) << "ARP_Router::resolve(addr=" << addr << ") => "
                        << pa << "\n";

                return pa;
            }

            Condition * cond = _arpt.insert(addr);
            Packet request(REQUEST, _nic->address(), _network->address(),
                                Link_Layer::BROADCAST, addr);

            _nic->send(Link_Layer::BROADCAST, Link_Layer::ARP, &request, sizeof(Packet));

            db<Network_Layer>(INF) << "ARP_Router::resolve:request sent!\n";

            Condition_Handler handler(cond);
            //Alarm alarm(Traits<Network>::ARP_TIMEOUT, &handler, 1);
            Alarm alarm(100000, &handler, 1);
            cond->wait();
        }

        db<Network_Layer>(TRC) << "ARP_Router::resolve(addr=" << addr << ") => not found!\n";

        return 0;
    }

    void received(const Link_Address& src, Protocol proto,
                  const char* data, int size)
    {
        if (proto == Link_Layer::ARP) {
            const Packet& packet = *reinterpret_cast<const Packet *>(data);
            db<Network_Layer>(INF) << "IP::update:ARP_Packet=" << packet << "\n";

            if((packet.op() == REQUEST) && (packet.tpa() == _network->address())) {
                Packet reply(REPLY, _nic->address(), _network->address(),
                        packet.sha(), packet.spa());

                db<Network_Layer>(INF) << "IP::update: ARP_Packet=" << reply << "\n";
                _nic->send(packet.sha(), Link_Layer::ARP, &reply, sizeof(Packet));

                db<Network_Layer>(INF) << "IP::update: ARP request answered!\n";
            }
            else if((packet.op() == REPLY) && (packet.tha() == _nic->address())) {
                db<Network_Layer>(INF) << "IP::update: ARP reply received!\n";

                _arpt.update(packet.spa(), packet.sha());
            }
        }
    }

private:
    Table _arpt;
};


// A dummy router that always resolves logical address to broadcast address
template<typename Link_Layer, typename Network_Layer>
class BCast: public Network_Service_Common<Link_Layer, Network_Layer>
{
public:
    typedef Network_Service_Common<Link_Layer, Network_Layer> Base;

    typedef typename Base::Link_Address Link_Address;
    typedef typename Base::Network_Address Network_Address;

    typedef typename Base::Protocol Protocol;

    typedef NIC::Address MAC_Address;


    BCast(Link_Layer* nic, Network_Layer* network): Base(nic, network) {}

    void update(const Network_Address& la, const Link_Address& pa)
    {
        // Do nothing
    }

    Link_Address resolve(const Network_Address& addr, SegmentedBuffer * pdu)
    {
        return MAC_Address(Link_Layer::BROADCAST);
    }

    void received(const Link_Address& src, Protocol proto,
                  const char* data, int size) {}
};

__END_SYS

#endif

