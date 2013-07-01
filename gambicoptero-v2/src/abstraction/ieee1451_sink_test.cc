#include <system/config.h>

#ifdef __NIC_H

#include <ieee1451_objects.h>
#include <ip.h>
#include <tcp.h>
#include <udp.h>
#include <thread.h>
#include <utility/list.h>
#include <utility/malloc.h>

__USING_SYS

class IEEE1451_Sink
{
private:
    class TCP_Socket;

    IEEE1451_Sink();

public:
    ~IEEE1451_Sink();

    static IEEE1451_Sink *get_instance();

    void execute();
    TCP_Socket *get_socket(const IP::Address &addr);

private:
    class TCP_Socket : public TCP::ServerSocket
    {
    public:
        TCP_Socket(const TCP::Address &local) : TCP::ServerSocket(local), _total_size(0), _attempts(0), _link(this) {
            _send_buffer = new (kmalloc(MAX_BUFFER_SIZE)) char[MAX_BUFFER_SIZE];
        }

        TCP_Socket(const TCP_Socket &socket) : TCP::ServerSocket(socket), _total_size(0), _attempts(0), _link(this) {
            _send_buffer = new (kmalloc(MAX_BUFFER_SIZE)) char[MAX_BUFFER_SIZE];
        }

        ~TCP_Socket() {
            if (_send_buffer)
                kfree(_send_buffer);
        }

        TCP::Socket *incoming(const TCP::Address &from);
        void connected();
        void received(const char *data, u16 size);
        void closing() { close(); }
        void closed();
        void error(short error_code);

        static int error_thread(TCP_Socket *socket, short error_code);

    public:
        char *_send_buffer;
        unsigned short _total_size;
        unsigned short _attempts;
        Simple_List<TCP_Socket>::Element _link;

        Thread *_error_thread;
        char _error_thread_alloc[sizeof(Thread)];
    };

    class UDP_Socket : public UDP::Socket
    {
    public:
        UDP_Socket(const UDP::Address &local, const UDP::Address &remote) : UDP::Socket(local, remote) {}
        void received(const UDP::Address &src, const char *data, unsigned int size);
    };

private:
    Simple_List<TCP_Socket> _tcp_sockets;
    UDP_Socket _udp_socket;
    char *_receive_buffer;
    UART _uart;

    IEEE1451_UART _send_uart;
    IEEE1451_Packet _send_packet;

    static IEEE1451_Sink *_ieee1451;

public:
    void log(const char *str);
};

//-------------------------------------------

IEEE1451_Sink *IEEE1451_Sink::_ieee1451 = 0;

IEEE1451_Sink::IEEE1451_Sink() :
    _udp_socket(UDP::Address(IP::instance()->address(), IEEE1451_PORT),
                UDP::Address(IP::BROADCAST, IEEE1451_PORT)),
    _uart(921600, 8, 0, 1)
{
    _receive_buffer = new (kmalloc(MAX_BUFFER_SIZE)) char[MAX_BUFFER_SIZE];
}

IEEE1451_Sink::~IEEE1451_Sink()
{
    Simple_List<TCP_Socket>::Iterator it = _tcp_sockets.begin();
    while (it != _tcp_sockets.end())
    {
        TCP_Socket *socket = it->object();
        it++;
        _tcp_sockets.remove_head();
        delete socket;
    }

    if (_receive_buffer)
        kfree(_receive_buffer);
}

IEEE1451_Sink *IEEE1451_Sink::get_instance()
{
    if (!_ieee1451)
        _ieee1451 = new IEEE1451_Sink();
    return _ieee1451;
}

IEEE1451_Sink::TCP_Socket *IEEE1451_Sink::get_socket(const IP::Address &addr)
{
    Simple_List<TCP_Socket>::Iterator it = _tcp_sockets.begin();
    while (it != _tcp_sockets.end())
    {
        if (it->object()->remote().ip() == addr)
            return it->object();
        it++;
    }

    return 0;
}

