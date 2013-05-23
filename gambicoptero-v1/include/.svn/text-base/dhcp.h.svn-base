#ifndef DHCPC_H

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define DHCPC_H

#include "udp.h"
#include <utility/random.h>

__BEGIN_SYS
/*
 * Reference:
 * DHCP:         http://www.ietf.org/rfc/rfc2131.txt
 * DHCP options: http://www.ietf.org/rfc/rfc2132.txt
 */
class DHCP {
public:
    // Limited to opt_size = 308

    template<int opt_size> class Packet {
    public:
        u8 _op, _htype, _hlen, _hopts;
        u32 _xid;
        u16 _secs, _flags;
        u32 _ciaddr, _yiaddr, _siaddr, _giaddr;
        u8 _chaddr[16];
        u8 _sname[64];
        u8 _file[128];
        u8 _magic[4];
        u8 _options[opt_size];
        u8 _end;
        u8 _padding[312 - 5 - opt_size];

        u8 op() const { return _op; }
        u32 xid() const { return _xid; }
        u16 secs() const { return CPU::ntohs(_secs); }
        u32 your_address() const { return CPU::ntohl(_yiaddr); }
        u32 server_address() const { return CPU::ntohl(_siaddr); }
        u8 * options() const { return const_cast<u8 * const>(_options); }

        Packet()
        {
            memset(&_op, 0, sizeof(Packet));
            _magic[0] =  99; // magic cookie
            _magic[1] = 130;
            _magic[2] =  83;
            _magic[3] =  99;
            _end = 255; // end of options
        }

    };

    class Discover : public Packet<3> {
    public:
        Discover(IP * _net) : Packet<3>() {
            _op = 1;
            _htype = 1;
            _hlen = _net->hw_address_len();
            _xid = Pseudo_Random::random();
            memcpy(_chaddr, &_net->hw_address(), _hlen);
            _options[0] = 53; // DHCPMSG
            _options[1] = 1;  // message size
            _options[2] = 1;  // dhcp discover
        }

    };

    class Request : public Packet<8> {
    public:
        Request(IP * _net,const Packet<255> * discovered) : Packet<8>() {
            _op = 1;
            _htype = 1;
            _hlen = _net->hw_address_len();
            _xid = discovered->_xid;
            _ciaddr = discovered->_ciaddr;
            _siaddr = discovered->_siaddr;
            memcpy(_chaddr, &_net->hw_address(), _hlen);
            _options[0] = 53; // DHCP message
            _options[1] = 1;  // size
            _options[2] = 3;  // dhcp discover
            _options[3] = 55; // parameter request
            _options[4] = 3;  // size
            _options[5] = 1;  // subnet
            _options[6] = 3;  // router
            _options[7] = 6;  // dns
        }
    };

    class Client;
};

class DHCP::Client : public UDP::Socket {
public:
    enum {
        IDLE,
        DISCOVER,
        REQUEST,
        RENEW,
        RELEASE
    };
   
    Client(UDP * udp = 0);
    
    ~Client() {}

    void received(const UDP::Address & src,const char *data, unsigned int size);

    void configure();
    void parse_options(const Packet<255> * packet);
    void renew();
    void release();

    IP::Address address() { return _ip; }
    IP::Address netmask() { return _mask; }
    IP::Address gateway() { return _gw; }
    IP::Address brodcast() { return _bcast; }
    IP::Address nameserver() { return _ns; }
    
protected:
    short _state;
    u32 _xid;
    u32 _lease_time;
    IP::Address _ip, _mask, _gw, _bcast, _ns;
};

__END_SYS

#endif
