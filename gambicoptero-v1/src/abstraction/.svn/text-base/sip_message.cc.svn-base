#include <sip_message.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <sip_header.h>

__BEGIN_SYS

SIP_Message::~SIP_Message()
{
    Simple_List<SIP_Header>::Iterator it = _headers.begin();
    while (it != _headers.end())
    {
        SIP_Header *header = it->object();
        _headers.remove(it++);
        delete header;
    }

    if (_body)
        delete _body;
}

SIP_Message *SIP_Message::decode_msg(const char *sip_msg)
{
    char *p = (char *) sip_msg;
    remove_lws(p);

    char type[30];
    match(p, " ", type);
    trim(type);
    SIP_Message_Type msg_type = get_msg_type(type);

    if (msg_type == SIP_MESSAGE_TYPE_INVALID)
    {
        db<SIP_Message>(WRN) << "SIP_Message::decode_msg -> Invalid message type\n";
        return 0;
    }

    SIP_Message *msg = 0;

    if (msg_type == SIP_RESPONSE)
        msg = new SIP_Response();
    else
        msg = new SIP_Request(msg_type);

    if ((msg) && (!msg->parse(p)))
    {
        db<SIP_Message>(WRN) << "SIP_Message::decode_msg -> Failed to parse " << type << " message type\n";
        delete msg;
        msg = 0;
    }

    return msg;
}

SIP_Message_Type SIP_Message::get_msg_type(const char *sip_msg)
{
    SIP_Message_Type type = SIP_MESSAGE_TYPE_INVALID;

    if (!strcmp(sip_msg, "ACK"))
        type = SIP_REQUEST_ACK;

    else if (!strcmp(sip_msg, "BYE"))
        type = SIP_REQUEST_BYE;

    else if (!strcmp(sip_msg, "INVITE"))
        type = SIP_REQUEST_INVITE;

    else if (!strcmp(sip_msg, "MESSAGE"))
        type = SIP_REQUEST_MESSAGE;

    else if (!strcmp(sip_msg, "NOTIFY"))
        type = SIP_REQUEST_NOTIFY;

    else if (!strcmp(sip_msg, "SUBSCRIBE"))
        type = SIP_REQUEST_SUBSCRIBE;

    else if (!strcmp(sip_msg, SIP_VERSION))
        type = SIP_RESPONSE;

    return type;
}

const char *SIP_Message::get_msg_type(SIP_Message_Type type)
{
    const char *sipMsg = 0;

    switch (type)
    {
        case SIP_REQUEST_ACK:        sipMsg = "ACK";       break;
        case SIP_REQUEST_BYE:        sipMsg = "BYE";       break;
        case SIP_REQUEST_INVITE:     sipMsg = "INVITE";    break;
        case SIP_REQUEST_MESSAGE:    sipMsg = "MESSAGE";   break;
        case SIP_REQUEST_NOTIFY:     sipMsg = "NOTIFY";    break;
        case SIP_REQUEST_SUBSCRIBE:  sipMsg = "SUBSCRIBE"; break;
        case SIP_RESPONSE:           sipMsg = SIP_VERSION; break;
        default: break;
    }

    return sipMsg;
}

bool SIP_Message::parse(const char *sip_msg)
{
    if (!parse_start_line(sip_msg))
        return false;

    if (!parse_header(sip_msg))
        return false;

    if (!parse_body(sip_msg))
        return false;

    return true;
}

bool SIP_Message::parse_header(const char *sip_msg)
{
    char line[MAX_LINE];
    Simple_List<SIP_Header> header;
    while (true)
    {
        bool ret = get_line(sip_msg, line);
        if ((!ret) || (!strcmp(line, "")))
            return true;

        SIP_Header::decode_header(line, header);
        add_headers(header);
    }

    return true;
}

bool SIP_Message::parse_body(const char *sip_msg)
{
    SIP_Header_Content_Type *content_type = (SIP_Header_Content_Type *) get_header(SIP_HEADER_CONTENT_TYPE);
    if (content_type)
        _body = SIP_Body::decode_body(sip_msg, content_type->get_content_type());
    return true;
}

bool SIP_Message::encode(char *sip_msg)
{
    char body_msg[512];
    body_msg[0] = 0;

    if (!encode_start_line(sip_msg))
        return false;

    if (!encode_body(body_msg))
        return false;

    if (!encode_header(sip_msg, body_msg))
        return false;

    return true;
}

