#include <sip_manager.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <sip_user_agent.h>
#include <utility/random.h>

__BEGIN_SYS

USER_CALLBACK SIP_Manager::_callback = 0;
SIP_Manager *SIP_Manager::_instance = 0;

SIP_Manager::~SIP_Manager()
{
    if (_host_ip)
        delete _host_ip;

    Simple_List<SIP_User_Agent>::Iterator it = _ua.begin();
    while (it != _ua.end())
    {
        SIP_User_Agent *ua = it->object();
        _ua.remove(it++);
        delete ua;
    }
}

SIP_Manager *SIP_Manager::get_instance()
{
    if (!_instance)
        _instance = new SIP_Manager();

    return _instance;
}

void SIP_Manager::delete_instance()
{
    if (_instance)
    {
        delete _instance;
        _instance = 0;
    }
}

bool SIP_Manager::init()
{
    _host_port = SIP_PORT;

    char local[20];
    ((IP::Address &) IP::instance(0)->address()).to_string(local);
    _host_ip = create_string(local);
    if (!_host_ip)
    {
        db<SIP_Manager>(WRN) << "SIP_Manager::init -> Failed to get the host ip\n";
        return false;
    }

    return true;
}

SIP_User_Agent *SIP_Manager::create_user_agent(const char *uri)
{
    SIP_User_Agent *ua = new SIP_User_Agent(uri);
    _ua.insert(&ua->_link);
    return ua;
}

SIP_User_Agent *SIP_Manager::get_user_agent(const char *uri)
{
    Simple_List<SIP_User_Agent>::Iterator it = _ua.begin();
    while (it != _ua.end())
    {
        SIP_User_Agent *ua = it->object();
        it++;

        if (!strcmp(ua->get_uri(), uri))
            return ua;
    }
    return 0;
}

int SIP_Manager::send_message(SIP_Message *msg)
{
    char *destination = 0;
    char aux_dest[512];
    unsigned short port = 0;

    SIP_Header_Via *via = (SIP_Header_Via *) msg->get_header(SIP_HEADER_VIA);

    if (msg->get_msg_type() == SIP_RESPONSE)
    {
        destination = (char *) via->get_received();
        if (!destination)
            destination = (char *) via->get_sent_by();
        port = via->get_port();
    }else
    {
        //via->set_sent_by(0, _host_ip);
        //via->set_port(0, _host_port);
        //via->set_transport(0, SIP_TRANSPORT_UDP);

        const char *request_uri = ((SIP_Request *) msg)->get_request_uri();

        SIP_Header_Route *route = (SIP_Header_Route *) msg->get_header(SIP_HEADER_ROUTE);
        const char *dest = 0;

        if ((route) && (route->is_lr()))
            dest = route->get_address();
        else //if ((!route) || (!route->is_lr()) /*Strict Route*/ )
            dest = request_uri;

        char aux[255];
        strcpy(aux_dest, dest);
        match(aux_dest, ":" , aux);
        skip(aux_dest, " \t");
        match(aux_dest, "@" , aux);
        skip(aux_dest, " \t");
        destination = aux_dest;
        port = 5060;
    }

    if ((!destination) || (port == 0))
    {
        db<SIP_Manager>(WRN) << "SIP_Manager::send_message -> Failed to send message (dest="
                             << destination << ", port=" << port << "\n";
        return -1;
    }

    _send_buffer[0] = 0;
    msg->encode(_send_buffer);
    int length = strlen(_send_buffer);

    return _socket.send_data(destination, port, _send_buffer, length);
}

int SIP_Manager::SIP_Socket::send_data(const char *destination, unsigned short port, const char *data, int length)
{
    UDP::Address dst(IP::Address(destination), port);
    remote(dst);

    db<SIP_Manager>(TRC) << "SIP_Manager::SIP_Socket::send_data -> Sending data to " << destination
                         << ":" << port << " (size = " << length << ")..\n" << data << "\n";

    if (send(data, length) <= 0)
    {
        db<SIP_Manager>(WRN) << "SIP_Manager::SIP_Socket::send_data -> Failed to send data\n";
        return -1;
    }

    return 0;
}

