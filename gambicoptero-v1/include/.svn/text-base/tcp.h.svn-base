// EPOS Transmission Control Protocol implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __tcp_h
#define __tcp_h

#include <alarm.h>
#include <condition.h>
#include <ip.h>
#include <icmp.h>
#include <udp.h> // TCP::Address == UDP_Address
#include <utility/handler.h>
#include <utility/random.h>

__BEGIN_SYS

// Optmization for single NIC support
class TCP_SingleNIC {
public:
    IP* ip() const { return IP::instance(); }
protected:
    TCP_SingleNIC(IP * ip) {}
};

// Generalization for multiple NICs support
class TCP_MultiNIC {
public:
    IP* ip() const { return _ip; }
    
protected:
    TCP_MultiNIC(IP * ip) : _ip(ip) {}
      
private:    
    IP * _ip;
};

class TCP: public IF<IP::multiNIC, TCP_MultiNIC, TCP_SingleNIC>::Result,
           public IP::Observer,
           public Data_Observed < UDP_Address >
{
public:
    typedef IF<IP::multiNIC, TCP_MultiNIC, TCP_SingleNIC>::Result Base;
    typedef UDP_Address Address;

    static const unsigned int ID_TCP = 6;

    class Header;
    
    class Socket_SingleNIC;
    class Socket_MultiNIC;
    
    class Socket;
    class ServerSocket;
    class ClientSocket;

    class Channel;
    
    TCP(IP * ip = 0);
    ~TCP();
          
    void update(Data_Observed<IP::Address>*, long, IP::Address,
                IP::Address, void*, unsigned int);
    
    static TCP * instance(unsigned int i=0);

    inline u16 mss();
};

class TCP::Header {
    friend class TCP;
    
 public:
    Header(u32 s = 0, u32 a = 0);

    u16 _src_port, _dst_port;
    u32 _seq_num, _ack_num;
#if !defined(i386) && !defined(AVR) && !defined(ARM)
    u8  _hdr_off:4, _un1:4;
    bool  _un3:1, _un2:1;
    bool  _urg:1, _ack:1, _psh:1, _rst:1, _syn:1, _fin:1;
#else
    u8  _un1:4, _hdr_off:4;
    bool  _fin:1, _syn:1, _rst:1, _psh:1, _ack:1, _urg:1;
    bool  _un2:1, _un3:1;
#endif
    u16 _wnd;
    volatile u16 _chksum;
    u16 _urgptr;

    // getters
    u16 size() const { return _hdr_off * 4; }
    u16 dst_port() const { return CPU::ntohs(_dst_port); }
    u16 src_port() const { return CPU::ntohs(_src_port); }
    u32 seq_num() const { return CPU::ntohl(_seq_num); }
    u32 ack_num() const { return CPU::ntohl(_ack_num); }
    u16 wnd() const { return CPU::ntohs(_wnd); }
    u16 chksum() const { return CPU::ntohs(_chksum); }
    u16 urgptr() const { return CPU::ntohs(_urgptr); }
    
    // setters
    void seq_num(u32 v) { _seq_num = CPU::htonl(v); }
    void ack_num(u32 v) { _ack_num = CPU::htonl(v); }
    void wnd(u16 v) { _wnd = CPU::htons(v); }
    void chksum(u16 v) { _chksum = CPU::htons(v); }
    void urgptr(u16 v) { _urgptr = CPU::htons(v); }
    void dst_port(u16 v) { _dst_port = CPU::htons(v); }
    void src_port(u16 v) { _src_port = CPU::htons(v); }
    
    u16 _checksum(IP::Address src,IP::Address dst,u16 len);
    void _checksum(IP::Address src,IP::Address dst,SegmentedBuffer * sb);
    void checksum(IP::Address src,IP::Address dst,u16 len);
    bool validate_checksum(IP::Address src,IP::Address dst,u16 len);

    // Ultimate unreadable coding style!
    friend Debug& operator<< (Debug & db, const Header& s)
    {
    db << "Header[SRC="<<s.src_port()<<",DST="<<s.dst_port()<<"] SEQ="<<
    s.seq_num()<<",ACK="<<s.ack_num()<<",off="<<s._hdr_off<<
    " CTL=["<<(s._urg ? "U" : "") <<(s._ack ? "A" : "") <<
    (s._psh ? "P" : "") <<(s._rst ? "R" : "") <<(s._syn ? "S" : "") <<
    (s._fin ? "F" : "") <<"],wnd="<<s.wnd()<<",chk="<<s.chksum()<<"]\n";
    return db;
    }

