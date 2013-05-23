#include <sip_header.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <sip_message.h>

__BEGIN_SYS

void SIP_Header::decode_header(char *line, Simple_List<SIP_Header> &ret)
{
    SIP_Header_Type header_type = SIP_HEADER_TYPE_INVALID;

    char type[255];
    bool matched = match(line, ":", type);
    if (!matched)
        return;

    if (!strcmp(type, "Allow"))
        header_type = SIP_HEADER_ALLOW;

    else if (!strcmp(type, "Allow-Events") || !strcmp(type, "u"))
        header_type = SIP_HEADER_ALLOW_EVENTS;

    else if (!strcmp(type, "Call-ID") || !strcmp(type, "i"))
        header_type = SIP_HEADER_CALLID;

    else if (!strcmp(type, "Contact") || !strcmp(type, "m"))
        header_type = SIP_HEADER_CONTACT;

    else if (!strcmp(type, "Content-Disposition"))
        header_type = SIP_HEADER_CONTENT_DISPOSITION;

    else if (!strcmp(type, "Content-Length") || !strcmp(type, "l"))
        header_type = SIP_HEADER_CONTENT_LENGTH;

    else if (!strcmp(type, "Content-Type") || !strcmp(type, "c"))
        header_type = SIP_HEADER_CONTENT_TYPE;

    else if (!strcmp(type, "CSeq"))
        header_type = SIP_HEADER_CSEQ;

    else if (!strcmp(type, "Event") || !strcmp(type, "o"))
        header_type = SIP_HEADER_EVENT;

    else if (!strcmp(type, "Expires"))
        header_type = SIP_HEADER_EXPIRES;

    else if (!strcmp(type, "From") || !strcmp(type, "f"))
        header_type = SIP_HEADER_FROM;

    else if (!strcmp(type, "Max-Forwards"))
        header_type = SIP_HEADER_MAX_FORWARDS;

    else if (!strcmp(type, "Record-Route"))
        header_type = SIP_HEADER_RECORD_ROUTE;

    else if (!strcmp(type, "Require"))
        header_type = SIP_HEADER_REQUIRE;

    else if (!strcmp(type, "Route"))
        header_type = SIP_HEADER_ROUTE;

    else if (!strcmp(type, "Subscription-State"))
        header_type = SIP_HEADER_SUBSCRIPTION_STATE;

    else if (!strcmp(type, "To") || !strcmp(type, "t"))
        header_type = SIP_HEADER_TO;

    else if (!strcmp(type, "Unsupported"))
        header_type = SIP_HEADER_UNSUPPORTED;

    else if (!strcmp(type, "Via") || !strcmp(type, "v"))
        header_type = SIP_HEADER_VIA;

    else
    {
        db<SIP_Header>(WRN) << "SIP_Header::decode_header -> Invalid header type (" << type << ")\n";
        return;
    }

    char result[MAX_LINE] = {0};
    matched = false;
    do
    {
        if ((header_type == SIP_HEADER_CONTACT) || (header_type == SIP_HEADER_RECORD_ROUTE) || (header_type == SIP_HEADER_ROUTE) || (header_type == SIP_HEADER_VIA))
            matched = match(line, ",", result);
        else
            strcpy(result, line);

        trim(result);

        SIP_Header *header = create_header(header_type);
        ret.insert(&header->_link);

        if (!header->parse(result))
        {
            db<SIP_Header>(WRN) << "SIP_Header::decode_header -> Failed to parse " << type << " header type\n";

            Simple_List<SIP_Header>::Iterator it = ret.begin();
            while (it != ret.end())
            {
                SIP_Header *header = it->object();
                ret.remove(it++);
                delete header;
            }
            break;
        }
    }while (matched);

    return;
}

