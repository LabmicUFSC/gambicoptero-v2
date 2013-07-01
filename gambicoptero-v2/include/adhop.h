#ifndef __adhop_h
#define __adhop_h

#include <network_service.h>

#ifdef __network_service_h

#include <alarm.h>
#include <system/kmalloc.h>
#include <utility/hash.h>
#include <utility/handler.h>

#define MAX_LIST 3
#define MAX_HASH 5

__BEGIN_SYS

template<typename Link_Layer, typename Network_Layer>
class ADHOP: public Network_Service_Common<Link_Layer, Network_Layer>
{
public:
    typedef Network_Service_Common<Link_Layer, Network_Layer> Base;

    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned long u32;
    typedef unsigned long long u64;

    using Base::_nic;
    using Base::_network;
    using Base::_my_nic_address;
    using Base::_my_network_address;

    typedef typename Base::Link_Address Link_Address;
    typedef typename Base::Network_Address Network_Address;

    typedef typename Base::Protocol Protocol;

    typedef typename Network_Layer::Header Network_Header;

    static const unsigned int INITIAL_PHEROMONE = 100;
    static const unsigned int TIMER_CHECK_ROUTING_TABLES = 10000000; // 10s

    ADHOP(Link_Layer* nic, Network_Layer* network):
        Base(nic, network), seqNO(0)
    {
        update_handler = new(kmalloc(sizeof(Functor_Handler<RTable>)))
            Functor_Handler<RTable>(&RTable::update, &_rtable);
        update_alarm = new(kmalloc(sizeof(Alarm)))
            Alarm(TIMER_CHECK_ROUTING_TABLES, update_handler, Alarm::INFINITE);
    }

    ~ADHOP()
    {
        kfree(update_alarm);
        kfree(update_handler);
    }

    // Ant types
    enum {
        ITA = 0,
        ETA = 1
    };

    class Control_Info // Ant
    {
    public:
        u32 type:1;
        u32 returning:1;
        u32 sequenceNO:14;
        u32 heuristic:16;
        Link_Address neighbor;
    } __attribute__((packed,__may_alias__));

private:
    class RTable_Neighbor
    {
    public:
        typedef
            List_Elements::Singly_Linked_Ordered<RTable_Neighbor, u16> Element;

        RTable_Neighbor(const Network_Address& la, const Link_Address& pa):
            _link(this, INITIAL_PHEROMONE)
        {
            this->restart();
            _destination_node = la;
            _neighbor = pa;
        }

        bool to_clear()
        {
            if(_link.rank() > 4)
                return false;
            else
                return true;
        }

        void evaporate() //EVAPORATION
        {
            _link.rank((u16) (_link.rank() >> 1));
        }
        void update(u16 heuristic) //UPDATE_PHEROMONE
        {
            if (!heuristic)
                _link.rank( _link.rank() + (u16) (_link.rank() >> 1));
        }

        bool match(const Network_Address& la)
        {
            return (la == _destination_node);
        }
        bool match(const Network_Address& la, const Link_Address& pa)
        {
            return ((la == _destination_node) && (pa == _neighbor));
        }

        bool verify(u16 seq)
        {
            if (seq > this->_seqNO) {
                this->_seqNO = seq;
                return true;
            } else {
                if (seq == 0) {
                    this->restart();
                }
                return false;
            }
            return false;
        }

    private:
        void restart()
        {
            _seqNO = 0;
        }

        u16 _seqNO:14;

    public:
        Link_Address _neighbor;
        Network_Address _destination_node;
        Element _link;
    };

