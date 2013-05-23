#include <tcp.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

__BEGIN_SYS

// static data
TCP::Socket::Handler TCP::Socket::handlers[13] = { 
    &TCP::Socket::__LISTEN,     &TCP::Socket::__SYN_SENT,
    &TCP::Socket::__SYN_RCVD,   &TCP::Socket::__ESTABLISHED,
    &TCP::Socket::__FIN_WAIT1,  &TCP::Socket::__FIN_WAIT2,
    &TCP::Socket::__CLOSE_WAIT, &TCP::Socket::__CLOSING,
    &TCP::Socket::__LAST_ACK,   &TCP::Socket::__TIME_WAIT,
    &TCP::Socket::__CLOSED };

TCP::TCP(IP * _ip) : Base(_ip)
{
    ip()->attach(this, ID_TCP);
}

TCP::~TCP()
{
    ip()->detach(this, ID_TCP);
}

TCP * TCP::instance(unsigned int i) {
    static TCP * _instance[Traits<NIC>::NICS::Length];
    if (!_instance[i])
        _instance[i] = new TCP(IP::instance(i));
    return _instance[i];
}

// Called by IP's notify(...)

void TCP::update(Data_Observed<IP::Address> *ob, long c, IP::Address src,
                 IP::Address dst, void *data, unsigned int size)
{
    Header& hdr = *reinterpret_cast<Header*>(data);

    db<TCP>(TRC) << "TCP::update: "<< hdr << endl;

    if (!(hdr.validate_checksum(src,dst,size - hdr.size()))) {
        db<TCP>(INF) << "TCP checksum failed for incoming packet!\n";
        return;
    }

    int len = size - hdr.size();
    if (len < 0) {
        db<TCP>(INF) << "Misformed TCP segment received\n";
        return;
    }

    notify(TCP::Address(src,hdr.src_port()),
           TCP::Address(dst,hdr.dst_port()),
           (int) hdr.dst_port(), data, size);
}


// Called by TCP's notify(...)

void TCP::Socket::update(Data_Observed<TCP::Address> *o, long c, TCP::Address src,
                         TCP::Address dst, void *data, unsigned int size)
{
    Header& hdr = *reinterpret_cast<Header*>(data);
    int len = size - hdr.size();
  
    if ((_remote == src) || (_remote.port() == 0))
    {
        if (state() == LISTEN) _remote = src;
        (this->*state_handler)(hdr,&((char*)data)[hdr.size()],len);
    } else {
        db<TCP>(TRC) << "TCP Segment does not belong to us\n";          
    }
}

// Header stuff

TCP::Header::Header(u32 seq,u32 ack)
{
    memset(this,0,sizeof(Header));
    seq_num(seq);
    ack_num(ack);
}

bool TCP::Header::validate_checksum(IP::Address src,IP::Address dst,u16 len)
{
    len += size();
    
    Pseudo_Header phdr((u32)src,(u32)dst,len);

    u32 sum = 0;

    sum = IP::calculate_checksum(this, len);
    sum += IP::calculate_checksum(&phdr, sizeof(phdr));
    
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return sum == 0xFFFF;
}

void TCP::Header::_checksum(IP::Address src,IP::Address dst,SegmentedBuffer * sb)
{
    u16 len;
    len = size();

    if (sb) len += sb->total_size();

    Pseudo_Header phdr((u32)src,(u32)dst,len);

    _chksum = 0;

    u32 sum = 0;

    sum = IP::calculate_checksum(&phdr, sizeof(phdr));
    sum += IP::calculate_checksum(this, size());

    while (sb) {
        sum += IP::calculate_checksum(sb->data(), sb->size());
        sb = sb->next();
    }
    
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    _chksum = ~sum;
}

// Socket stuff

TCP::Socket::Socket(const Address &remote,const Address &local,TCP * _tcp)
    : Base(_tcp), _remote(remote), _local(local), _rtt(500000), _timeout(0)
{
    rcv_wnd = tcp()->mss();
    state(CLOSED);
    tcp()->attach(this, _local.port());
}

TCP::Socket::Socket(const TCP::Socket& socket) : Base(socket.tcp())
{
    memcpy(this, &socket, sizeof(Socket));
    state(CLOSED);
    tcp()->attach(this, _local.port());
}