SIP_Header *SIP_Header::create_header(SIP_Header_Type type)
{
    SIP_Header *header = 0;

    switch (type)
    {
        case SIP_HEADER_ALLOW:               header = new SIP_Header_Allow();               break;
        case SIP_HEADER_ALLOW_EVENTS:        header = new SIP_Header_Allow_Events();        break;
        case SIP_HEADER_CALLID:              header = new SIP_Header_Call_ID();             break;
        case SIP_HEADER_CONTACT:             header = new SIP_Header_Contact();             break;
        case SIP_HEADER_CONTENT_DISPOSITION: header = new SIP_Header_Content_Disposition(); break;
        case SIP_HEADER_CONTENT_LENGTH:      header = new SIP_Header_Content_Length();      break;
        case SIP_HEADER_CONTENT_TYPE:        header = new SIP_Header_Content_Type();        break;
        case SIP_HEADER_CSEQ:                header = new SIP_Header_CSeq();                break;
        case SIP_HEADER_EVENT:               header = new SIP_Header_Event();               break;
        case SIP_HEADER_EXPIRES:             header = new SIP_Header_Expires();             break;
        case SIP_HEADER_FROM:                header = new SIP_Header_From();                break;
        case SIP_HEADER_MAX_FORWARDS:        header = new SIP_Header_Max_Forwards();        break;
        case SIP_HEADER_RECORD_ROUTE:        header = new SIP_Header_Record_Route();        break;
        case SIP_HEADER_REQUIRE:             header = new SIP_Header_Require();             break;
        case SIP_HEADER_ROUTE:               header = new SIP_Header_Route();               break;
        case SIP_HEADER_SUBSCRIPTION_STATE:  header = new SIP_Header_Subscription_State();  break;
        case SIP_HEADER_TO:                  header = new SIP_Header_To();                  break;
        case SIP_HEADER_UNSUPPORTED:         header = new SIP_Header_Unsupported();         break;
        case SIP_HEADER_VIA:                 header = new SIP_Header_Via();                 break;
        default: break;
    }

    return header;
}

//-------------------------------------------

bool Value_String::parse(char *sip_msg)
{
    trim(sip_msg);
    _string = create_string(sip_msg);
    if (!_string)
        return false;

    return true;
}

bool Value_String::encode(char *sip_msg)
{
    switch (get_header_type())
    {
        case SIP_HEADER_CALLID:              strcat(sip_msg, "Call-ID: ");             break;
        case SIP_HEADER_CONTENT_DISPOSITION: strcat(sip_msg, "Content-Disposition: "); break;
        default: return false;
    }

    strcat(sip_msg, _string);
    strcat(sip_msg, "\r\n");
    return true;
}

void Value_String::set_string(const char *string)
{
    if (_string)
        delete _string;
    _string = create_string(string);
}

//-------------------------------------------

bool Value_Number::parse(char *sip_msg)
{
    trim(sip_msg);
    _number = (int) atol(sip_msg);
    return true;
}

bool Value_Number::encode(char *sip_msg)
{
    switch (get_header_type())
    {
        case SIP_HEADER_CONTENT_LENGTH: strcat(sip_msg, "Content-Length: "); break;
        case SIP_HEADER_EXPIRES:        strcat(sip_msg, "Expires: ");        break;
        case SIP_HEADER_MAX_FORWARDS:   strcat(sip_msg, "Max-Forwards: ");   break;
        default: return false;
    }

    char value[11];
    itoa(_number, value);

    strcat(sip_msg, value);
    strcat(sip_msg, "\r\n");
    return true;
}

//-------------------------------------------

Value_Address_Params::Value_Address_Params(Value_Address_Params &value)
{
    _display = create_string(value._display);
    _address = create_string(value._address);
    _tag = create_string(value._tag);
    _lr = value._lr;
}

Value_Address_Params::~Value_Address_Params()
{
    if (_display)
        delete _display;
    if (_address)
        delete _address;
    if (_tag)
        delete _tag;
}

bool Value_Address_Params::parse(char *sip_msg)
{
    char result[512];
    bool matched;
    int i = 0;

    do
    {
        matched = match(sip_msg, ";", result);
        trim(result);

        if (i == 0)
        {
            char aux[255];
            bool matched1 = match(result, "<", aux, true);
            if (matched1)
            {
                trim(aux);
                _display = create_string(aux);
            }

            bool matched2 = match(result, ";", aux, true);
            if (matched2)
            {
                trim(aux);
                _address = create_string(aux);

                if (contains(result, "lr"))
                    _lr = true;

            }else if (matched1)
            {
                matched1 = match(result, ">", aux, true);
                if (!matched1)
                    return false;

                trim(aux);
                _address = create_string(aux);
            }else
                _address = create_string(result);

            if (!_address)
                return false;
        }else
        {
            if (start_with(result, "tag="))
            {
                _tag = create_string(&result[4]);
                if (!_tag)
                    return false;
            }
        }
        i++;
    }while (matched);

    return true;
}

