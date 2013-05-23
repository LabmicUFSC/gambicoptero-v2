#include <udp.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/string.h> // for Address constructor

__BEGIN_SYS

// UDP::Address
UDP_Address::UDP_Address(const char *addr) : _ip(addr)
{
    char *sep = strchr(addr,':');
    if (sep) {
        _port = atol(++sep);
    } else {
        _port = 0;
    }
}

char* UDP_Address::to_string(char * dst)
{
    char *p = _ip.to_string(dst);
    *p++ = ':';
    p += utoa(_port,p);
    *p = 0;
    return p;
}


// TCP's checksum was added to UDP, we should merge this to avoid code redundancy
void UDP::Header::checksum(IP::Address src,IP::Address dst,SegmentedBuffer * sb)
{
    if (!Traits<UDP>::checksum) {
        _checksum = 0;
        return;
    }

    db<UDP>(TRC) << __PRETTY_FUNCTION__ << endl;
    u16 len;
    len = sizeof(this);

    if (sb) len += sb->total_size();

    Pseudo_Header phdr((u32)src,(u32)dst,len);

    _checksum = 0;

    u32 sum = 0;

    sum = IP::calculate_checksum(&phdr, sizeof(phdr));
    sum += IP::calculate_checksum(this, sizeof(this));

    while (sb) {
        sum += IP::calculate_checksum(sb->data(), sb->size());
        sb = sb->next();
    }
    
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    _checksum = ~sum;
}

UDP::UDP(IP * _ip) : Base(_ip) {
    ip()->attach(this, ID_UDP);
}

UDP::~UDP() {
    ip()->detach(this, ID_UDP);
}

UDP::Socket::Socket(Address local, Address remote, UDP * _udp)
    : Base(_udp), _local(local), _remote(remote)
{
    udp()->attach(this, _local.port());
}

UDP::Socket::~Socket() {
    udp()->detach(this, _local.port());
}

// Assembles data and sends to IP layer

s32 UDP::send(Address _local, Address _remote, SegmentedBuffer * data) {
    UDP::Header hdr(_local.port(), _remote.port(),
                    data->total_size());
    SegmentedBuffer sb(&hdr, sizeof(UDP::Header), data);
    hdr.checksum(_local.ip(),_remote.ip(),&sb);
    return ip()->send(_local.ip(), _remote.ip(), &sb, ID_UDP) - 8;	// discard header
}

// Called by IP's notify(...)

void UDP::update(Data_Observed<IP::Address> *ob, long c, IP::Address src,
                 IP::Address dst, void *data, unsigned int size)
{
    Header& hdr = *reinterpret_cast<Header*>(data);

    db<UDP>(INF) << "UDP::update: received "<< size <<" bytes from " 
                 << src << " to " << dst << "\n";

    if (Traits<UDP>::checksum && hdr._checksum != 0) {
        SegmentedBuffer sb(static_cast<char*>(data) + sizeof(Header), size - sizeof(Header));
        u16 csum = hdr._checksum;
        hdr.checksum(src,dst,&sb);
        if (hdr._checksum != csum) {
            db<UDP>(INF) << "UDP::checksum failed for incomming data\n";
            return;
        }
    }
    notify(UDP::Address(src,hdr.src_port()),UDP::Address(dst,hdr.dst_port()),
           (int) hdr.dst_port(), &((char*)data)[sizeof(Header)],
           size - sizeof(Header));
}

UDP * UDP::instance(unsigned int i) {
    static UDP * _instance[Traits<NIC>::NICS::Length];
    if (!_instance[i])
        _instance[i] = new UDP(IP::instance(i));
    return _instance[i];
}

// Called by UDP's notify(...)

void UDP::Socket::update(Observed *o, long c, UDP_Address src, UDP_Address dst,
                         void *data, unsigned int size)
{
    // virtual call
    received(src,(const char*)data,size);
}

// UDP Channel

int UDP::Channel::receive(Address * from,char * buf,unsigned int size)
{
    _buffer_size = size;
    _buffer_data = buf;
    _buffer_src  = from;
    _buffer_wait.lock();
    _buffer_data = 0;
    return _buffer_size;
}

void UDP::Channel::received(const Address & src,
                                const char *data, unsigned int size)
{
    if (_buffer_data) {
        if (size < _buffer_size)
            _buffer_size = size;
        memcpy(_buffer_data, data, _buffer_size);
        memcpy(_buffer_src, &src, sizeof(Address));
        _buffer_wait.unlock();
    }
}

void UDP::Channel::update(Data_Observed<IP::Address> *ob, long c,
                          IP::Address src, IP::Address dst,
                          void *data, unsigned int size)
{
    ICMP::Packet& packet = *reinterpret_cast<ICMP::Packet*>(data);
    if (packet.type() == ICMP::UNREACHABLE)
    {
        IP::Header& ip_hdr = *reinterpret_cast<IP::Header*>(packet.data());
        if (ip_hdr.src_ip() != (u32)_local.ip() ||
            ip_hdr.dst_ip() != (u32)_remote.ip()) {
            return;
        }
        char * ip_data = (char*)data + ip_hdr.hlength();
        UDP::Header& udp_hdr = *reinterpret_cast<UDP::Header*>(ip_data);
        if (udp_hdr.src_port() != _local.port() ||
            udp_hdr.dst_port() != _remote.port()) {
            return;    
        }
        
        _error = ~(packet.code());
    }
}

UDP::Channel_MultiNIC::Channel_MultiNIC()
{
    int i;
    /* 
     * Attention here:
     * The correct is static_cast<>(), but it doesn't work
     * if we are in a single-NIC case. For this reason C-style cast 
     * is used since it falls back to reinterpret_cast and this
     * code will not be used in single-NIC scenario anyway.
     */
    IP * _ip = ((Channel*)this)->udp()->ip();
    for(i=0;i < Traits<NIC>::NICS::Length; ++i) 
    {
        if (IP::instance(i) == _ip) {
            _icmp = ICMP_MultiNIC::instance(i);
            break;
        }
    }
}

void UDP::Channel::operator()()
{
    _buffer_data = 0;
    _buffer_size = TIMEOUT;
    _buffer_wait.unlock();
}

UDP::Channel::Channel(const Address& local,const Address& remote)
//TODO: mult NIC support for channels
   : Socket(local, remote, 0), _error(0)
{
    icmp()->attach(this, ICMP::UNREACHABLE);
}

UDP::Channel::~Channel()
{
    if (_buffer_data) {
        db<UDP>(ERR) << "UDP::Channel for "<<this<<" destroyed while receiving\n";
        _buffer_size = DESTROYED;
        _buffer_wait.unlock();
    }
    icmp()->detach(this, ICMP::UNREACHABLE);
}

__END_SYS

