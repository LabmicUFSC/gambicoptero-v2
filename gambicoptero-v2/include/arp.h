#ifndef __arp_h
#define __arp_h

#include <network_service.h>

#ifdef __network_service_h

#include <utility/malloc.h>
#include <utility/hash.h>
#include <alarm.h>
#include <condition.h>

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
    enum {
        M_ARP    = Link_Layer::ARP,
        M_RARP   = Link_Layer::RARP,
        P_ELP    = Link_Layer::ELP,
        P_IP     = Link_Layer::IP,
        P_ROUTER = Link_Layer::ROUTER
    };

    ARP(Link_Layer* nic, Network_Layer* network):
        Base(nic, network)
    { }

private:
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
                return Link_Layer::BROADCAST;
        }
    };

public:
    class Control_Info // ARP Packet
    {
    public:
        // Hardware Types (HTYPE)
        enum { HTYPE_ETHERNET = 1 };

    public:
        Control_Info() {}
        ~Control_Info() {}

        void fill(Oper op, Link_Address sha, Network_Address spa,
                Link_Address tha, Network_Address tpa)
        {
            _htype = CPU::htons(HTYPE_ETHERNET);
            _ptype = CPU::htons(P_IP);
            _hlen  = sizeof(Link_Address);
            _plen  = sizeof(Network_Address);
            _oper  = CPU::htons(op);
            _sha   = sha;
            _spa   = spa;
            _tha   = tha;
            _tpa   = tpa;
        }

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

        friend Debug & operator<<(Debug & db, const Control_Info & p)
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
    } __attribute__((packed,__may_alias__));

public:
    void update(const Network_Address& la, const Link_Address& pa)
    {
        _arpt.update(la, pa);
    }

    Link_Address resolve(const Network_Address& la, const char * pdu)
    {
        for(unsigned int i = 0; i < Traits<Network>::TRIES; i++) {
            Link_Address pa = _arpt.search(la);
            if(pa) {
                db<Network>(TRC) << "ARP::resolve(la=" << la
                    << ") => " << pa << "\n";

                return pa;
            }

            Condition * cond = _arpt.insert(la);
            Control_Info request;
            request.fill(REQUEST, _nic->address(),
                    _network->address(), Link_Layer::BROADCAST, la);

            _nic->send(Link_Layer::BROADCAST,
                    M_ARP, &request, sizeof(Control_Info));

            db<Network>(INF) << "ARP::resolve:request sent!\n";

            Condition_Handler handler(cond);
            Alarm alarm(Traits<Network>::TIMEOUT, &handler, 1);
            cond->wait();
        }

        db<Network>(TRC) << "ARP::resolve(la=" << la
            << ") => not found!\n";

        return Link_Layer::BROADCAST;
    }

    virtual Network_Address resolve(const Link_Address& pa)
    {
        for(unsigned int i = 0; i < Traits<Network>::TRIES; i++) {
            Network_Address la = Network_Address(0,0,0,0);
            if(la) {
                db<Network>(TRC) << "ARP::resolve(pa=" << pa
                    << ") => " << la << "\n";

                return la;
            }

            Condition * cond = _arpt.insert(la);
            Control_Info request;
            request.fill(RARP_REQUEST, pa, la, pa, la);
            _nic->send(Link_Layer::BROADCAST,
                    M_RARP, &request, sizeof(Control_Info));

            db<Network>(INF) << "ARP::resolve:request sent!\n";

            Condition_Handler handler(cond);
            Alarm alarm(Traits<Network>::TIMEOUT, &handler, 1);
            cond->wait();
        }

        db<Network>(TRC) << "ARP::resolve(pa=" << pa
            << ") => not found!\n";

        return Network_Address(0,0,0,0);
    }

    void received(const Link_Address& src, Protocol proto,
            const char* data, int size)
    {
        if (proto == M_ARP) {
            const Control_Info& packet =
                *reinterpret_cast<const Control_Info *>(data);
            db<Network>(INF)
                << "ARP::update:ARP_Control_Info=" << packet << "\n";

            if((packet.op() == REQUEST)
                    && (packet.tpa() == _network->address())) {
                Control_Info reply;
                reply.fill(REPLY, _nic->address(), _network->address(),
                        packet.sha(), packet.spa());

                db<Network>(INF)
                    << "ARP::update: ARP_Control_Info=" << reply << "\n";
                _nic->send(packet.sha(),
                        M_ARP, &reply, sizeof(Control_Info));

                db<Network>(INF)
                    << "ARP::update: ARP request answered!\n";
            } else if((packet.op() == REPLY)
                    && (packet.tha() == _nic->address())) {
                db<Network>(INF)
                    << "ARP::update: ARP reply received!\n";

                _arpt.update(packet.spa(), packet.sha());
            }
        } else
            db<Network>(WRN)
                << "Network::update:unknown packet type (" << proto << ")\n";
    }

private:
    Table _arpt;
};

__END_SYS

#endif /* __network_service_h */

#endif /* __arp_h */

