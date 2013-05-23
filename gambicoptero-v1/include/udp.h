#ifndef __udp_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __udp_h

#include <alarm.h>
#include <ip.h>
#include <icmp.h>
#include <utility/handler.h>
#include <mutex.h>

__BEGIN_SYS 

class UDP_Address {
public:
    /// Creates a NULL address.
    UDP_Address() { }
    
    /// Creates an UDP_Address from an ip number and port.
    UDP_Address(u32 ip, u16 port): _ip(ip), _port(port) { }
    
    /// Creates and UDP_Address from an IP::Address object and port number.
    UDP_Address(IP_Address ip, u16 port): _ip(ip), _port(port) { }
    
    /**
     * Creates and UDP_Address from its string representation in the
     * a.b.c.d:port format.
     */
    UDP_Address(const char *addr);

    
    /// Output the UDP_Address string in the format a.b.c.d:port to a stream. 
    template < typename T >
    friend T & operator <<(T & out, const UDP_Address & h) {
        out << dec << h.ip() << ":" << (unsigned int)(h.port());
        return out;
    }

    /// Write the string representation in the format a.b.c.d:port to _dst_.
    char* to_string(char * dst);

    /// Get port number.
    u16 port() const { return _port; }
    
    /// Get IP address.
    IP_Address ip() const { return _ip; }

    /**
     * Change port number, beware that a Channel using this Address
     * will not be be bound to this port unless you use UDP::Channel->local().
     */
    void port(u16 new_port) { _port = new_port; }
    
    /**
     * Change IP address for this UDP_Address, beware that a Channel
     * using this Address will not be be bound to this IP unless you 
     * use UDP::Channel->local().
     */
    void ip(const IP_Address& ip) { _ip = ip; }
    
    /// Compare this address to other and returns true if both are equal.
    bool operator==(const UDP_Address& other)
    {
        return ip() == other.ip() && port() == other.port();    
    }

private:
     
    IP_Address _ip;
    u16 _port;
};

// Optmization for single NIC support
class UDP_SingleNIC {
public:
    IP* ip() const { return IP::instance(); }
    
protected:
    UDP_SingleNIC(IP * ip) {}
};

// Generalization for multiple NICs support
class UDP_MultiNIC {
public:
    IP* ip() const { return _ip; }
    
protected:
    UDP_MultiNIC(IP * ip) : _ip(ip) {}
      
private:    
    IP * _ip;
};

class UDP : public IF<IP::multiNIC, UDP_MultiNIC, UDP_SingleNIC>::Result,
            public IP::Observer,
            public Data_Observed < UDP_Address > {
protected:
    typedef IF<IP::multiNIC, UDP_MultiNIC, UDP_SingleNIC>::Result Base;
    
public:
    // UDP ID (IP Frame)
    static const IP::Protocol ID_UDP = 0x11;

    typedef UDP_Address Address;

    class Header;
    class Socket;
    class Channel;
    
    class Socket_SingleNIC;
    class Socket_MultiNIC;
    
    class Channel_SingleNIC;
    class Channel_MultiNIC;

    UDP(IP * ip = 0);

    ~UDP();

    s32 send(Address local, Address remote, SegmentedBuffer * data);

    // Data_Observer callback
    void update(Data_Observed<IP::Address> *ob, long c, IP::Address src,
                IP::Address dst, void *data, unsigned int size);
    
    static UDP * instance(unsigned int i=0);

private:

    struct Pseudo_Header {
        u32 src_ip;
        u32 dst_ip;
        u8 zero;
        u8 protocol;
        u16 length;
        
        Pseudo_Header(u32 src,u32 dst,u16 len) 
        : src_ip(src), dst_ip(dst), zero(0), protocol(ID_UDP),
          length(CPU::htons(len)) {};
    };
};

class UDP::Header {
    friend class UDP;
public:
    Header() {}

    Header(u16 src_port = 0, u16 dst_port = 0, u16 data_size = 0)
    : _src_port(CPU::htons(src_port)),
      _dst_port(CPU::htons(dst_port)),
      _length(CPU::htons(sizeof(UDP::Header) + data_size)),
      _checksum(0) { }

    void checksum(IP::Address src,IP::Address dst,SegmentedBuffer * sb);

    u16 dst_port() const { return CPU::ntohs(_dst_port); }
    u16 src_port() const { return CPU::ntohs(_src_port); }

