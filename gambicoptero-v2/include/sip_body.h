#ifndef __sip_body_h
#define __sip_body_h

#include <sip_defs.h>

__BEGIN_SYS

class SIP_Body
{
public:
    virtual ~SIP_Body() {}

    static SIP_Body *decode_body(const char *sip_msg, SIP_Body_Type body_type);
    static SIP_Body *create_body(SIP_Body_Type type);

    virtual SIP_Body_Type get_body_type() = 0;

    virtual bool parse(const char *sip_msg) = 0;
    virtual bool encode(char *sip_msg) = 0;
};


class SIP_SDP_Body : public SIP_Body
{
public:
    SIP_SDP_Body() : _address(0), _port(0) {}
    ~SIP_SDP_Body() { if (_address) delete _address; }

    SIP_Body_Type get_body_type() { return SIP_BODY_APPLICATION_SDP; }

    bool parse(const char *sip_msg);
    bool encode(char *sip_msg);

    const char *get_address() { return _address; }
    unsigned short get_port() { return _port; }
    void set_address(const char *address);
    void set_port(unsigned short port) { _port = port; }

private:
    char *_address;
    unsigned short _port;
};


class SIP_Pidf_Xml_Body : public SIP_Body
{
public:
    SIP_Pidf_Xml_Body() : _uri(0), _element(SIP_PIDF_XML_BASIC_ELEMENT_INVALID) {}
    ~SIP_Pidf_Xml_Body() { if (_uri) delete _uri; }

    SIP_Body_Type get_body_type() { return SIP_BODY_APPLICATION_PIDF_XML; }

    bool parse(const char *sip_msg) { return true; }
    bool encode(char *sip_msg);

    void set_uri(const char *uri);
    void set_element(SIP_Pidf_Xml_Basic_Element element) { _element = element; }

private:
    char *_uri;
    SIP_Pidf_Xml_Basic_Element _element;
};


class SIP_Text_Plain_Body : public SIP_Body
{
public:
    SIP_Text_Plain_Body() : _text(0) {}
    ~SIP_Text_Plain_Body() { if (_text) delete _text; }

    SIP_Body_Type get_body_type() { return SIP_BODY_TEXT_PLAIN; }

    bool parse(const char *sipMsg) { _text = create_string(sipMsg); return true; }
    bool encode(char *sip_msg) { strcat(sip_msg, _text); return true; }

    void set_text(const char *text);
    const char *get_text() { return _text; }

private:
    char *_text;
};

__END_SYS

#endif
