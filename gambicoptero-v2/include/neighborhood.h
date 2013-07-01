#ifndef __neighborhood_h
#define __neighborhood_h

#include <radio.h>
#include <system/kmalloc.h>

__BEGIN_SYS

class Neighborhood {
public:
    typedef Radio_Common::Address Address;

    class Node {
    public:
        Node(Address addr, unsigned int energy, unsigned int lqi, unsigned int rssi) :
            _address(addr),
            _energy(energy),
            _lqi(lqi),
            _rssi(rssi),
            _expire(Traits<Neighborhood>::EXPIRE)
        {}

        ~Node() {}

        Address get_address() {
            return _address;
        }

        unsigned int get_energy() {
            return _energy;
        }

        unsigned int get_lqi() {
            return _lqi;
        }

        unsigned int get_rssi() {
            return _rssi;
        }

        void set_energy(unsigned int val) {
            _energy = val;
        }

        void set_lqi(unsigned int val) {
            _lqi = val;
        }

        void set_rssi(unsigned int val) {
            _rssi = val;
        }

        void decrease_expire() {
            _expire--;
        }

        bool expired() {
            return (_expire == 0);
        }

    private:
        Address _address;
        unsigned int _energy;
        unsigned int _lqi;
        unsigned int _rssi;
        int _expire;
    };

    typedef List_Elements::Doubly_Linked<Node> Element;

    static Neighborhood * get_instance() {
        if (!Traits<Neighborhood>::enabled)
            return 0;

        if (!_instance)
            _instance = new(kmalloc(sizeof(Neighborhood))) Neighborhood();

        return _instance;
    }

    int neighborhood(short * data, const unsigned int size) {
        update();

        if (_nodes.empty())
            return 0;

        unsigned int i;
        List_Iterators::Bidirecional<Element> it;
        for (i = 1, it = _nodes.begin(); (it != _nodes.end()) && (i < size); i++, it++) {
            data[i - 1] = (short) it->object()->get_address();
        }
        return i - 1;
    }

    void update() {
        List_Iterators::Bidirecional<Element> it;
        for (it = _nodes.begin(); it != _nodes.end(); it++) {
            it->object()->decrease_expire();
        }
        for (it = _nodes.begin(); it != _nodes.end(); it++) {
            if (it->object()->expired()) {
                remove_node(it->object());
                it = _nodes.begin();
            }
        }
    }

    void update(Node * n) {
        add_node(n);
        //update();
    }

    int get_node_energy(Address addr) {
        Node * n = 0;
        if ((n = search(addr)))
            return n->get_energy();

        return 0;
    }

    int get_node_lqi(Address addr) {
        Node * n = 0;
        if ((n = search(addr)))
            return n->get_lqi();

        return 0;
    }

    int get_node_rssi(Address addr) {
        Node * n = 0;
        if ((n = search(addr)))
            return n->get_rssi();

        return 0;
    }

    friend Debug &operator<< (Debug &out, Neighborhood &n) {
        out << "\nNeighborhood size: " << n._nodes.size() << " of " << n._size << "\n";
        if (n._nodes.empty())
            return out;

        out << "LRU node list:\n";

        List_Iterators::Bidirecional<Element> it;
        for (it = n._nodes.begin(); it != n._nodes.end(); it++) {
            out << "--------------------\n";
            out << "Node address: " << it->object()->get_address() << "\n";
            out << "Remaining energy: " << it->object()->get_energy() << "\n";
            out << "LQI: " << it->object()->get_lqi() << "\n";
            out << "RSSI: " << it->object()->get_rssi() << "\n";
        }
        out << "--------------------\n";

        return out;
    }

private:
    void add_node(Node * n) {
        remove_node(n);

        Element * e = new(kmalloc(sizeof(Element))) Element(n);
        _nodes.insert_head(e);

        if (_nodes.size() > _size)
            _nodes.remove_tail();
    }

    void remove_node(Node * n) {
        if (_nodes.empty())
            return;

        List_Iterators::Bidirecional<Element> it;
        for (it = _nodes.begin(); it != _nodes.end(); it++) {
            if (it->object()->get_address() == n->get_address()) {
                _nodes.remove(it);
                kfree(it->object());
                kfree(it);
                return;
            }
        }
    }

    Node * search(Address address) {
        if (_nodes.empty())
            return 0;

        List_Iterators::Bidirecional<Element> it;
        for (it = _nodes.begin(); it != _nodes.end(); it++) {
            if (it->object()->get_address() == address)
                return it->object();
        }

        return 0;
    }


private:
    Neighborhood() {}

    static Neighborhood * _instance;
    static const unsigned int _size = Traits<Neighborhood>::MAX_NEIGHBORS;
    List<Node, Element> _nodes;
};

__END_SYS

#endif /* __neighborhood_h */