void SIP_Manager::SIP_Socket::received(const UDP::Address &src, const char *data, unsigned int size)
{
    char remote_addr[20];
    src.ip().to_string(remote_addr);
    unsigned short remote_port = src.port();
    ((char *) data)[size] = 0;

    db<SIP_Manager>(TRC) << "SIP_Manager::SIP_Socket::received -> Received message from " << remote_addr
                         << ":" << remote_port << " (size = " << size << ")..\n" << data << "\n";

    SIP_Message *msg = SIP_Message::decode_msg(data);

    if (!msg)
    {
        db<SIP_Manager>(WRN) << "SIP_Manager::SIP_Socket::received -> Failed to decode SIP message received\n";
        return;
    }

    SIP_Header_Via *header_via = (SIP_Header_Via *) msg->get_header(SIP_HEADER_VIA);

    if ((!msg->get_header(SIP_HEADER_CSEQ)) || (!msg->get_header(SIP_HEADER_CALLID)) ||
        (!msg->get_header(SIP_HEADER_FROM)) || (!msg->get_header(SIP_HEADER_TO)) ||
        (!header_via))
    {
        db<SIP_Manager>(WRN) << "SIP_Manager::SIP_Socket::received -> Invalid SIP message received\n";
        return;
    }

    bool ok = false;
    const char *sent_by = header_via->get_sent_by();
    if (sent_by)
    {
        if (msg->get_msg_type() == SIP_RESPONSE)
        {
            if (!strcmp(sent_by, SIP_Manager::get_instance()->get_host_ip()))
                ok = true;
        }else
        {
            if (strcmp(sent_by, remote_addr))
                header_via->set_received(remote_addr);
            ok = true;
        }
    }

    if (!ok)
    {
        db<SIP_Manager>(WRN) << "SIP_Manager::SIP_Socket::received -> Incorrect message...\n";
        delete msg;
        return;
    }

    SIP_Message_Type type = msg->get_msg_type();
    db<SIP_Manager>(TRC) << "SIP_Manager::SIP_Socket::received -> Message received (type=" << type << ")\n";

    if (type == SIP_RESPONSE)
    {
        const char *uri = ((SIP_Header_From *) msg->get_header(SIP_HEADER_FROM))->get_address();
        SIP_User_Agent *ua = SIP_Manager::get_instance()->get_user_agent(uri);

        if (!ua)
            db<SIP_Manager>(INF) << "SIP_Manager::SIP_Socket::received -> Ignoring invalid response\n";
        else
            ua->receive_response((SIP_Response *) msg);
    }else
    {
        const char *uri = ((SIP_Request *) msg)->get_request_uri();
        SIP_User_Agent *ua = SIP_Manager::get_instance()->get_user_agent(uri);

        if (!ua)
            db<SIP_Manager>(INF) << "SIP_Manager::SIP_Socket::received -> Ignoring invalid request\n";
        else
            ua->receive_request((SIP_Request *) msg);
    }

    if (msg->get_can_delete())
        delete msg;
}

void SIP_Manager::random(char *buffer)
{
    long value = Pseudo_Random::random();
    if (value < 0)
        value *= -1;
    itoa(value, buffer);
}

//-------------------------------------------

void Send_RTP::send_data(const char *destination, unsigned short port, const char *data, unsigned int size)
{
    _buffer[0] = 0x80; //(_version << 6) && 0xc0;
    _buffer[1] = (_sequence == 0x016a) ? 0x80 : 0x00;
    _buffer[2] = _sequence >> 8;
    _buffer[3] = _sequence;
    _buffer[4] = _timestamp >> 24;
    _buffer[5] = _timestamp >> 16;
    _buffer[6] = _timestamp >> 8;
    _buffer[7] = _timestamp;
    _buffer[8] = _ssrc >> 24;
    _buffer[9] = _ssrc >> 16;
    _buffer[10] = _ssrc >> 8;
    _buffer[11] = _ssrc;
    memcpy(&_buffer[12], data, size);

    char aux_dest[512];
    char aux[255];
    strcpy(aux_dest, destination);
    match(aux_dest, ":" , aux);
    skip(aux_dest, " \t");
    match(aux_dest, "@" , aux);
    skip(aux_dest, " \t");
    destination = aux_dest;

    //db<Send_RTP>(INF) << "Send_RTP::send_data -> Sending data to " << destination << ":" << port << " (size: " <<
    //      size << ", seq: " << _sequence << ", timestamp: " << _timestamp << ")\n";

    UDP::Address dst(IP::Address(destination), port);
    _socket.remote(dst);

    if (_socket.send(_buffer, size + 12) <= 0)
    {
        db<Send_RTP>(WRN) << "Send_RTP::send_data -> Failed to send data\n";
        //return;
    }

    _sequence++;
    _timestamp += 0xA0;
}

__END_SYS
