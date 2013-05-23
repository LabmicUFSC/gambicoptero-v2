#ifndef __neighboring_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __neighboring_h

#include <radio.h>
#include <system/kmalloc.h>

__BEGIN_SYS

class Neighboring {
public:
    typedef Radio_Common::Address Address;

    class Node {
    public:
        Node(Address addr, unsigned int energy, unsigned int lqi, unsigned int rssi) :
            _address(addr), _energy(energy), _lqi(lqi), _rssi(rssi) {}

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

    private:
        Address _address;
        unsigned int _energy;
        unsigned int _lqi;
        unsigned int _rssi;
    };

    typedef List_Elements::Doubly_Linked<Node> Element;

    static Neighboring * get_instance() {
        if (!Traits<Neighboring>::enabled)
            return 0;

        if (!_instance)
            _instance = new(kmalloc(sizeof(Neighboring))) Neighboring();

        return _instance;
    }

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

    friend Debug &operator<< (Debug &out, Neighboring &n) {
        out << "\nNeighboring size: " << n._nodes.size() << " of " << n._size << "\n";
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
    Neighboring() {}

    static Neighboring * _instance;
    static const unsigned int _size = Traits<Neighboring>::MAX_NEIGHBORS;
    List<Node, Element> _nodes;
};

__END_SYS

#endif