TCP::Socket::~Socket()
{
    tcp()->detach(this, _local.port());
    clear_timeout();
}

s32 TCP::Socket::_send(Header * hdr, SegmentedBuffer * sb)
{
    // fill header
    hdr->src_port(_local.port());
    hdr->dst_port(_remote.port());
    hdr->_hdr_off = 5; // our header is always 20 bytes
    hdr->wnd(rcv_wnd);
    hdr->chksum(0);
    hdr->_checksum(_local.ip(),_remote.ip(),sb);

    // hdr + sb
    SegmentedBuffer nsb(hdr,hdr->size());
    nsb.append(sb);


    return tcp()->ip()->send(_local.ip(),_remote.ip(),&nsb,TCP::ID_TCP) - hdr->size();
}

/**
 * The basic Socket::send() splits data into multiple segments
 * respecting snd_wnd and mtu.
 */
void TCP::Socket::send(const char *data,u16 len,bool push)
{
    if (snd_wnd == 0) { // peer cannot receive data
        set_timeout();
        send_ack(); // zero-window probing
        return;
    }
    // cannot send more than the peer is willing to receive
    if (len > snd_wnd)
        len = snd_wnd;

    int more = 0,mss = tcp()->mss();
    if (len > mss) { // break up into multiple segments
        more = len - mss;
        len = mss;
    }
    Header hdr(snd_nxt,rcv_nxt);
    hdr._ack = true;  // with pidgeback ack
    hdr._psh = (push && (more == 0)); // set push flag only on the last segment
    snd_nxt += len;
    SegmentedBuffer sb(data,len);
    _send(&hdr,&sb);
    
    if (more > 0) // send next segment too
        send(&data[len], more);

    set_timeout();
}

void TCP::Socket::set_timeout() {
    if (_timeout) delete _timeout;
    _timeout = new Alarm(2 * _rtt, this, 1);
}

void TCP::Socket::clear_timeout() {
    if (_timeout)
    {
        delete _timeout;
        _timeout = 0;
    }
}

void TCP::Socket::operator()() {
    delete _timeout;
    _timeout = 0;

    snd_nxt = snd_una; // rollback, so the user can resend
    error(ERR_TIMEOUT);
}

void TCP::Socket::close()
{
    send_fin();
    set_timeout();
    if (state() == ESTABLISHED)
        state(FIN_WAIT1);
    else if (state() == CLOSE_WAIT) {
        state(LAST_ACK);    
    }
    else if (state() == SYN_SENT) {
        state(CLOSED);
        clear_timeout();
        closed();    
    }
}


TCP::ClientSocket::ClientSocket(const Address& remote,const Address& local,
                                bool start, TCP * tcp) 
	: Socket(remote,local,tcp)
{
    if (start)
        Socket::connect();
}

void TCP::Socket::connect() {
    if (state() != CLOSED && state() != SYN_SENT) {
        db<TCP>(ERR) << "TCP::Socket::connect() could not be called\n";
        return;
    }
    
    state(SYN_SENT);
    snd_ini = Pseudo_Random::random() & 0x00FFFFFF;
    snd_una = snd_ini;
    snd_nxt = snd_ini + 1;

    Header hdr(snd_ini, 0);
    hdr._syn = true;
    _send(&hdr,0);
    set_timeout();
}

TCP::ServerSocket::ServerSocket(const Address& local,bool start,TCP * tcp) 
    : Socket(Address(0,0),local,tcp)
{
    if (start)
        Socket::listen();
}

TCP::ServerSocket::ServerSocket(const TCP::ServerSocket &socket)
    : Socket(socket)
{
}

void TCP::Socket::listen()
{
    if (state() != CLOSED || state() != LISTEN) {
        db<TCP>(ERR) << "TCP::Socket::listen() called with state: " << state() << endl;
        return;
    }
    
    _remote = Address(0,0);
    state(LISTEN);
}