    // Pseudo header for checksum calculations
    struct Pseudo_Header {
        u32 src_ip, dst_ip;
        u8 zero, protocol;
        u16 length;
        
        Pseudo_Header(u32 src,u32 dst,u16 len) 
        : src_ip(src), dst_ip(dst), zero(0), protocol(ID_TCP),
          length(CPU::htons(len)) {};
    };
// Compact bitfields istead of using 1 char for each single bit attribute
} __attribute__((packed)); 

class TCP::Socket_SingleNIC {
public:
    TCP * tcp() const { return TCP::instance(); }
protected:
    Socket_SingleNIC(TCP * tcp) {}
};

class TCP::Socket_MultiNIC {
public:
    TCP * tcp() const { return _tcp; }
protected:
    Socket_MultiNIC(TCP * tcp) : _tcp(tcp) {
        if (!tcp) _tcp = TCP::instance();
    }
private:
    TCP * _tcp;
};

/**
 * The TCP::Socket is a base class for the event driven communication API.
 * For correct usage the application should use the ClientSocket
 * or ServerSocket instead of this class.
 */
class TCP::Socket : public IF<IP::multiNIC,
                              Socket_MultiNIC, Socket_SingleNIC>::Result,
                    public Data_Observer<TCP::Address>,
                    public Handler
{
public:
    typedef IF<IP::multiNIC,
               Socket_MultiNIC, Socket_SingleNIC>::Result Base;
    typedef void (Socket::* Handler)(const Header&,const char*,u16);
    
    enum { // Erros
        ERR_NOT_CONNECTED = 1,
        ERR_TIMEOUT,
        ERR_RESET,
        ERR_CLOSING,
        ERR_NO_ROUTE,
        ERR_NOT_STARTED,
        ERR_REFUSED,
        ERR_ILEGAL
    };

    Socket(const Socket& socket);    
    Socket(const Address &local,const Address &remote,TCP * tcp);
    virtual ~Socket();
    
    // Data_Observer callback
    void update(Data_Observed<TCP::Address> *ob, long c, TCP::Address src,
            TCP::Address dst, void *data, unsigned int size);
            
    //* Called when connection handshake is complete
    virtual void connected() {};
    
    //* Called when data arrives
    virtual void received(const char* data,u16 size) {};
    
    //* Called when the peer signal a push flag
    virtual void push() {};
    
    //* Called when data was sucessfuly sent
    virtual void sent(u16 size) {};
    
    //* Called when there is an error
    virtual void error(short errorcode) {};
    
    //* Called when the connection is closed
    // Default action deletes the Socket object
    virtual void closed() { delete this; };
    
    //* Called when the peer closed his side of the connection
    virtual void closing() {} 
    
    //* Called to notify an incoming connection
    //* Should return a copy of (or) itself to accept the connection
    virtual Socket* incoming(const Address& from) { return this; }

    void abort();
    void close();
    void connect();
    void listen();
    void send(const char* data,u16 len,bool push = true);
    
    const Address & remote() { return _remote; }
    const Address & local()  { return _local; }
    
    enum { // Connection state
        LISTEN,     SYN_SENT,
        SYN_RCVD,   ESTABLISHED,
        FIN_WAIT1,  FIN_WAIT2,
        CLOSE_WAIT, CLOSING,
        LAST_ACK,   TIME_WAIT,
        CLOSED,
    };
    
    u8 state() { return _state; }
    void state(u8 s) {
        _state = s;
        state_handler = handlers[s];   
    }

    // state-processing functions
    void __LISTEN(const Header&,const char*,u16);
    void __SYN_SENT(const Header&,const char*,u16);
    void __SYN_RCVD(const Header&,const char*,u16);
    void __ESTABLISHED(const Header&,const char*,u16);
    void __FIN_WAIT1(const Header&,const char*,u16);
    void __FIN_WAIT2(const Header&,const char*,u16);
    void __CLOSE_WAIT(const Header&,const char*,u16);
    void __CLOSING(const Header&,const char*,u16);
    void __LAST_ACK(const Header&,const char*,u16);
    void __TIME_WAIT(const Header&,const char*,u16);
    void __CLOSED(const Header&,const char*,u16);
    void __SNDING(const Header&,const char*,u16);
    void __RCVING(const Header&,const char*,u16);
    
    Handler state_handler;

    void operator()();
 protected: 
    // methods
    bool check_seq(const Header &h,u16 len);
    void send_ack();
    void send_fin();
    void send_reset();
    s32 _send(Header * hdr, SegmentedBuffer * sb);
    void set_timeout();
    void clear_timeout();

    // attributes
    TCP::Address _remote;
    TCP::Address _local;
    volatile u8 _state;
    volatile u32 _rtt;
    
    volatile u32 snd_una, snd_nxt, snd_ini, snd_wnd;
    volatile u32 rcv_nxt, rcv_ini, rcv_wnd;

    Alarm * _timeout;

    // class attributes
    static Handler handlers[13];
};