bool Value_Address_Params::encode(char *sip_msg)
{
    switch (get_header_type())
    {
        case SIP_HEADER_CONTACT:
        {
            strcat(sip_msg, "Contact: ");
            if (!strcmp(_address, "*"))
            {
                strcat(sip_msg, "*\r\n");
                return true;
            }
            break;
        }
        case SIP_HEADER_FROM:         strcat(sip_msg, "From: ");         break;
        case SIP_HEADER_RECORD_ROUTE: strcat(sip_msg, "Record-Route: "); break;
        case SIP_HEADER_ROUTE:        strcat(sip_msg, "Route: ");        break;
        case SIP_HEADER_TO:           strcat(sip_msg, "To: ");           break;
        default: return false;
    }

    if (_display)
    {
        strcat(sip_msg, _display);
        strcat(sip_msg, " ");
    }

    strcat(sip_msg, "<");
    strcat(sip_msg, _address);

    if (_lr)
        strcat(sip_msg, ";lr");

    strcat(sip_msg, ">");

    if (_tag)
    {
        strcat(sip_msg, ";tag=");
        strcat(sip_msg, _tag);
    }

    strcat(sip_msg, "\r\n");
    return true;
}

void Value_Address_Params::set_address(const char *address, bool lr)
{
    if (_address)
        delete _address;
    _address = create_string(address);
    _lr = lr;
}

void Value_Address_Params::set_tag(const char *tag)
{
    if (_tag)
        delete _tag;
    _tag = create_string(tag);
}

//-------------------------------------------

Value_Options::Value_Options(Value_Options &value)
{
    Simple_List<char>::Iterator it = value._options.begin();
    while (it != value._options.end())
    {
        char *option = it->object();
        it++;

        if (!option)
            continue;

        char *option2 = create_string(option);
        if (!option2)
            continue;

        Simple_List<char>::Element *e_option2 = new Simple_List<char>::Element(option2);
        _options.insert(e_option2);
    }
}

Value_Options::~Value_Options()
{
    Simple_List<char>::Iterator it = _options.begin();
    while (it != _options.end())
    {
        Simple_List<char>::Element *el = it++;
        _options.remove(el);
        delete el->object();
        delete el;
    }
}

bool Value_Options::parse(char *sip_msg)
{
    char result[512];
    bool matched;

    do
    {
        matched = match(sip_msg, ",", result);
        trim(result);
        char *option = create_string(result);
        if (!option)
            return false;

        Simple_List<char>::Element *e_option = new Simple_List<char>::Element(option);
        _options.insert(e_option);
    }while (matched);

    return true;
}

bool Value_Options::encode(char *sip_msg)
{
    switch (get_header_type())
    {
        case SIP_HEADER_ALLOW:        strcat(sip_msg, "Allow: ");        break;
        case SIP_HEADER_ALLOW_EVENTS: strcat(sip_msg, "Allow-Events: "); break;
        case SIP_HEADER_REQUIRE:      strcat(sip_msg, "Require: ");      break;
        case SIP_HEADER_UNSUPPORTED:  strcat(sip_msg, "Unsupported: ");  break;
        default: return false;
    }

    Simple_List<char>::Iterator it = _options.begin();
    while (it != _options.end())
    {
        if (it != _options.begin())
            strcat(sip_msg, ", ");

        char *option = it->object();
        it++;

        if (!option)
            continue;

        strcat(sip_msg, option);
    }

    strcat(sip_msg, "\r\n");
    return true;
}

const char *Value_Options::get_option(int pos)
{
    int i = 0;
    Simple_List<char>::Iterator it = _options.begin();
    while (it != _options.end())
    {
        if (i == pos)
            return it->object();
        i++; it++;
    }
    return 0;
}

void Value_Options::add_option(const char *option)
{
    char *option2 = create_string(option);
    if (!option2)
        return;
    Simple_List<char>::Element *e_option2 = new Simple_List<char>::Element(option2);
    _options.insert(e_option2);
}

//-------------------------------------------

void SIP_Header_Allow::add_allowed(SIP_Message_Type allowed)
{
    const char *allowed2 = SIP_Message::get_msg_type(allowed);
    if (allowed2)
        Value_Options::add_option(allowed2);
}

//-------------------------------------------

bool SIP_Header_Content_Disposition::parse(char *sip_msg)
{
    char result[255];

    match(sip_msg, ";", result);
    trim(result);
    _string = create_string(result);
    if (!_string)
        return false;

    return true;
}

