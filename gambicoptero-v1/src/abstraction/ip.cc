#include <ip.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

__BEGIN_SYS

IP* IP::_instance[Traits<NIC>::NICS::Length];
const IP::Address IP::NULL = IP::Address((u32)0);

u16 IP::Header::pktid = 0; // incremental packet id

// IP::Address

IP_Address::IP_Address(const char * _addr) {
    unsigned char addr[4];
    addr[0] = 0; addr[1] = 0; addr[2] = 0; addr[3] = 0;
    int i;
    for(i=0;i<4;++i) {
        char * sep = strchr(_addr,'.');
        addr[i] = atol(_addr);
        if (!sep) break;
        _addr = ++sep;
    }
    memcpy(this,addr,sizeof(this));
}

char* IP_Address::to_string(char * dst) {
    const u8 * _addr = reinterpret_cast<const u8*>(this);
    char* p = dst;
    for(int i=0;i<4;i++) {
        p += utoa(_addr[i], p);
        *p++ = '.';
    }
    // remove last dot
    --p;
    *p = 0;
    return p;
}

Debug& operator<<(Debug& db,const IP_Address& addr) {
   const u8 * _addr = reinterpret_cast<const u8*>(&addr);
   db << dec << (int)(_addr[0]) << "." << (int)(_addr[1])
      << "." << (int)(_addr[2]) << "." << (int)(_addr[3]);
   return db;
}

// IP::Header

void IP::Header::calculate_checksum() {
    _checksum = 0;
    _checksum = ~(IP::calculate_checksum(this, hlength()));
}

// IP
IP::IP(unsigned int unit)
    : _nic(unit),
      _network_service(&_nic, this),
      _self(IP::NULL),
      _broadcast(255,255,255,255),
      _thread(0)
{
    if (_instance[unit])
    {
        db<IP>(ERR) << "IP::created IP object twice for the same NIC!";	
    }
    _network_service.update(_broadcast, NIC::BROADCAST);

    if (CONFIG == STATIC) {
        _self = Address(ADDRESS);
        _network_service.update(_self,_nic.address());
        _broadcast = Address(BROADCAST);
        _network_service.update(_broadcast, NIC::BROADCAST);
        _netmask = Address(NETMASK);
    }

    _instance[unit] = this;

    // allocate memory for receiving packets
    for(unsigned int i=0;i<MAX_FRAGMENTS;++i)
        _packet[i] = new (kmalloc(mtu())) char[mtu()];

    start();
}

IP::~IP() {
    for(unsigned int i=0;i<MAX_FRAGMENTS;++i)
        kfree(_packet[i]);
}

void IP::process_ip(char *data, u16 size)
{
    Header &pck_h = *reinterpret_cast<Header*>(data);
    if((u32)_self != (u32)0 && // We MUST accept anything if our IP address is not set
       (u32)(pck_h.dst_ip()) != (u32)(_self) &&
       (u32)(pck_h.dst_ip()) != (u32)(_broadcast))
    {
        db<IP>(INF) << "IP Packet discarded. dst= " << pck_h.dst_ip() << "\n";
        return;
    }
    else {
        db<IP>(TRC) << "IP: " << pck_h << "\n" ;
    }

    if(!fragmentation && (pck_h.flags() == Header::MF_FLAG || pck_h.offset() != 0))
    {
        db<IP>(INF) << "IP::Fragmented packet discarded\n";
        return;
    }

    if (calculate_checksum(data,pck_h.hlength()) != 0xFFFF) {
        db<IP>(TRC) << "IP checksum failed for incoming packet\n";
    } else {
        notify(pck_h.src_ip(),pck_h.dst_ip(),(int)pck_h.protocol(),
                &data[pck_h.hlength()], pck_h.length() - pck_h.hlength());
        if (pck_h.ttl() > 0) {
            pck_h.ttl(pck_h.ttl() - 1);
        }
    }

}

int IP::run()
{
    db<IP>(TRC) << __PRETTY_FUNCTION__ << endl;
    NIC::Address src;
    NIC::Protocol prot;

    while (true) {
        int size = _nic.receive(&src, &prot, _packet[0], _nic.mtu());
        if(size <= 0) {
            db<IP>(WRN) << "NIC::received error!" << endl;
            Thread::self()->yield();
            continue;
        }

        if (prot == NIC::IP) {
            _network_service.update(reinterpret_cast<Header*>(_packet[0])->src_ip(), src);
            process_ip(_packet[0], size);
        }

        // notify routing algorithm
        _network_service.received(src, prot, _packet[0], size);
        
        Thread::yield();
    }

    return 0;
}

s32 IP::send(const Address & from,const Address & to,SegmentedBuffer * data,Protocol proto)
{
    Header hdr(from,to,proto,data->total_size());
    SegmentedBuffer pdu(&hdr,hdr.hlength(),data);

    MAC_Address mac = NIC::BROADCAST;
    if (from.is_neighbor(to,_netmask))
        mac = _network_service.resolve(to,&pdu);
    else
        mac = _network_service.resolve(_gateway,&pdu);

    //TODO: put fragmentation here
    int size = pdu.total_size();
    char sbuf[size];
    //TODO: possible stack overflow here, we must change NIC::send to accept SegmentedBuffers
    pdu.copy_to(sbuf,size);   

    db<IP>(TRC) << "IP::send() " << size << " bytes" << endl;

    if (_nic.send(mac,NIC::IP,sbuf,size) >= 0)
        return size;
    else
        return -1;
}

// From http://www.faqs.org/rfcs/rfc1071.html
u16 IP::calculate_checksum(const void* ptr, u16 count)
{
    u32 sum = 0;

    const unsigned char * _ptr = reinterpret_cast<const unsigned char *>(ptr);
    u16 i;

    for(i = 0; i < count-1; i+=2)
        sum += (((unsigned short)(_ptr[i+1]) & 0x00FF) << 8) | _ptr[i];
    if(count & 1) {
        sum += _ptr[count-1];
    }

    while(sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    return sum;
}

__END_SYS