bool SIP_Message::encode_header(char *sip_msg, char *body_msg)
{
    if (_body)
    {
        SIP_Header_Content_Type *header_content_type = (SIP_Header_Content_Type *) get_header(SIP_HEADER_CONTENT_TYPE);
        if (!header_content_type)
        {
            header_content_type = new SIP_Header_Content_Type();
            add_header(header_content_type);
        }
        header_content_type->set_content_type(_body->get_body_type());
    }

    SIP_Header_Content_Length *header_content_length = (SIP_Header_Content_Length *) get_header(SIP_HEADER_CONTENT_LENGTH);
    if (!header_content_length)
    {
        header_content_length = new SIP_Header_Content_Length();
        add_header(header_content_length);
    }
    header_content_length->set_number(strlen(body_msg));

    char *p;
    Simple_List<SIP_Header>::Iterator it = _headers.begin();
    while (it != _headers.end())
    {
        SIP_Header *header = it->object();
        it++;

        p = sip_msg + strlen(sip_msg);

        if (header)
            header->encode(p);
    }

    strcat(sip_msg, "\r\n");
    strcat(sip_msg, body_msg);
    return true;
}

bool SIP_Message::encode_body(char *sip_msg)
{
    if (_body)
        _body->encode(sip_msg);
    return true;
}

void SIP_Message::add_header(SIP_Header *header)
{
    if (header)
        _headers.insert(&header->_link);
}

void SIP_Message::add_headers(Simple_List<SIP_Header> &headers)
{
    Simple_List<SIP_Header>::Iterator it = headers.begin();
    while (it != headers.end())
    {
        SIP_Header *header = it->object();
        it++;
        headers.remove_head();
        _headers.insert(&header->_link);
    }
}

SIP_Header *SIP_Message::get_header(unsigned short type, unsigned short pos)
{
    int count = -1;
    Simple_List<SIP_Header>::Iterator it = _headers.begin();
    while (it != _headers.end())
    {
        SIP_Header *header = it->object();
        it++;

        if (header->get_header_type() == type)
        {
            count++;
            if (count == pos)
                return header;
        }
    }
    return 0;
}

int SIP_Message::get_num_header(unsigned short type)
{
    int count = 0;
    Simple_List<SIP_Header>::Iterator it = _headers.begin();
    while (it != _headers.end())
    {
        SIP_Header *header = it->object();
        it++;

        if (header->get_header_type() == type)
            count++;
    }
    return count;
}

SIP_Transport_Type SIP_Message::get_transport_type(const char *type)
{
    SIP_Transport_Type transport = SIP_TRANSPORT_TYPE_INVALID;

    if (!strcmp(type, "UDP"))
        transport = SIP_TRANSPORT_UDP;

    //else if (!strcmp(type, "TCP"))
    //    transport = SIP_TRANSPORT_TCP;

    return transport;
}

const char *SIP_Message::get_transport_type(const SIP_Transport_Type type)
{
    const char *transport = 0;

    switch (type)
    {
        case SIP_TRANSPORT_UDP: transport = "UDP"; break;
        //case SIP_TRANSPORT_TCP: transport = "TCP"; break;
        default: break;
    }

    return transport;
}

//-------------------------------------------

SIP_Request::~SIP_Request()
{
    if (_request_uri)
        delete _request_uri;

    if (_sip_version)
        delete _sip_version;
}

bool SIP_Request::parse_start_line(const char *sip_msg)
{
    char line[MAX_LINE];
    get_line(sip_msg, line);

    _method = get_msg_type();

    char result[255];
    skip(line, " \t");
    match(line, " ", result);
    _request_uri = create_string(result);
    if (!_request_uri)
    {
        db<SIP_Request>(WRN) << "SIP_Request::parse_start_line -> Request URI parse failed\n";
        return false;
    }

    trim(line);
    _sip_version = create_string(line);
    if (!_sip_version)
    {
        db<SIP_Request>(WRN) << "SIP_Request::parse_start_line -> SIP version parse failed\n";
        return false;
    }

    return true;
}

bool SIP_Request::encode_start_line(char *sip_msg)
{
    strcat(sip_msg, SIP_Message::get_msg_type(_method));
    strcat(sip_msg, " ");
    strcat(sip_msg, _request_uri);
    strcat(sip_msg, " ");
    strcat(sip_msg, _sip_version);
    strcat(sip_msg, "\r\n");
    return true;
}