    friend Debug & operator <<(Debug & db, const Header & h) {
        db << "{sprt=" << CPU::ntohs(h._src_port)
            << ",dprt=" << CPU::ntohs(h._dst_port)
            << ",len=" << CPU::ntohs(h._length)
            << ",chk=" << (void *)h._checksum << "}";
        return db;
    }

private:
    u16 _src_port;  // Source UDP port
    u16 _dst_port;  // Destination UDP port
    u16 _length;    // Length of datagram (header + data) in bytes
    volatile u16 _checksum;  // Pseudo header checksum (see RFC)
};

// Socket optmization for single NIC scenario
class UDP::Socket_SingleNIC {
public:
    UDP* udp() const { return UDP::instance(); }
protected:
    Socket_SingleNIC(UDP * udp) {}
};

// Socket optmization for multiple NICs scenario
class UDP::Socket_MultiNIC {
public:
    UDP* udp() const { return _udp; }
protected:
    Socket_MultiNIC(UDP * udp) : _udp(udp) {
        if (!udp) _udp = UDP::instance();
    }   
private:    
    UDP * _udp;
};

class UDP::Socket : public IF<IP::multiNIC,
                              Socket_MultiNIC, Socket_SingleNIC>::Result,
                    public Data_Observer <UDP_Address>
{
    friend class UDP;
    typedef IF<IP::multiNIC,
               Socket_MultiNIC, Socket_SingleNIC>::Result Base;
public:
    Socket(Address local, Address remote, UDP * udp = 0);

    ~Socket();

    s32 send(const char *data, u16 size) const {
        SegmentedBuffer sb(data, size);
        return send(&sb);
    }
    s32 send(SegmentedBuffer * data) const {
        return udp()->send(_local, _remote, data);
    }

    void local(const Address & local) { _local = local; }
    void remote(const Address & party) { _remote = party; }

    const Address & remote() const { return _remote; }

    void update(Observed * o, long c, UDP_Address src, UDP_Address dst,
                void * data, unsigned int size);

    // every Socket should implement one
    virtual void received(const Address & src,
                          const char *data, unsigned int size) {};
protected:
    Address _local;
    Address _remote;
};

class UDP::Channel_SingleNIC {
public:
    ICMP * icmp() const { return ICMP::instance(); }
};

class UDP::Channel_MultiNIC {
public:
    ICMP * icmp() const { return _icmp; }
protected:
    Channel_MultiNIC();
private:
    ICMP * _icmp;
};

/**
 * The UDP::Channel is the top level class for using the UDP/IP protocol.
 * It has a simple send/receive interface with rendezvou semantics.
 */
class UDP::Channel : public Socket,
                     public IF<IP::multiNIC,
                               Channel_MultiNIC, Channel_SingleNIC>::Result,
                     public ICMP::Observer,
                     public Handler
{
public:
    /// Return conditions
    static const int DESTROYED  = -1;
    static const int TIMEOUT    = -2;
    
    
    /**
     * Return non-zero on error condition.
     * Error values are from ICMP::Packet unreachable codes.
     */
    unsigned char error() { if (_error) return ~_error; }
    
    /**
     * Wait for incoming data. A max of _size_ bytes will be written to _buf_.
     * Returns the amount received or a negative value in case of error.
     * Blocks indefinitely.
     */
    int receive(Address * from,char * buf,unsigned int size);
    
    /**
     * Wait for incoming data. A max of _size_ bytes will be written to _buf_.
     * Returns the amount received or a negative value in case of error.
     * Blocks for _timeout_ microseconds at max.
     */
    int receive(Address * from,char * buf,unsigned int size,
                const Alarm::Microsecond& timeout);
    
    /**
     * Sends _size_ bytes from _buf_ to destination _to_.
     */
    int send(const Address& to,const char * buf,unsigned int size);

    
    Channel(const Address& local,const Address& remote);
    
    /**
     * Destroys the channel. If a thread is blocked on a receive call it will
     * be released but the result can be catastrophic.
     */
    ~Channel();
    
    
protected:
    // Socket callback
    virtual void received(const Address & src,
                          const char *data, unsigned int size);

    // ICMP callback
    void update(Data_Observed<IP::Address> *ob, long c,
                IP::Address src, IP::Address dst,
                void *data, unsigned int size);
    
    // Handler callback
    void operator()();
    
    Address * _buffer_src;
    Mutex     _buffer_wait;
    unsigned int _buffer_size;
    char       * _buffer_data;
    unsigned char _error;
};


__END_SYS
#endif
