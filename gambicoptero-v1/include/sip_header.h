#ifndef __sip_header_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __sip_header_h

#include <sip_defs.h>
#include <utility/list.h>

__BEGIN_SYS

class SIP_Header
{
    friend class SIP_Message;
    friend class SIP_Dialog;

public:
    SIP_Header() : _link(this) {}
    virtual ~SIP_Header() {}

    static void decode_header(char *line, Simple_List<SIP_Header> &ret);
    static SIP_Header *create_header(SIP_Header_Type type);

    virtual SIP_Header_Type get_header_type() = 0;

    virtual bool parse(char *sip_msg) = 0;
    virtual bool encode(char *sip_msg) = 0;

private:
    Simple_List<SIP_Header>::Element _link;
};


class Value_String : public SIP_Header
{
public:
    Value_String() : _string(0) {}
    Value_String(Value_String &value) { _string = create_string(value._string); }
    ~Value_String() { if (_string) delete _string; }

    virtual bool parse(char *sip_msg);
    bool encode(char *sip_msg);

    const char *get_string() { return _string; }
    void set_string(const char *string);

protected:
    char *_string;
};


class Value_Number : public SIP_Header
{
public:
    Value_Number() : _number(0) {}
    Value_Number(Value_Number &value) : _number(value._number) {}
    ~Value_Number() {}

    bool parse(char *sip_msg);
    bool encode(char *sip_msg);

    int get_number() { return _number; }
    void set_number(int number) { _number = number; }

protected:
    int _number;
};


class Value_Address_Params : public SIP_Header
{
public:
    Value_Address_Params() : _display(0), _address(0), _tag(0), _lr(false) {}
    Value_Address_Params(Value_Address_Params &value);
    ~Value_Address_Params();

    bool parse(char *sip_msg);
    bool encode(char *sip_msg);

    const char *get_address() { return _address; }
    const char *get_tag() { return _tag; }
    bool is_lr() { return _lr; }

    void set_address(const char *address, bool lr = false);
    void set_tag(const char *tag);

private:
    char *_display;
    char *_address;
    char *_tag;
    bool _lr;
};


class Value_Options : public SIP_Header
{
public:
    Value_Options() {};
    Value_Options(Value_Options &value);
    ~Value_Options();

    bool parse(char *sip_msg);
    bool encode(char *sip_msg);

    int get_option_size() { return (int) _options.size(); }
    const char *get_option(int pos);
    void add_option(const char *option);

private:
    Simple_List<char> _options;
};


class SIP_Header_Allow : public Value_Options
{
public:
    //TODO: Allow pode ser vazio!
    SIP_Header_Type get_header_type() { return SIP_HEADER_ALLOW; }

    void add_allowed(SIP_Message_Type allowed);
};


class SIP_Header_Allow_Events : public Value_Options
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_ALLOW_EVENTS; }
};


class SIP_Header_Call_ID : public Value_String
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_CALLID; }
};


class SIP_Header_Contact : public Value_Address_Params
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_CONTACT; }
};


class SIP_Header_Content_Disposition : public Value_String
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_CONTENT_DISPOSITION; }

    bool parse(char *sip_msg);
};


class SIP_Header_Content_Length : public Value_Number
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_CONTENT_LENGTH; }
};


class SIP_Header_Content_Type : public SIP_Header
{
public:
    SIP_Header_Content_Type() : _type(SIP_BODY_TYPE_INVALID) {}
    SIP_Header_Content_Type(SIP_Header_Content_Type &header) { _type = header._type; }
    ~SIP_Header_Content_Type() {}

    SIP_Header_Type get_header_type() { return SIP_HEADER_CONTENT_TYPE; }

    bool parse(char *sip_msg);
    bool encode(char *sip_msg);

    void set_content_type(SIP_Body_Type type) { _type = type; }
    SIP_Body_Type get_content_type() { return _type; }

private:
    SIP_Body_Type _type;
};


class SIP_Header_CSeq : public SIP_Header
{
public:
    SIP_Header_CSeq() : _sequence(0), _method(SIP_MESSAGE_TYPE_INVALID) {}
    SIP_Header_CSeq(SIP_Header_CSeq &header) { _sequence = header._sequence; _method = header._method; }
    ~SIP_Header_CSeq() {}