//-------------------------------------------

bool SIP_Header_Content_Type::parse(char *sip_msg)
{
    char result[255];

    match(sip_msg, ";", result);
    trim(result);

    if (!strcmp(result, "application/sdp"))
        _type = SIP_BODY_APPLICATION_SDP;

    else if (!strcmp(result, "application/pidf+xml"))
        _type = SIP_BODY_APPLICATION_PIDF_XML;

    else if (!strcmp(result, "text/plain"))
        _type = SIP_BODY_TEXT_PLAIN;

    else
        return false;

    return true;
}

bool SIP_Header_Content_Type::encode(char *sip_msg)
{
    strcat(sip_msg, "Content-Type: ");

    const char *content_type = 0;
    switch (_type)
    {
        case SIP_BODY_APPLICATION_SDP:      content_type = "application/sdp";      break;
        case SIP_BODY_APPLICATION_PIDF_XML: content_type = "application/pidf+xml"; break;
        case SIP_BODY_TEXT_PLAIN:           content_type = "text/plain";           break;
        default: break;
    }

    strcat(sip_msg, content_type);
    strcat(sip_msg, "\r\n");
    return true;
}

//-------------------------------------------

bool SIP_Header_CSeq::parse(char *sip_msg)
{
    char result[255];
    bool matched;

    skip(sip_msg, " \t");
    matched = match(sip_msg, " \t", result);
    if (!matched)
        return false;
    trim(result);
    _sequence = (unsigned int) atol(result);

    trim(sip_msg);
    _method = SIP_Message::get_msg_type(sip_msg);
    if (_method == SIP_MESSAGE_TYPE_INVALID)
        return false;

    return true;
}

bool SIP_Header_CSeq::encode(char *sip_msg)
{
    char value[11];
    itoa(_sequence, value);

    strcat(sip_msg, "CSeq: ");
    strcat(sip_msg, value);
    strcat(sip_msg, " ");
    strcat(sip_msg, SIP_Message::get_msg_type(_method));
    strcat(sip_msg, "\r\n");
    return true;
}

//-------------------------------------------

bool SIP_Header_Event::parse(char *sip_msg)
{
    char result[255];
    bool matched;

    matched = match(sip_msg, ";", result);
    trim(result);
    if (!strcmp(result, "presence"))
        _type = SIP_EVENT_PRESENCE;
    else
        return false;

    while (matched)
    {
        matched = match(sip_msg, ";", result);
        trim(result);

        if (start_with(result, "id="))
        {
            _id = create_string(&result[3]);
            if (!_id)
                return false;
        }
    }

    return true;
}

bool SIP_Header_Event::encode(char *sip_msg)
{
    const char *event_type = 0;
    switch (_type)
    {
        case SIP_EVENT_PRESENCE: event_type = "presence"; break;
        default: break;
    }

    strcat(sip_msg, "Event: ");
    strcat(sip_msg, event_type);

    if (_id)
    {
        strcat(sip_msg, ";id=");
        strcat(sip_msg, _id);
    }

    strcat(sip_msg, "\r\n");
    return true;
}

void SIP_Header_Event::set_event(SIP_Event_Package type, const char *id)
{
    _type = type;

    if (_id)
        delete _id;
    _id = create_string(id);
}

//-------------------------------------------

bool SIP_Header_Subscription_State::parse(char *sip_msg)
{
    char result[255];
    bool matched;

    matched = match(sip_msg, ";", result);
    trim(result);

    if (!strcmp(result, "active"))
        _state = SIP_SUBSCRIPTION_STATE_ACTIVE;

    //else if (!strcmp(result, "pending"))
    //	_state = SIP_SUBSCRIPTION_STATE_PENDING;

    else if (!strcmp(result, "terminated"))
        _state = SIP_SUBSCRIPTION_STATE_TERMINATED;

    else
        return false;

    while (matched)
    {
        matched = match(sip_msg, ";", result);
        trim(result);

        if (start_with(result, "expires="))
            _expires = (int) atol(&result[8]);
    }

    return true;
}

