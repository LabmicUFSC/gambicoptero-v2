#include <system/config.h>

#ifdef __NIC_H

#include <sip_manager.h>
#include <sip_user_agent.h>
#include <utility/random.h>

__BEGIN_SYS

USER_CALLBACK SIP_Manager::_callback = 0;
SIP_Manager *SIP_Manager::_instance = 0;
bool SIP_Manager::_terminated = false;

SIP_Manager::~SIP_Manager()
{
    _terminated = true;

    if (_thread_receive)
    {
        delete _thread_receive;
        _thread_receive = 0;
    }

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

    _thread_receive = new Thread(SIP_Manager::receive_thread);
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

    _snd_buffer[0] = 0;
    msg->encode(_snd_buffer);
    int length = strlen(_snd_buffer);

    UDP::Address dst(IP::Address(destination), port);
    _channel.remote(dst);

    db<SIP_Manager>(TRC) << "SIP_Manager::send_message -> Sending data to " << destination
                         << ":" << port << " (size = " << length << ")..\n" << _snd_buffer << "\n";

    if (_channel.send(_snd_buffer, length) <= 0)
    {
        db<SIP_Manager>(WRN) << "SIP_Manager::send_message -> Failed to send data\n";
        return -1;
    }

    return 0;
}

int SIP_Manager::receive_thread()
{
    db<SIP_Manager>(INF) << "SIP_Manager::receive_thread -> Started..\n";

    SIP_Manager *manager = SIP_Manager::get_instance();

    UDP::Address src;
    char remote_addr[20];
    int size;

    while (!_terminated)
    {
        manager->_rcv_buffer[0] = 0;

        size = manager->_channel.receive(&src, manager->_rcv_buffer, MAX_MSG_SIZE);
        if (size < 0)
        {
            db<SIP_Manager>(INF) << "SIP_Manager::receive_thread -> Failed receiving message\n";
            continue;
        }

        if (_terminated)
            break;

        src.ip().to_string(remote_addr);
        unsigned short remote_port = src.port();
        manager->_rcv_buffer[size] = 0;

        db<SIP_Manager>(TRC) << "SIP_Manager::receive_thread -> Received message from " << remote_addr
                             << ":" << remote_port << " (size = " << size << ")..\n" << manager->_rcv_buffer << "\n";

        SIP_Message *msg = SIP_Message::decode_msg(manager->_rcv_buffer);

        if (!msg)
        {
            db<SIP_Manager>(WRN) << "SIP_Manager::receive_thread -> Failed to decode SIP message received\n";
            continue;
        }

        SIP_Header_Via *header_via = (SIP_Header_Via *) msg->get_header(SIP_HEADER_VIA);

        if ((!msg->get_header(SIP_HEADER_CSEQ)) || (!msg->get_header(SIP_HEADER_CALLID)) ||
            (!msg->get_header(SIP_HEADER_FROM)) || (!msg->get_header(SIP_HEADER_TO)) ||
            (!header_via))
        {
            db<SIP_Manager>(WRN) << "SIP_Manager::receive_thread -> Invalid SIP message received\n";
            continue;
        }

        bool ok = false;
        const char *sent_by = header_via->get_sent_by();
        if (sent_by)
        {
            if (msg->get_msg_type() == SIP_RESPONSE)
            {
                if (!strcmp(sent_by, manager->get_host_ip()))
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
            db<SIP_Manager>(WRN) << "SIP_Manager::receive_thread -> Incorrect message...\n";
            delete msg;
            continue;
        }

        SIP_Message_Type type = msg->get_msg_type();
        db<SIP_Manager>(TRC) << "SIP_Manager::receive_thread -> Message received (type=" << type << ")\n";

        if (type == SIP_RESPONSE)
        {
            const char *uri = ((SIP_Header_From *) msg->get_header(SIP_HEADER_FROM))->get_address();
            SIP_User_Agent *ua = manager->get_user_agent(uri);

            if (!ua)
                db<SIP_Manager>(INF) << "SIP_Manager::receive_thread -> Ignoring invalid response\n";
            else
                ua->receive_response((SIP_Response *) msg);
        }else
        {
            const char *uri = ((SIP_Request *) msg)->get_request_uri();
            SIP_User_Agent *ua = manager->get_user_agent(uri);

            if (!ua)
                db<SIP_Manager>(INF) << "SIP_Manager::receive_thread -> Ignoring invalid request\n";
            else
                ua->receive_request((SIP_Request *) msg);
        }

        if (msg->get_can_delete())
            delete msg;
    }

    db<SIP_Manager>(INF) << "SIP_Manager::receive_thread -> Stopped..\n";
    return 0;
}

void SIP_Manager::random(char *buffer)
{
    long value = Pseudo_Random::random();
    if (value < 0)
        value *= -1;
    itoa(value, buffer);
}

__END_SYS

#endif