void TCP::Socket::__LISTEN(const Header& r ,const char* data,u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;

    if (r._syn && !r._rst && !r._fin) {
        Socket * n;
          
        if ((n = incoming(_remote)) != 0) {
            n->_remote = _remote;
            n->rcv_nxt = r.seq_num()+1;
            n->rcv_ini = r.seq_num();
            n->snd_wnd = r.wnd();

            n->state(SYN_RCVD);

            n->snd_ini = Pseudo_Random::random() & 0x0000FFFF;

            Header s(n->snd_ini,n->rcv_nxt);
            s._syn = true;
            s._ack = true;
            n->_send(&s,0);
        
            n->snd_nxt = n->snd_ini+1;
            n->snd_una = n->snd_ini;

            n->set_timeout();
        }
        // else = connection rejected
    } 
    if (state() == LISTEN) {
        // a new socket was created to handle the incomming connection
        // and we stay in the listening state
        _remote = Address((u32)0,(u16)0);
    }
}


void TCP::Socket::__SYN_SENT(const Header& r,const char* data,u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;

    if (r._rst || r._fin) {
            error(ERR_REFUSED);
            state(CLOSED);
            clear_timeout();
            closed();
    }
    else if (r._ack) {
        if ((r.ack_num() <= snd_ini) || (r.ack_num() > snd_nxt)) {
            error(ERR_RESET);
            state(CLOSED);
            clear_timeout();
            closed();
        } else if ((r.ack_num() >= snd_una) && (r.ack_num() <= snd_nxt)) {
            if (r._syn) {
                rcv_nxt = r.seq_num() + 1;
                rcv_ini = r.seq_num();
                snd_una = r.ack_num();
                snd_wnd = r.wnd();
                if (snd_una <= snd_ini) {
                    state(SYN_RCVD);
                } else {
                    state(ESTABLISHED);
                    clear_timeout();
                    send_ack();
                    connected();
                }
            } else {
                // TODO: discover what to do here
            }
        }
    } else if (!r._rst && r._syn) {
        rcv_nxt = r.seq_num() + 1;
        snd_ini = r.seq_num();
        snd_wnd = r.wnd();
        state(SYN_RCVD);
    }

}

void TCP::Socket::__SYN_RCVD(const Header& r ,const char* data,u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;

    if (!check_seq(r,len)) 
        return;

    if (r._rst || r._fin) {
        error(ERR_RESET);
        state(CLOSED);
        clear_timeout();
        closed();
    }
    else if (r._ack) {
        snd_wnd = r.wnd();
        snd_una = r.ack_num();
        state(ESTABLISHED);
        clear_timeout();
        connected();
    }
}

void TCP::Socket::__RCVING(const Header &r,const char* data,u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;
    if (len) {
        rcv_nxt += len;
        send_ack();
        received(data,len);
        if (r._psh)
            push();
    } else {
        send_ack();
    }
}

void TCP::Socket::__SNDING(const Header &r,const char* data, u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;

    if (r._ack) {
        int bytes = r.ack_num() - snd_una;
        if (bytes < 0) // sliding window overflow
            bytes = r.ack_num() + (0xFFFF - snd_una);
        sent(bytes);
        snd_una = r.ack_num();
        if (snd_una == snd_nxt)
            clear_timeout();
    }
}

void TCP::Socket::__ESTABLISHED(const Header& r ,const char* data,u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;

    if (!check_seq(r,len))
    {
        if ((len) && (r.seq_num() < rcv_nxt))
            send_ack();
        return;
    }

    if (r._rst) {
        error(ERR_RESET);
        state(CLOSED);
        clear_timeout();
        closed();
    }
    else if (r.seq_num() == rcv_nxt) { // implicit reject out-of-order segments
        snd_wnd = r.wnd();

        if (snd_una < r.ack_num())
            __SNDING(r,data,len);

        if (len)
            __RCVING(r,data,len);

        if (r._fin) {
            send_ack();
            state(CLOSE_WAIT);
            closing();
        }
    }
    else {
        db<TCP>(TRC) << "TCP::out of order segment received\n";
    }
}


void TCP::Socket::__FIN_WAIT1(const Header& r ,const char* data,u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;

    if (!check_seq(r,len))
        return;
   
    if (!r._fin && len) {
        __RCVING(r,data,len);
        if (r._ack)
            state(FIN_WAIT2);
        return;
    }
    if (r._ack && !r._fin) { // TODO: check snd_una
        rcv_nxt = r.seq_num() + len;
        state(FIN_WAIT2);
        send_ack();
    }
    if (r._ack && r._fin) {
        state(CLOSED); // no TIME_WAIT
        send_ack();
        clear_timeout();
        closed();
    }
    if (!r._ack && r._fin) {
        state(CLOSING);
        send_ack();
    }
}