u16 TCP::mss() {
   return ip()->nic()->mtu() - sizeof(TCP::Header) - sizeof(IP::Header);
}

/**
 * The ClientSocket represents an active communicator using the
 * TCP/IP subsystem.
 * 
 * Applications using this class must implement all pure virtual methods
 * from TCP::Socket.
 */
class TCP::ClientSocket : public TCP::Socket {
public:

    /**
     * Creates an active socket bound to address _local_ and remote peer
     * address _remote_. With _start_ = true a connection attempt will
     * be made as soon as the object is created.
     */
    ClientSocket(const Address &remote,const Address &local,
                 bool start = true,TCP * tcp = 0);
    virtual ~ClientSocket() {}
   
    void connect() { Socket::connect(); }
    void connect(const Address& to) {
        _local = to;
        Socket::connect();
    }
};

/**
 * The ServerSocket represents a passive communicator using the
 * TCP/IP subsystem.
 * 
 * Applications using this class (by inheritance) must implement 
 * all pure virtual methods from TCP::Socket.
 */
class TCP::ServerSocket : public TCP::Socket {
public:

    /**
     * Creates a passive socket that listens on the IP:PORT specified by _local_
     * If _start_ is true, the socket will start listening right after the
     * object creation, otherwise the implementor should call listen().
     */
    ServerSocket(const Address &local,bool start = true,TCP * tcp = 0);
    ServerSocket(const TCP::ServerSocket &socket);
    virtual ~ServerSocket() {}
};


/**
 * TCP::Channel is a blocking (synchronous) API for communication using
 * the TCP/IP subsystem. It is built upon the event-driven TCP::Socket
 * to offer a simple send/receive, connect/listen, stream-based, comm.
 * framework.
 * 
 * While the TCP::Socket use is done by inheritance, TCP::Channel can be
 * used by composition/agregation.
 */
class TCP::Channel : public ICMP::Observer, public TCP::Socket {
public:
    typedef Alarm::Microsecond Microsecond;
      
    Channel();
    virtual ~Channel();
    
    /**
     * Waits for incoming data.
     * @return number of bytes received
     */
    int receive(char * dst,unsigned int size);
    
    /**
     * Sends _size_ bytes from _src_ to the remote peer.
     * Returns upon acknownledge or timeout.
     * @return number of bytes sent. 
     */
    int send(const char * src,unsigned int size);

    /**
     * Connect to remote host.
     * This method blocks until the connection is established.
     */
    bool connect(const TCP::Address& to);
    
    /**
     * Associate this channel with a local port.
     */
    void bind(unsigned short port);
    
    /**
     * Closes the connection.
     * Attention: The meaning of the "CLOSE" function in the
     * BSD Sockets API is: "I will not send more data, but can receive"
     * This method DOES NOT HAVE the same meaning. It will block until
     * the connection is fully closed or aborted.
     */
    bool close();
    
    /**
     * Wait for a remote peer to connect.
     * @return true if connected, false otherwise
     */
    bool listen();
    
    /**
     * Get error condition. Zero means no error.
     */
    short error() { return _error; }
    
protected:
    // clear internal attributes
    void clear();
    
    // from TCP::Socket
    
    void received(const char* data,u16 size);
    void closing();
    void closed();
    void connected();
    void sent(u16 size);
    void error(short errorcode);
    void push();
    
    // ICMP callback
    void update(Data_Observed<IP::Address> *ob, long c,
                IP::Address src, IP::Address dst,
                void *data, unsigned int size);
    
    // Attributes
       
    bool _sending;
    bool _receiving;
    
    Condition _rx_block;
    Condition _tx_block;
    
    char * _rx_buffer_ptr;
    unsigned int _rx_buffer_size;
    volatile unsigned int _rx_buffer_used;
    
    volatile unsigned int _tx_bytes_sent;
    
    volatile short _error;
};

__END_SYS
#endif