    class RTable_Neighbors:
        Simple_List<RTable_Neighbor, typename RTable_Neighbor::Element>
    {
    private:
        typedef
            Simple_List<RTable_Neighbor, typename RTable_Neighbor::Element>
            Base;
        typedef typename Base::Object_Type Object_Type;
        typedef typename Base::Element Element;

    public:
        RTable_Neighbors(): Base()
        {}

        void clear()
        {
            while(Base::size()) {
                Element* element = Base::remove();
                if(element) {
                    kfree(element->object());
                }
            }
        }

        void insert(Element * e)
        {
            if(Base::size() >= MAX_LIST) {
                Element* element = Base::remove_tail();
                if(element) {
                    kfree(element->object());
                }
            }
            if(Base::empty())
                insert_first(e);
            else {
                Element * next, * prev;
                for(
                        next = Base::head(), prev = 0;
                        (next->rank() > e->rank()) && next->next();
                        prev = next, next = next->next()
                   );
                if(next->rank() > e->rank()) {
                    insert_tail(e);
                } else if(!prev) {
                    insert_head(e);
                } else {
                    Base::insert(e, prev, next);
                }
            }
        }

        void update()
        {
            for(Element* e = Base::head(); e; e = e->next()) {
                if (e->object()->to_clear()) {
                    Base::remove(e);
                    kfree(e->object());
                    e = Base::head();
                } else {
                    e->object()->evaporate();
                }
            }
        }

        void update(const Network_Address& la,
                const Link_Address& pa, u16 heuristic)
        {
            Element* element = search_entry(la, pa);
            if(element) {
                Base::remove(element);
                element->object()->update(heuristic);
                this->insert(element);
            } else {
                Object_Type* neighbor =
                    new(kmalloc(sizeof(Object_Type))) Object_Type(la, pa);
                this->insert(&(neighbor->_link));
            }
        }

        Element* search_entry(const Network_Address& la,
                const Link_Address& pa)
        {
            Element * e;
            for(e = Base::head();
                    e && !e->object()->match(la, pa);
                    e = e->next());
            return e;
        }

        Link_Address search_address(const Network_Address& la)
        {
            Element * e;
            for(e = Base::head(); e && !e->object()->match(la); e = e->next());
            if(e)
                return e->object()->_neighbor;
            else
                return Link_Layer::BROADCAST;
        }

        bool verify(const Network_Address& la, u16 seq)
        {
            Element * e;
            for(e = Base::head(); e && !e->object()->match(la); e = e->next());
            if(e)
                return e->object()->verify(seq);
            else
                return true;
        }
    };

    class RTable:
        Hash<RTable_Neighbor,
            MAX_HASH,
            int,
            typename RTable_Neighbor::Element,
            RTable_Neighbors>
    {
    private:
        typedef
            Hash<RTable_Neighbor,
                MAX_HASH,
                int,
                typename RTable_Neighbor::Element,
                RTable_Neighbors>
            Base;

    public:
        typedef typename Base::Element Element;
        typedef typename Base::List List;

        RTable()
        {}

        ~RTable()
        {
            for(int i = 0; i < MAX_HASH; i++) {
                List * list = (*this)[i];
                list->clear();
            }
        }

        static void update(RTable* __rtable) {
            __rtable->update();
            return;
        }

        void update()
        {
            for(int i = 0; i < MAX_HASH; i++) {
                List * list = (*this)[i];
                list->update();
            }
        }

        void update(const Network_Address& la,
                const Link_Address& pa, u16 heuristic = 0)
        {
            List * list = (*this)[la];
            list->update(la, pa, heuristic);
        }

        Link_Address search(const Network_Address& la)
        {
            List * list = (*this)[la];
            return list->search_address(la);
        }

        bool verify(const Network_Address& la, u16 seq)
        {
            List * list = (*this)[la];
            return list->verify(la, seq);
        }
    };

private:
    bool update(const Network_Address& src, Control_Info* ant)
    {
        if (_rtable.verify(src, ant->sequenceNO)) {
            _rtable.update(src, ant->neighbor, ant->heuristic);
            if (ant->type == ETA && ant->returning == 0)
            {
                ant->neighbor = _my_nic_address;
                ant->returning = 1;
                return true;
            }
        }
        return false;
    }

