#include <ip.h>

#ifdef __NIC_H

__BEGIN_SYS

IP* IP::_instance[Traits<NIC>::NICS::Length];
const IP::Address IP::NULL = IP::Address((u32)0);

u16 IP::Header::pktid = 0; // incremental packet id

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
        _packet_receive[i] = new (kmalloc(mtu())) char[mtu()];

    _packet_send = new (kmalloc(mtu())) char[mtu()];

    start();
}

IP::~IP() {
    for(unsigned int i=0;i<MAX_FRAGMENTS;++i)
        kfree(_packet_receive[i]);

    kfree(_packet_send);
}

void IP::process_ip(char *data, u16 size)
{
    Header &pck_h = *reinterpret_cast<Header*>(data);
    if((u32)_self != (u32)0 && // We MUST accept anything if our IP address is not set
       (u32)(pck_h.dst()) != (u32)(_self) &&
       (u32)(pck_h.dst()) != (u32)(_broadcast))
    {
        db<IP>(INF) << "IP Packet discarded. dst= " << pck_h.dst() << "\n";
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
        notify(pck_h.src(),pck_h.dst(),(int)pck_h.protocol(),
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
        int size = _nic.receive(&src, &prot, _packet_receive[0], _nic.mtu());

        if(size <= 0) {
            //db<IP>(WRN) << "NIC::received error!" << endl;
            Thread::self()->yield();
            continue;
        }

        if (prot == P_IP) {
            _network_service.update(reinterpret_cast<Header*>(_packet_receive[0])->src(), src);
            process_ip(_packet_receive[0], size);
        }

        // notify routing algorithm
        _network_service.received(src, prot, _packet_receive[0], size);
        
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
        mac = _network_service.resolve(to,pdu.data());
    else
        mac = _network_service.resolve(_gateway,pdu.data());

    //TODO: put fragmentation here
    int size = pdu.total_size();
    pdu.copy_to(_packet_send,size);

    db<IP>(TRC) << "IP::send() " << size << " bytes" << endl;

    int retry = 5, ret;

    do {
        ret = _nic.send(mac,P_IP,_packet_send,size);
        if (ret >= 0)
            return size;

        Thread::self()->yield();
    } while (retry-- > 0);

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

#endif
