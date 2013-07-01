// EPOS Router - Network Abstraction

#ifndef __router_h
#define __router_h

#include <network_protocol.h>

#ifdef __network_protocol_h

#include <machine.h>
#include <utility/queue.h>

__BEGIN_SYS

class Router: public Network_Common
{
public:
    using Network_Common::_nic;
    using Network_Common::_prot;
    using Network_Common::_address;

    typedef
        Service<Traits<Network>::SERVICE>::Network_Service<NIC,Router>
        Network_Service;

private:
    class Data
    {
    public:
        typedef Queue<Data,true>::Element Element;

        Data(int size, const Address & from, const char * data):
            _size(size), _from(from), e(this)
        {
            for (int i = (int) sizeof(Header);
                    i < (size + (int) sizeof(Header)); i++)
                _data[i - (int) sizeof(Header)] = data[i];
        }

        ~Data() {}

        int size() { return _size; }
        const Address & from() { return _from; }
        char * data() { return _data; }

    private:
        int _size;
        Address _from;
        char _data[118];

    public:
        Element e;
    };

public:
    class Header
    {
    public:
        Header() {}
        ~Header() {}

        typedef Network_Service::Control_Info Control_Info;

        Address dst() { return _dst; }
        void dst(const Address & __dst) { _dst = __dst; }
        Address src() { return _src; }
        void src(const Address & __src) { _src = __src; }
        Control_Info * options() { return &_options; }

    private:
        Address _dst;
        Address _src;

        Control_Info _options;
    } __attribute__((packed,__may_alias__));

    Router(unsigned int unit = 0):
        Network_Common(unit),
        _network_service(&_nic, this)
    {
        db<Network>(TRC)
            << "Router(unit = 0)\n";

        _nic.attach(this, P_ROUTER);
    }

    ~Router()
    {
        db<Network>(TRC)
            << "~Router()\n";
    }

    int send(const Address & to, const char * data, unsigned int size)
    {
        db<Network>(TRC) << "Router::send\n";

        if (size + sizeof(Header) > _nic.mtu())
            db<Network>(WRN)
                << "Router::send: packet size exceeds MTU!\n";

        char packet[sizeof(Header) + size];

        for (unsigned int i = sizeof(Header); i < sizeof(Header) + size; i++)
            packet[i] = data[i - sizeof(Header)];

        ((Header *) packet)->src(_address);
        ((Header *) packet)->dst(to);

        NIC::Address dst = _network_service.resolve(to, packet);

        return _nic.send(dst, P_ROUTER, packet, sizeof(Header) + size);
    }

    int receive(Address * from, char * data, unsigned int size)
    {
        db<Network>(TRC) << "Router::receive\n";

        if (size > _nic.mtu())
            db<Network>(WRN)
                << "Router::receive: packet size exceeds MTU!\n";

        if (_buffer.empty())
            return 0;

        Data::Element * e = _buffer.remove();
        Data * _data = (Data *) e->object();

        int data_size = _data->size();
        *from = _data->from();
        char * __data = _data->data();

        for (int i = 0; i < data_size; i++)
            data[i] = __data[i];

        e = 0;
        kfree(_data);

        return data_size;
    }

    void update(Observed * o, int p)
    {
        db<Network>(TRC) << "Router::update\n";

        int size = _nic.mtu();
        char pdu[size];

        NIC::Address src;
        Protocol prot = 0;

        int packet_size = _nic.receive(&src, &prot, pdu, size);

        if (prot != P_ROUTER)
            return;

        if (packet_size < 0)
            return;

        db<Network>(TRC) << "Router::update: received packet!\n";

        if (((packet_size - (int) sizeof(Header)) > size)) {
            db<Network>(WRN)
                << "Router::receive: "
                << "data packet size exceeds size of 'data' argument!\n";
            db<Network>(WRN)
                << "Router::receive: data packet size = "
                << packet_size << " bytes!\n";
            return;
        }

        if ((packet_size < (int) sizeof(Header))) {
            db<Network>(WRN)
                << "Router::receive: "
                << "data packet corrupted!\n";
            db<Network>(WRN)
                << "Router::receive: data packet size = "
                << packet_size << " bytes!\n";
            return;
        }

        Address net_dst = ((Header *) pdu)->dst();
        Address net_src = ((Header *) pdu)->src();

        _network_service.received(src, prot, pdu, packet_size);

        if ((net_dst == _address)
                && (prot == P_ROUTER)
                && (packet_size > (int) sizeof(Header))) {
            Data * data = new(kmalloc(sizeof(Data)))
                Data((packet_size - (int) sizeof(Header)), net_src, pdu);
            _buffer.insert(&(data->e));

            db<Network>(TRC) << "Router::notify\n";
            notify((int) _prot);
        }

        prot = 0;
    }

    const Address & address() { return _address; }

    void address(const Address & address) {
        _address = address;
        _network_service.address(address);
    }

private:
    Queue<Data,true> _buffer;
    Network_Service _network_service;
};

__END_SYS

#endif /* __network_protocol_h */

#endif /* __router_h */