void TCP::Socket::__FIN_WAIT2(const Header& r ,const char* data,u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;
    if (!check_seq(r,len))
        return;
    if (len) {
        __RCVING(r,data,len);  
    }

    if (r._fin) {
        state(CLOSED); // no TIME_WAIT
        send_ack();
        clear_timeout();
        closed();
    }
}

void TCP::Socket::__CLOSE_WAIT(const Header& r ,const char* data,u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;
    if (!check_seq(r,len))
        return;

    if (r._rst || len) {
        if (len)
            send_reset();
        error(ERR_RESET);
        state(CLOSED);
        clear_timeout();
        closed();
    } 
}

void TCP::Socket::__CLOSING(const Header& r ,const char* data,u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;
    if (!check_seq(r,len))
        return;
    
    if (r._ack) {
        state(CLOSED); // no TIME_WAIT
        clear_timeout();
        closed();
    }
}

void TCP::Socket::__LAST_ACK(const Header& r ,const char* data,u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;
    if (!check_seq(r,len))
        return;
    if (r._ack) {
        state(CLOSED);
        closed();
    }
}

void TCP::Socket::__TIME_WAIT(const Header& r ,const char* data,u16 len)
{
    db<TCP>(TRC) << __PRETTY_FUNCTION__ << endl;
    if (!check_seq(r,len))
        return;
    
    if (r._fin && r._ack) {
        state(CLOSED);
        clear_timeout();
        closed();
    }
}

void TCP::Socket::__CLOSED(const Header&,const char*,u16)
{
    // does nothing
}

void TCP::Socket::send_ack()
{
    Header s(snd_nxt,rcv_nxt);
    s._ack = true;
    _send(&s,0);
}

void TCP::Socket::send_fin()
{
    Header s(snd_nxt,rcv_nxt);
    s._fin = true;
    s._ack = true;
    _send(&s,0);
}

void TCP::Socket::send_reset()
{
    Header s(snd_nxt,rcv_nxt);
    s._fin = true;
    s._ack = true;
    s._psh = true;
    s._rst = true;
    _send(&s,0);
}

bool TCP::Socket::check_seq(const Header &h,u16 len)
{
    if ((len <= rcv_wnd) &&
        (h.seq_num() == rcv_nxt))
    {
        return true;
    }

    db<TCP>(TRC) << "TCP: check_seq() == false\n";
    return false;
}

void TCP::Socket::abort()
{
    send_reset();
    clear_timeout();
    state(CLOSED);
}

// Channel stuff

TCP::Channel::Channel() 
    : TCP::Socket(TCP::Address(0,0),TCP::Address(0,0),0)
{
    clear();
    ICMP::instance()->attach(this, ICMP::UNREACHABLE);
}

TCP::Channel::~Channel() {
    if (state() != CLOSED) {
        db<TCP>(ERR) << "Destroying non-closed channel!\n";
        // This condition must REALLY not happen.
    }
    ICMP::instance()->detach(this, ICMP::UNREACHABLE);
}

bool TCP::Channel::connect(const TCP::Address& to)
{
    if (state() != CLOSED) {
        db<TCP>(ERR) << "TCP::Channel::connect() called for open connection!\n";
        return false;
    }
    
    int retry = 5;
    _remote = to;
    clear();
    _sending = true;
    do {
        Socket::connect();
        _rx_block.wait();
    } while (retry-- > 0 && state() != ESTABLISHED);
    _sending = false;
    
    return state() == ESTABLISHED;
}

int TCP::Channel::receive(char * dst,unsigned int size)
{
    if (_error)
        return -_error;
    
    if (state() != ESTABLISHED)
        return -ERR_NOT_CONNECTED;
    
    if (_receiving) {
        db<TCP>(ERR) << "TCP::Channel::receive already called!\n";
        return -ERR_ILEGAL;
    }
    
    _rx_buffer_ptr  = dst;
    _rx_buffer_size = size;
    _rx_buffer_used = 0;
    _receiving = true;
    rcv_wnd = size;
    send_ack(); // send a window update
    
    _rx_block.wait();
    
    int rcvd = _rx_buffer_used;

    _rx_buffer_ptr  = 0;
    _rx_buffer_size = 0;
    _rx_buffer_used = 0;
    _receiving = false;
    
    return rcvd;
}