    SIP_Header_Type get_header_type() { return SIP_HEADER_CSEQ; }

    bool parse(char *sip_msg);
    bool encode(char *sip_msg);

    unsigned int get_sequence() { return _sequence; }
    SIP_Message_Type get_method() { return _method; }
    void set_cseq(SIP_Message_Type method, int sequence) { _method = method; _sequence = sequence; }

private:
    unsigned int _sequence;
    SIP_Message_Type _method;
};


class SIP_Header_Event : public SIP_Header
{
public:
    SIP_Header_Event() : _type(SIP_EVENT_PACKAGE_INVALID), _id(0) {};
    SIP_Header_Event(SIP_Header_Event &header) { _type = header._type; _id = create_string(header._id); }
    ~SIP_Header_Event() { if (_id) delete _id; }

    SIP_Header_Type get_header_type() { return SIP_HEADER_EVENT; }

    bool parse(char *sip_msg);
    bool encode(char *sip_msg);

    SIP_Event_Package get_type() { return _type; }
    const char *get_id() { return _id; }
    void set_event(SIP_Event_Package type, const char *id);

private:
    SIP_Event_Package _type;
    char *_id;
};


class SIP_Header_Expires : public Value_Number
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_EXPIRES; }
};


class SIP_Header_From : public Value_Address_Params
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_FROM; }
};


class SIP_Header_Max_Forwards : public Value_Number
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_MAX_FORWARDS; }
};


class SIP_Header_Record_Route : public Value_Address_Params
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_RECORD_ROUTE; }
};


class SIP_Header_Require : public Value_Options
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_REQUIRE; }
};


class SIP_Header_Route : public Value_Address_Params
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_ROUTE; }
};


class SIP_Header_Subscription_State : public SIP_Header
{
public:
    SIP_Header_Subscription_State() : _state(SIP_SUBSCRIPTION_STATE_INVALID), _expires(-1) {}
    SIP_Header_Subscription_State(SIP_Header_Subscription_State &header) { _state = header._state; _expires = header._expires; }
    ~SIP_Header_Subscription_State() {}

    SIP_Header_Type get_header_type() { return SIP_HEADER_SUBSCRIPTION_STATE; }

    bool parse(char *sip_msg);
    bool encode(char *sip_msg);

    //SIP_Subscription_State get_state() { return _state; }
    //int get_expires() { return _expires; }
    void set_subscription_state(SIP_Subscription_State state, int expires = -1) { _state = state; _expires = expires; }

private:
    SIP_Subscription_State _state;
    int _expires;
};


class SIP_Header_To : public Value_Address_Params
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_TO; }
};


class SIP_Header_Unsupported : public Value_Options
{
public:
    SIP_Header_Type get_header_type() { return SIP_HEADER_UNSUPPORTED; }
};


class SIP_Header_Via : public SIP_Header
{
public:
    SIP_Header_Via() : _protocol_name(0), _protocol_version(0), _transport(SIP_TRANSPORT_TYPE_INVALID), _sent_by(0), _port(0), _branch(0), _received(0) {}
    SIP_Header_Via(SIP_Header_Via &header);
    ~SIP_Header_Via();

    SIP_Header_Type get_header_type() { return SIP_HEADER_VIA; }

    bool parse(char *sip_msg);
    bool encode(char *sip_msg);

    //SIP_Transport_Type get_transport() { return _transport; }
    const char *get_sent_by() { return _sent_by; }
    unsigned short get_port() { return _port; }
    const char *get_branch() { return _branch; }
    const char *get_received() { return _received; }
    void set_via(const char *protocolName, const char *protocolVersion, const SIP_Transport_Type transport,
            const char *sentBy, unsigned short port, const char *branch);
    void set_received(const char *received);

private:
    char *_protocol_name;
    char *_protocol_version;
    SIP_Transport_Type _transport;
    char *_sent_by;
    unsigned short _port;
    char *_branch;
    char *_received;
};

__END_SYS

#endif
