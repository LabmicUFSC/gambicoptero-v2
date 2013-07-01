#include <sip_body.h>
#include <ip.h>

__BEGIN_SYS

SIP_Body *SIP_Body::decode_body(const char *sip_msg, SIP_Body_Type body_type)
{
    SIP_Body *body = create_body(body_type);

    if (!body)
    {
        db<SIP_Body>(WRN) << "SIP_Body::decode_body -> Invalid body type\n";
        return 0;
    }

    if (!body->parse(sip_msg))
    {
        db<SIP_Body>(WRN) << "SIP_Body::decode_body -> Failed to parse " << body_type << " body type\n";
        delete body;
        body = 0;
    }

    return body;
}

SIP_Body *SIP_Body::create_body(SIP_Body_Type type)
{
    SIP_Body *body = 0;

    switch (type)
    {
        case SIP_BODY_APPLICATION_SDP:          body = new SIP_SDP_Body();        break;
        //case SIP_BODY_APPLICATION_PIDF_XML:   body = new SIP_Pidf_Xml_Body();   break;
        case SIP_BODY_TEXT_PLAIN:               body = new SIP_Text_Plain_Body(); break;
        default: break;
    }

    return body;
}

//-------------------------------------------

bool SIP_SDP_Body::parse(const char *sip_msg)
{
    char line[MAX_LINE];
    char result[255];

    while (true)
    {
        bool ret = get_line(sip_msg, line);
        if ((!ret) || (!strcmp(line, "")))
            break;

        bool matched = match(line, "=", result);
        if (!matched)
            continue;

        if (!strcmp(result, "c"))
        {
            if (_address)
                continue;

            matched = match(line, " ", result);
            if (!matched)
                continue;

            matched = match(line, " ", result);
            if (!matched)
                continue;

            _address = create_string(line);

        }else if (!strcmp(result, "m"))
        {
            if (_port != 0)
                continue;

            matched = match(line, " ", result);
            if (!matched)
                continue;

            matched = match(line, " ", result);
            if (!matched)
                continue;

            _port = (unsigned short) atol(result);
        }
    }

    if ((!_address) || (_port == 0))
        return false;

    return true;
}

//-------------------------------------------

bool SIP_SDP_Body::encode(char *sip_msg)
{
    char value[11];
    itoa(_port, value);

    strcat(sip_msg, "v=0\r\n");
    strcat(sip_msg, "o=- 0 0 IN IP4 ");
    strcat(sip_msg, _address);
    strcat(sip_msg, "\r\n");
    strcat(sip_msg, "s=-\r\n");
    strcat(sip_msg, "c=IN IP4 ");
    strcat(sip_msg, _address);
    strcat(sip_msg, "\r\n");
    strcat(sip_msg, "t=0 0\r\n");
    strcat(sip_msg, "m=audio ");
    strcat(sip_msg, value);
    strcat(sip_msg, " RTP/AVP 0\r\n");
    strcat(sip_msg, "a=rtpmap:0 pcmu/8000\r\n");
    strcat(sip_msg, "a=sendonly\r\n");
    return true;
}

void SIP_SDP_Body::set_address(const char *address)
{
    if (_address)
        delete _address;
    _address = create_string(address);
}

//-------------------------------------------

bool SIP_Pidf_Xml_Body::encode(char *sip_msg)
{
    if (_element == SIP_PIDF_XML_OPEN)
    {
        strcat(sip_msg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        strcat(sip_msg, "<presence xmlns=\"urn:ietf:params:xml:ns:pidf\" entity=\"pres:");
        strcat(sip_msg, _uri);
        strcat(sip_msg, "\">");
        strcat(sip_msg, "<tuple id=\"sg89ae\">");
        strcat(sip_msg, "<status>");
        strcat(sip_msg, "<basic>open</basic>");
        strcat(sip_msg, "</status>");
        strcat(sip_msg, "</tuple>");
        strcat(sip_msg, "</presence>");

    }else if (_element == SIP_PIDF_XML_CLOSED)
    {
        strcat(sip_msg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        strcat(sip_msg, "<presence xmlns=\"urn:ietf:params:xml:ns:pidf\" entity=\"pres:");
        strcat(sip_msg, _uri);
        strcat(sip_msg, "\">");
        strcat(sip_msg, "<tuple id=\"sg89ae\">");
        strcat(sip_msg, "<status>");
        strcat(sip_msg, "<basic>closed</basic>");
        strcat(sip_msg, "</status>");
        strcat(sip_msg, "</tuple>");
        strcat(sip_msg, "</presence>");
    }
    return true;
}

void SIP_Pidf_Xml_Body::set_uri(const char *uri)
{
    if (_uri)
        delete _uri;
    _uri = create_string(uri);
}

//-------------------------------------------

void SIP_Text_Plain_Body::set_text(const char *text)
{
    if (_text)
        delete _text;
    _text = create_string(text);
}

__END_SYS
