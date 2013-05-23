#ifndef __sip_body_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
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
    SIP_Body_Type get_body_type() { return SIP_BODY_APPLICATION_SDP; }

    bool parse(const char *sip_msg) { return true; }
    bool encode(char *sip_msg);
};


class SIP_Pidf_Xml_Body : public SIP_Body
{
public:
    SIP_Pidf_Xml_Body() : _element(SIP_PIDF_XML_BASIC_ELEMENT_INVALID) {}

    SIP_Body_Type get_body_type() { return SIP_BODY_APPLICATION_PIDF_XML; }

    bool parse(const char *sip_msg) { return true; }
    bool encode(char *sip_msg);

    void set_element(SIP_Pidf_Xml_Basic_Element element) { _element = element; }

private:
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
