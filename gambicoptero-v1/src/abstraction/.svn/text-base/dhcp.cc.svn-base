#include <dhcp.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

__BEGIN_SYS

DHCP::Client::Client(UDP * udp)
    : UDP::Socket(UDP::Address(0,68),UDP::Address(~0,67), udp),
      _state(IDLE) 
    { }



void DHCP::Client::configure() {
    db<IP>(INF) << "DHCP::Client sending discover msg\n";
    _state = DISCOVER;

    DHCP::Discover pkt(udp()->ip());
    SegmentedBuffer sb(&pkt,sizeof(DHCP::Discover));

    _xid = pkt.xid();
    send(&sb);
}

void DHCP::Client::parse_options(const Packet<255> * packet) {
    db<IP>(TRC) << "DHCP::Parsing OPTIONS\n";

    u8 * opt = packet->options();
    int i;

    for(i=0;i < 255;i++) {
        switch(opt[i]) {
        case 0: // padding
            break;
        case 1: // netmask
            ++i;
            if (opt[i] == 4) // IPv4, good
            {
               _mask = IP::Address(opt[i+1],opt[i+2],opt[i+3],opt[i+4]); 
                db<IP>(TRC) << "Found netmask " << _mask << endl;
            }
            i += opt[i];
            break;
        case 3: // routers
            ++i;
            if (opt[i] >= 4) // one or more, let's get the first
            {
               _gw = IP::Address(opt[i+1],opt[i+2],opt[i+3],opt[i+4]); 
                db<IP>(TRC) << "Found gateway " << _gw << endl;
            }
            i += opt[i];
            break;
        case 6: // nameserver
            ++i;
            if (opt[i] >= 4) // same logic as routers
            {
               _ns = IP::Address(opt[i+1],opt[i+2],opt[i+3],opt[i+4]); 
                db<IP>(TRC) << "Found nameserver " << _ns << endl;
            }
            i += opt[i];
            break;
        case 28: // broadcast address
            ++i;
            if (opt[i] == 4) // IPv4, good
            {
               _bcast = IP::Address(opt[i+1],opt[i+2],opt[i+3],opt[i+4]); 
                db<IP>(TRC) << "Found bcast " << _bcast << endl;
            }
            i += opt[i];
            break;
        case 51: // lease time in secs
            ++i;
            if (opt[i] == 4) { // Good size!
                _lease_time = (((u32)(opt[i+1]) << 24) & 0xFF000000) |
                              (((u32)(opt[i+2]) << 16) & 0x00FF0000) |
                              (((u32)(opt[i+3]) << 8 ) & 0x0000FF00) |
                              (((u32)(opt[i+4])      ) & 0x000000FF);
                db<IP>(TRC) << "Lease time " << _lease_time << endl;
            }
            i += opt[i];
            break;
        case 255: // end
            i = 500; // get out of the loop
            {
                db<IP>(TRC) << "End of options " << endl;
            }
            break;
        default:
            {
            db<IP>(TRC) << "Skipping code " << (int)opt[i]  << " len: " << opt[i+1] << endl;
                i += opt[i+1] + 1;
            }

        }
    }
}

void DHCP::Client::received(const UDP::Address & src,
                            const char *data, unsigned int size)
{
    db<IP>(INF) << "DHCP::Client state: " << _state << "\n";
    const DHCP::Packet<255> * packet = reinterpret_cast<const DHCP::Packet<255> *>(data);

    if (_xid != packet->xid()) {
        db<IP>(TRC) << "This DHCP message does not belong to me\n";
    }

    switch(_state) {
    case DISCOVER:

        if (packet->your_address()) {
            remote(src);

            UDP::Address me(packet->your_address(),68);
        //  set_local(me);
        //  _udp->ip()->set_address(me.ip());

            _state = REQUEST;
            db<IP>(INF) << "Server " << src.ip() << " offered IP " << me.ip() << "\n";
            parse_options(packet);

            DHCP::Request req(udp()->ip(),packet);
            SegmentedBuffer sb(&req,sizeof(DHCP::Request));
            send(&sb);
        }

        break;

    case REQUEST:

        if (packet->your_address()) {
            _ip = IP::Address((u32)packet->your_address());

            parse_options(packet);
            _state = RENEW;

        }
    }
}

__END_SYS
