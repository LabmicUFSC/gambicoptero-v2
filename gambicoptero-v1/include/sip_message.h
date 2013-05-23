#ifndef __sip_message_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __sip_message_h

#include <sip_defs.h>
#include <sip_header.h>
#include <sip_body.h>
#include <utility/list.h>
#include <utility/malloc.h>

__BEGIN_SYS

class SIP_Manager;

class SIP_Message
{
public:
    SIP_Message() : _body(0), _can_delete(true), _link(this) {}
    virtual ~SIP_Message();

    static SIP_Message *decode_msg(const char *sip_msg);

    static SIP_Message_Type get_msg_type(const char *sip_msg);
    static const char *get_msg_type(SIP_Message_Type type);
    virtual SIP_Message_Type get_msg_type() = 0;

    bool parse(const char *sip_msg);
    virtual bool parse_start_line(const char *sip_msg) = 0;
    bool parse_header(const char *sip_msg);
    bool parse_body(const char *sip_msg);

    bool encode(char *sip_msg);
    virtual bool encode_start_line(char *sip_msg) = 0;
    bool encode_header(char *sip_msg, char *body_msg);
    bool encode_body(char *sip_msg);

    void add_header(SIP_Header *header);
    void add_headers(Simple_List<SIP_Header> &headers);
    SIP_Header *get_header(unsigned short type, unsigned short pos = 0);
    int get_num_header(unsigned short type);

    void set_body(SIP_Body *body) { _body = body; }
    SIP_Body *get_body() { return _body; }

    static SIP_Transport_Type get_transport_type(const char *type);
    static const char *get_transport_type(const SIP_Transport_Type type);

    void set_can_delete(bool can_delete) { _can_delete = can_delete; }
    bool get_can_delete() { return _can_delete; }

private:
    Simple_List<SIP_Header> _headers;
    SIP_Body *_body;
    bool _can_delete;

    Simple_List<SIP_Message>::Element _link;
};

class SIP_Request : public SIP_Message
{
public:
    SIP_Request(SIP_Message_Type type) : _method(type), _request_uri(0), _sip_version(0) {}
    virtual ~SIP_Request();

    bool parse_start_line(const char *sip_msg);
    bool encode_start_line(char *sip_msg);

    SIP_Message_Type get_msg_type() { return _method; }
    const char *get_request_uri() const { return _request_uri; }

    void set_request_line(const SIP_Message_Type msg_type, const char *request_uri, const char *sip_version);

private:
    SIP_Message_Type _method;
    char *_request_uri;
    char *_sip_version;
};

class SIP_Response : public SIP_Message
{
public:
    SIP_Response() : _sip_version(0), _status_code(0), _reason_phrase(0) {}
    SIP_Response(unsigned short status_code);
    ~SIP_Response();

    bool parse_start_line(const char *sip_msg);
    bool encode_start_line(char *sip_msg);

    SIP_Message_Type get_msg_type() { return SIP_RESPONSE; }
    unsigned short get_status_code() { return _status_code; }

    void set_status_line(const char *sip_version, unsigned short status_code, const char *reason_phrase);

private:
    char *_sip_version;
    unsigned short _status_code;
    char *_reason_phrase;

    static SIP_Status_Code _status_codes[];
};

__END_SYS

#endif