void IEEE1451_Sink::execute()
{
    char *p;
    IEEE1451_UART *uart = (IEEE1451_UART *) _receive_buffer;
    IEEE1451_Packet *packet = (IEEE1451_Packet *) (_receive_buffer + sizeof(IEEE1451_UART));
    const char *msg = _receive_buffer + sizeof(IEEE1451_UART);
    unsigned int size;
    int ret = -1;

    TCP_Socket *socket = new TCP_Socket(TCP::Address(IP::instance()->address(), IEEE1451_PORT));
    _tcp_sockets.insert(&socket->_link);

    while (true)
    {
        p = _receive_buffer;

#ifdef __mc13224v__
        while (!_uart.has_data())
            Thread::yield();
#endif

        for (unsigned short i = 0; i < sizeof(IEEE1451_UART); i++)
            *p++ = _uart.get();

        for (unsigned short i = 0; i < sizeof(IEEE1451_Packet); i++)
            *p++ = _uart.get();

        for (unsigned short i = 0; i < packet->_length; i++)
            *p++ = _uart.get();

        //cout << "IEEE1451_Sink - Received (addr=" << uart->_address << ", type=" << uart->_type
        //     << ", id=" << packet->_trans_id << ", len=" << packet->_length << ")\n";

        size = sizeof(IEEE1451_Packet) + packet->_length;

        if (uart->_type == PACKET_TYPE_MESSAGE)
        {
            TCP_Socket *socket = get_socket(uart->_address);
            if (!socket)
                continue;

            socket->_attempts = 0;
            ret = socket->send(msg, size);

        }else if (uart->_type == PACKET_TYPE_MESSAGE_MULTIMEDIA)
        {
            _udp_socket.remote(UDP::Address(uart->_address, IEEE1451_PORT));
            ret = _udp_socket.send(msg, size);
        }

        if (ret < 0)
            log("Failed sending message");
    }
}

TCP::Socket *IEEE1451_Sink::TCP_Socket::incoming(const TCP::Address &from)
{
    IEEE1451_Sink *sink = IEEE1451_Sink::get_instance();
    TCP_Socket *socket = 0;

    Simple_List<TCP_Socket>::Iterator it = sink->_tcp_sockets.begin();
    while (it != sink->_tcp_sockets.end())
    {
        TCP_Socket *aux = it->object();

        if ((aux->remote().ip() == from.ip()) && (aux->state() != LISTEN))
        {
            sink->log("Using old socket");
            socket = aux;
            socket->clear_timeout();
            socket->rcv_wnd = tcp()->mss();
            socket->state(LISTEN);
            break;
        }
        it++;
    }

    if (!socket)
    {
        socket = new TCP_Socket(*this);
        sink->_tcp_sockets.insert(&socket->_link);
    }
    return socket;
}

void IEEE1451_Sink::TCP_Socket::connected()
{
    //cout << "IEEE1451_Sink - Socket connected (ip=" << remote().ip() << ")\n";
    IEEE1451_Sink *sink = IEEE1451_Sink::get_instance();

    sink->_send_uart._address = remote().ip();
    sink->_send_uart._type = PACKET_TYPE_CONNECT;
    sink->_send_packet._trans_id = 0;
    sink->_send_packet._length = 0;

    for (unsigned short i = 0; i < sizeof(IEEE1451_UART); i++)
        sink->_uart.put(((char  *) &sink->_send_uart)[i]);

    for (unsigned short i = 0; i < sizeof(IEEE1451_Packet); i++)
        sink->_uart.put(((char  *) &sink->_send_packet)[i]);
}

void IEEE1451_Sink::TCP_Socket::received(const char *data, u16 size)
{
    //cout << "IEEE1451_Sink - Socket TCP received (ip=" << remote().ip() << ", size=" << size << ")\n";
    if (size < sizeof(IEEE1451_Packet))
        return;

    IEEE1451_Sink *sink = IEEE1451_Sink::get_instance();
    IEEE1451_Packet *packet = (IEEE1451_Packet *) _send_buffer;

    if (_total_size == 0)
    {
        memcpy(_send_buffer, data, size);
        _total_size = size - sizeof(IEEE1451_Packet);
    }else
    {
        memcpy(_send_buffer + sizeof(IEEE1451_Packet) + _total_size, data, size);
        _total_size += size;
    }

    if (_total_size == packet->_length)
    {
        sink->_send_uart._address = remote().ip();
        sink->_send_uart._type = PACKET_TYPE_MESSAGE;

        for (unsigned short i = 0; i < sizeof(IEEE1451_UART); i++)
            sink->_uart.put(((char  *) &sink->_send_uart)[i]);

        for (unsigned short i = 0; i < sizeof(IEEE1451_Packet) + _total_size; i++)
            sink->_uart.put(_send_buffer[i]);

        _total_size = 0;
    }
}