void SIP_Request::set_request_line(const SIP_Message_Type msg_type, const char *request_uri, const char *sip_version)
{
    _method = msg_type;

    if (_request_uri)
        delete _request_uri;
    _request_uri = create_string(request_uri);

    if (_sip_version)
        delete _sip_version;
    _sip_version = create_string(sip_version);
}

//-------------------------------------------

SIP_Status_Code SIP_Response::_status_codes[] =
{
    //Informational
    { 100, "Trying" },
    { 180, "Ringing" },
    { 181, "Call is Being Forwarded" },
    { 182, "Queued" },
    { 183, "Session Progress" },

    //Success
    { 200, "OK" },
    { 202, "Accepted" },

    //Redirection
    { 300, "Multiple Choices" },
    { 301, "Moved Permanently" },
    { 302, "Moved Temporarily" },
    //{ 303, "See Other" },
    { 305, "Use Proxy" },
    { 380, "Alternative Service" },

    //Client-Error
    { 400, "Bad Request" },
    { 401, "Unauthorized" },
    { 402, "Payment Required" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
    { 405, "Method Not Allowed" },
    { 406, "Not Acceptable" },
    { 407, "Proxy Authentication Required" },
    { 408, "Request Timeout" },
    { 409, "Conflict" },
    { 410, "Gone" },
    //{ 411, "Length Required" },
    { 413, "Request Entity Too Large" },
    { 414, "Request-URI Too Large" },
    { 415, "Unsupported Media Type" },
    { 420, "Bad Extension" },
    { 480, "Temporarily Unavailable" },
    { 481, "Call Leg/Transaction Does Not Exist" },
    { 482, "Loop Detected" },
    { 483, "Too Many Hops" },
    { 484, "Address Incomplete" },
    { 485, "Ambiguous" },
    { 486, "Busy Here" },
    { 487, "Request Terminated" },
    { 488, "Not Acceptable Here" },
    { 489, "Bad Event" },

    //Server-Error
    { 500, "Server Internal Error" },
    { 501, "Not Implemented" },
    { 502, "Bad Gateway" },
    { 503, "Service Unavailable" },
    { 504, "Server Timed-out" },
    { 505, "Version Not Supported" },
    { 513, "Message Too Large" },

    //Global-Failure
    { 600, "Busy Everywhere" },
    { 603, "Decline" },
    { 604, "Does Not Exist Anywhere" },
    { 606, "Not Acceptable" },
    { 0, "Unknown" },
};

SIP_Response::SIP_Response(unsigned short status_code)
{
    int i = 0;
    while (_status_codes[i]._code != 0)
    {
        if (_status_codes[i]._code == status_code)
        {
            set_status_line(SIP_VERSION, status_code, _status_codes[i]._reason_phrase);
            break;
        }
        i++;
    }
}

SIP_Response::~SIP_Response()
{
    if (_sip_version)
        delete _sip_version;

    if (_reason_phrase)
        delete _reason_phrase;
}

bool SIP_Response::parse_start_line(const char *sip_msg)
{
    char line[MAX_LINE];
    get_line(sip_msg, line);

    _sip_version = create_string(SIP_VERSION);
    if (!_sip_version)
    {
        db<SIP_Response>(WRN) << "SIP_Response::parse_start_line -> SIP version parse failed\n";
        return false;
    }

    char result[255];
    skip(line, " \t");
    match(line, " ", result);
    int size = strlen(result);
    if (size == 0)
    {
        db<SIP_Response>(WRN) << "SIP_Response::parse_start_line -> Status code parse failed\n";
        return false;
    }
    _status_code = (unsigned short) atol(result);

    trim(line);
    _reason_phrase = create_string(line);
    if (!_reason_phrase)
    {
        db<SIP_Response>(WRN) << "SIP_Response::parse_start_line -> Reason phrase parse failed\n";
        return false;
    }

    return true;
}

bool SIP_Response::encode_start_line(char *sip_msg)
{
    strcat(sip_msg, _sip_version);
    strcat(sip_msg, " ");

    char value[11];
    itoa(_status_code, value);
    strcat(sip_msg, value);
    strcat(sip_msg, " ");

    strcat(sip_msg, _reason_phrase);
    strcat(sip_msg, "\r\n");
    return true;
}

void SIP_Response::set_status_line(const char *sip_version, unsigned short status_code, const char *reason_phrase)
{
    if (_sip_version)
        delete _sip_version;
    _sip_version = create_string(sip_version);

    _status_code = status_code;

    if (_reason_phrase)
        delete _reason_phrase;
    _reason_phrase = create_string(reason_phrase);
}

__END_SYS