int TCP::Channel::send(const char * src,unsigned int size)
{
    if (_error)
        return -_error;
    
    if (state() != ESTABLISHED)
        return -ERR_NOT_CONNECTED;
    
    // congestion control not yet done
    
    _tx_bytes_sent = 0;
    _sending = true;
    
    do {
        int offset = _tx_bytes_sent;
        Socket::send(&src[offset], size - offset);
        
        _tx_block.wait();
        
        if (state() != ESTABLISHED || _error)
            break;
        
    } while (_tx_bytes_sent < size);
    
    _sending = false;
    return _tx_bytes_sent;
}

void TCP::Channel::bind(unsigned short port)
{
    if (state() != CLOSED) {
        db<TCP>(ERR) << "Cannot use TCP::Channel::bind() on open connection\n";
        return;
    }
    
    tcp()->detach(this, _local.port());
    
    _local = TCP::Address(tcp()->ip()->address(), port);
    
    tcp()->attach(this, port);
}

bool TCP::Channel::close()
{
    if (state() == CLOSED)
        return true;
    
    if (state() == SYN_SENT) {
        _rx_block.signal();
        abort();
        return true;
    }
    
    if (_receiving) {
        _error = ERR_CLOSING;
        _rx_block.signal();
    }
    
    int retry = 5;
    _sending = true;
    
    do {
        Socket::close();
        _tx_block.wait();
    } while (retry-- > 0 && state() != CLOSED || _error);
    
    _sending = false;
    
    return state() == CLOSED;
}

bool TCP::Channel::listen()
{
    if (state() != CLOSED) {
        db<TCP>(ERR) << "TCP::Channel::listen() called on non-closed channel\n";
        return false;
    }
    
    clear();
    Socket::listen();   
    _rx_block.wait();
    
    return state() == ESTABLISHED;
}

void TCP::Channel::clear()
{
    _sending   = false;
    _receiving = false;;
    _rx_buffer_ptr  = 0;
    _rx_buffer_size = 0;
    _rx_buffer_used = 0;
    _tx_bytes_sent = 0;
    _error = 0;
    rcv_wnd = 0;
}

// Channel's implementation of Socket callbacks

void TCP::Channel::received(const char* data,u16 size)
{
    int remaining = _rx_buffer_size - _rx_buffer_used;
    
    if (!_rx_buffer_ptr || (remaining == 0)) {
        db<TCP>(WRN) << "Channel::received droping data, no buffer space\n";
        return;
    }
    if (remaining < static_cast<int>(size)) {
        db<TCP>(WRN) << "Channel::received data truncated\n";
        size = static_cast<u16>(remaining);
    }
    
    memcpy(&_rx_buffer_ptr[_rx_buffer_used], data, size);
    
    _rx_buffer_used += size;
    rcv_wnd = _rx_buffer_size - _rx_buffer_used;
    
    if (_rx_buffer_size == _rx_buffer_used)
        if (_receiving)
            _rx_block.signal();
}

void TCP::Channel::push()
{
    if (_receiving)
        _rx_block.signal();
}

void TCP::Channel::closing()
{
    if (_receiving)
        _rx_block.signal();
}

void TCP::Channel::closed()
{
    if (_receiving)
        _rx_block.signal();
    
    if (_sending)
        _tx_block.signal();
}

void TCP::Channel::connected()
{
    _rx_block.signal();
}

void TCP::Channel::sent(u16 size)
{
    if (_sending) {
        _tx_bytes_sent += size;
        
        _tx_block.signal();
    }
}

void TCP::Channel::error(short errorcode)
{
    if (errorcode != ERR_TIMEOUT) {
        _error = errorcode; 
    }
    else if (_receiving) {
        _rx_block.signal();  
    }
    
    if (_sending)
        _tx_block.signal();  
}

void TCP::Channel::update(Data_Observed<IP::Address> *ob, long c,
                IP::Address src, IP::Address dst,
                void *data, unsigned int size)
{
        // TODO
}
__END_SYS