void IEEE1451_Sink::TCP_Socket::closed()
{
    IEEE1451_Sink *sink = IEEE1451_Sink::get_instance();

    sink->_send_uart._address = remote().ip();
    sink->_send_uart._type = PACKET_TYPE_DISCONNECT;
    sink->_send_packet._trans_id = 0;
    sink->_send_packet._length = 0;

    for (unsigned short i = 0; i < sizeof(IEEE1451_UART); i++)
        sink->_uart.put(((char  *) &sink->_send_uart)[i]);

    for (unsigned short i = 0; i < sizeof(IEEE1451_Packet); i++)
        sink->_uart.put(((char  *) &sink->_send_packet)[i]);
}

void IEEE1451_Sink::TCP_Socket::error(short error_code)
{
    //IEEE1451_Sink *sink = IEEE1451_Sink::get_instance();

    if (error_code == ERR_TIMEOUT)
    {
        if ((state() == LAST_ACK) || (state() == ESTABLISHED))
        {
            if (_error_thread)
                _error_thread->Thread::~Thread();
            _error_thread = new (&_error_thread_alloc) Thread(error_thread, (TCP_Socket *) this, error_code);
        }
    }
}

int IEEE1451_Sink::TCP_Socket::error_thread(TCP_Socket *socket, short error_code)
{
    IEEE1451_Sink *sink = IEEE1451_Sink::get_instance();

    if (socket->state() == LAST_ACK)
    {
        sink->log("Socket error_thread (LAST_ACK)");

        if (socket->_attempts >= 3)
        {
            sink->log("Retry limit reached");
            socket->state(CLOSED);
            socket->closed();
        }

        socket->_attempts++;
        socket->close();

    }else if (socket->state() == ESTABLISHED)
    {
        sink->log("Socket error_thread (ESTABLISHED)");

        IEEE1451_UART *uart = (IEEE1451_UART *) sink->_receive_buffer;
        IEEE1451_Packet *packet = (IEEE1451_Packet *) (sink->_receive_buffer + sizeof(IEEE1451_UART));

        if (uart->_address == socket->remote().ip())
        {
            if (socket->_attempts >= 10)
            {
                sink->log("Retry limit reached");
                return 0;
            }

            //sink->log("Resending due to TIMEOUT error");

            unsigned int size = sizeof(IEEE1451_Packet) + packet->_length;
            const char *msg = sink->_receive_buffer + sizeof(IEEE1451_UART);
            socket->_attempts++;

            int ret = socket->send(msg, size);
            if (ret < 0)
                sink->log("Failed resending message");
        }
    }

    return 0;
}

void IEEE1451_Sink::UDP_Socket::received(const UDP::Address &src, const char *data, unsigned int size)
{
    //cout << "IEEE1451_Sink - Socket UDP received (ip=" << src.ip() << ", size=" << size << ")\n";
    if (size < sizeof(IEEE1451_Packet))
        return;

    IEEE1451_Sink *sink = IEEE1451_Sink::get_instance();

    sink->_send_uart._address = src.ip();
    sink->_send_uart._type = PACKET_TYPE_MESSAGE_MULTIMEDIA;

    for (unsigned short i = 0; i < sizeof(IEEE1451_UART); i++)
        sink->_uart.put(((char  *) &sink->_send_uart)[i]);

    for (unsigned short i = 0; i < size; i++)
        sink->_uart.put(data[i]);
}

void IEEE1451_Sink::log(const char *str)
{
    _send_uart._address = IP_Address((unsigned long) 0);
    _send_uart._type = PACKET_TYPE_LOG;
    _send_packet._trans_id = 0;
    _send_packet._length = strlen(str);

    for (unsigned short i = 0; i < sizeof(IEEE1451_UART); i++)
        _uart.put(((char  *) &_send_uart)[i]);

    for (unsigned short i = 0; i < sizeof(IEEE1451_Packet); i++)
        _uart.put(((char  *) &_send_packet)[i]);

    for (unsigned short i = 0; i < _send_packet._length; i++)
        _uart.put(str[i]);
}

//-------------------------------------------

int main()
{
    unsigned int *GPIO_BASE = (unsigned int *) 0x80000000;
    *GPIO_BASE = 0;

    Alarm::delay(TIME_500_MS * 2);
    //cout << "+++++ Starting sink +++++\n";

    IP *ip = IP::instance();
    ip->set_address(IP::Address(10, 0, 0, 110));
    ip->set_gateway(IP::Address(10, 0, 0, 1));
    ip->set_netmask(IP::Address(255, 255, 255, 0));

    IEEE1451_Sink *app = IEEE1451_Sink::get_instance();
    app->execute();
    return 0;
}

#else

int main() { return 0; }

#endif