bool SIP_Header_Subscription_State::encode(char *sip_msg)
{
    char value[11];
    itoa(_expires, value);

    const char *subscriptionState = 0;
    switch (_state)
    {
        case SIP_SUBSCRIPTION_STATE_ACTIVE:     subscriptionState = "active";     break;
        //case SIP_SUBSCRIPTION_STATE_PENDING:	subscriptionState = "pending";    break;
        case SIP_SUBSCRIPTION_STATE_TERMINATED: subscriptionState = "terminated"; break;
        default: break;
    }

    strcat(sip_msg, "Subscription-State: ");
    strcat(sip_msg, subscriptionState);

    if (_expires >= 0)
    {
        strcat(sip_msg, ";expires=");
        strcat(sip_msg, value);
    }

    strcat(sip_msg, "\r\n");
    return true;
}

//-------------------------------------------

SIP_Header_Via::SIP_Header_Via(SIP_Header_Via &header)
{
    _protocol_name = create_string(header._protocol_name);
    _protocol_version = create_string(header._protocol_version);
    _transport = header._transport;
    _sent_by = create_string(header._sent_by);
    _port = header._port;
    _branch = create_string(header._branch);
    _received = create_string(header._received);
}

SIP_Header_Via::~SIP_Header_Via()
{
    if (_protocol_name)
        delete _protocol_name;
    if (_protocol_version)
        delete _protocol_version;
    if (_sent_by)
        delete _sent_by;
    if (_branch)
        delete _branch;
    if (_received)
        delete _received;
}

bool SIP_Header_Via::parse(char *sip_msg)
{
    char result[255];
    bool matched;

    matched = match(sip_msg, "/", result);
    if (!matched)
        return false;
    trim(result);
    _protocol_name = create_string(result);
    if (!_protocol_name)
        return false;

    matched = match(sip_msg, "/", result);
    if (!matched)
        return false;
    trim(result);
    _protocol_version = create_string(result);
    if (!_protocol_version)
        return false;

    skip(sip_msg, " \t");
    matched = match(sip_msg, " \t", result);
    if (!matched)
        return false;
    trim(result);
    _transport = SIP_Message::get_transport_type(result);
    if (_transport == SIP_TRANSPORT_TYPE_INVALID)
        return false;

    matched = match(sip_msg, ":", result);
    if (matched)
    {
        trim(result);
        _sent_by = create_string(result);
        if (!_sent_by)
            return false;

        matched = match(sip_msg, ";", result);
        trim(result);
        _port = (int) atol(result);
    }else
    {
        matched = match(sip_msg, ";", result);
        trim(result);
        _sent_by = create_string(result);
        if (!_sent_by)
            return false;

        _port = 5060;
    }

    while (matched)
    {
        matched = match(sip_msg, ";", result);
        trim(result);

        if (start_with(result, "branch="))
        {
            _branch = create_string(&result[7]);
            if (!_branch)
                return false;
        }else if (start_with(result, "received="))
        {
            _received = create_string(&result[9]);
            if (!_received)
                return false;
        }
    }

    return true;
}

bool SIP_Header_Via::encode(char *sip_msg)
{
    char value[11];
    itoa(_port, value);

    const char *type = SIP_Message::get_transport_type(_transport);

    strcat(sip_msg, "Via: ");
    strcat(sip_msg, _protocol_name);
    strcat(sip_msg, "/");
    strcat(sip_msg, _protocol_version);
    strcat(sip_msg, "/");
    strcat(sip_msg, type);
    strcat(sip_msg, " ");
    strcat(sip_msg, _sent_by);
    strcat(sip_msg, ":");
    strcat(sip_msg, value);

    if (_branch)
    {
        strcat(sip_msg, ";branch=");
        strcat(sip_msg, _branch);
    }

    if (_received)
    {
        strcat(sip_msg, ";received=");
        strcat(sip_msg, _received);
    }

    strcat(sip_msg, "\r\n");
    return true;
}

void SIP_Header_Via::set_via(const char *protocolName, const char *protocolVersion,
        const SIP_Transport_Type transport, const char *sentBy, unsigned short port,
        const char *branch)
{
    if (_protocol_name)
        delete _protocol_name;
    _protocol_name = create_string(protocolName);

    if (_protocol_version)
        delete _protocol_version;
    _protocol_version = create_string(protocolVersion);

    _transport = transport;

    if (_sent_by)
        delete _sent_by;
    _sent_by = create_string(sentBy);

    _port = port;

    if (_branch)
        delete _branch;
    _branch = create_string(branch);
}

void SIP_Header_Via::set_received(const char *received)
{
    if (_received)
        delete _received;
    _received = create_string(received);
}

__END_SYS
