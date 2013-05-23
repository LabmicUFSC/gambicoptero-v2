#include <sip_body.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
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
        case SIP_BODY_APPLICATION_SDP:      body = new SIP_SDP_Body();        break;
        case SIP_BODY_APPLICATION_PIDF_XML: body = new SIP_Pidf_Xml_Body();   break;
        case SIP_BODY_TEXT_PLAIN:           body = new SIP_Text_Plain_Body(); break;
        default: break;
    }

    return body;
}

//-------------------------------------------

bool SIP_SDP_Body::encode(char *sip_msg)
{
    char local[20];
    ((IP::Address &) IP::instance(0)->address()).to_string(local);

    strcat(sip_msg, "v=0\r\n");
    strcat(sip_msg, "o=- 0 0 IN IP4 ");
    strcat(sip_msg, local);
    strcat(sip_msg, "\r\n");
    strcat(sip_msg, "s=-\r\n");
    strcat(sip_msg, "c=IN IP4 ");
    strcat(sip_msg, local);
    strcat(sip_msg, "\r\n");
    strcat(sip_msg, "t=0 0\r\n");
    strcat(sip_msg, "m=audio 8000 RTP/AVP 0\r\n");
    strcat(sip_msg, "a=rtpmap:0 pcmu/8000\r\n");
    strcat(sip_msg, "a=sendonly\r\n");
    return true;
}

//-------------------------------------------

bool SIP_Pidf_Xml_Body::encode(char *sip_msg)
{
    char local[20];
    ((IP::Address &) IP::instance(0)->address()).to_string(local);

    if (_element == SIP_PIDF_XML_OPEN)
    {
        strcat(sip_msg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        strcat(sip_msg, "<presence xmlns=\"urn:ietf:params:xml:ns:pidf\" entity=\"pres:1111@"); //TODO: pres
        strcat(sip_msg, local);
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
        strcat(sip_msg, "<presence xmlns=\"urn:ietf:params:xml:ns:pidf\" entity=\"pres:1111@"); //TODO: pres
        strcat(sip_msg, local);
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

//-------------------------------------------

void SIP_Text_Plain_Body::set_text(const char *text)
{
    if (_text)
        delete _text;
    _text = create_string(text);
}

__END_SYS