    Link_Address resolve(const Network_Address& dst, Control_Info* ant)
    {
        Link_Address nic_addr = _rtable.search(dst);
        if (nic_addr == Link_Layer::BROADCAST) {
            ant->type = ETA;
        } else {
            ant->type = ITA;
        }
        ant->sequenceNO = seqNO++;
        ant->neighbor = _my_nic_address;
        ant->heuristic = 0;
        ant->returning = 0;
        return nic_addr;
    }

    Link_Address resolve(const Network_Address& src,
            const Network_Address& dst, Control_Info* ant)
    {
        if (_rtable.verify(src, ant->sequenceNO)) {
            _rtable.update(src, ant->neighbor, ant->heuristic);
            switch ((int) ant->type) {
                case ITA:
                    return receiving_ITA(src, dst, ant);
                case ETA:
                    return receiving_ETA(src, dst, ant);
                default:
                    return Link_Layer::BROADCAST;
            }
        }
        return Link_Layer::BROADCAST;
    }

    Link_Address receiving_ITA(const Network_Address& src,
            const Network_Address& dst, Control_Info* ant)
    {
        ant->neighbor = _my_nic_address;
        ant->heuristic = 0;
        Link_Address neighbor = _rtable.search(dst);
        if (neighbor == Link_Layer::BROADCAST) {
            ant->type = ETA;
            ant->sequenceNO = seqNO++;
            return Link_Layer::BROADCAST;
        }
        return neighbor;
    }

    Link_Address receiving_ETA(const Network_Address& src,
            const Network_Address& dst, Control_Info* ant)
    {
        ant->neighbor = _my_nic_address;
        ant->heuristic = 0;
        return _rtable.search(dst);
    }

public:
    void update(const Network_Address& la, const Link_Address& pa)
    {
        db<Network>(TRC) << "ADHOP::update\n";

        _rtable.update(la, pa, 0);
    }

    Link_Address resolve(const Network_Address& la, const char * pdu)
    {
        db<Network>(TRC) << "ADHOP::resolve\n";

        Control_Info * ant =
            (Control_Info *) ((Network_Header *) pdu)->options();
        Link_Address result = resolve(la, ant);
        return result;
    }

    virtual Network_Address resolve(const Link_Address& pa)
    {
        // Do nothing
        return Network_Address(0,0,0,0);
    }

    void received(const Link_Address& pa, Protocol proto,
            const char * data, int size)
    {
        db<Network>(TRC) << "ADHOP::received\n";

        int stat;

        Control_Info * ant =
            (Control_Info *) ((Network_Header *) data)->options();
        Network_Address net_dst = ((Network_Header *) data)->dst();
        Network_Address net_src = ((Network_Header *) data)->src();
        Link_Address neighbor = ant->neighbor;

        if (net_dst == _my_network_address) {
            db<Network>(TRC) << "ADHOP::received - Destination!\n";

            if (update(net_src, ant)) {
                ((Network_Header *) data)->dst(net_src);
                ((Network_Header *) data)->src(net_dst);

                do {
                    stat = _nic->send(neighbor, proto,
                            data, sizeof(Network_Header));
                    Alarm::delay(200);
                } while ((stat == -1) || (stat == 16));

                db<Network>(TRC) << "ADHOP::received - Response sent!\n";

                if (stat != 11)
                    db<Network>(ERR)
                        << "ADHOP::received: Link address unknown!\n"
                        << "Link status: " << stat << "\n";
            }
        } else {
            db<Network>(TRC) << "ADHOP::received - Router!\n";

            Link_Address next_hop =
                resolve(net_src, net_dst, ant);

            do {
                stat = _nic->send(next_hop, proto, data, size);
                Alarm::delay(200);
            } while ((stat == -1) || (stat == 16));

            db<Network>(TRC) << "ADHOP::received - Forwarding!\n";

            if (stat != 11)
                db<Network>(ERR)
                    << "ADHOP::received: Link address unknown!\n"
                    << "Link status: " << stat << "\n";
        }
    }

private:
    RTable _rtable;
    u16 seqNO:14;

    Functor_Handler<RTable> * update_handler;
    Alarm * update_alarm;
};

__END_SYS

#endif /* __network_service_h */

#endif /* __adhop_h */

