#ifndef __sip_user_agent_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __sip_user_agent_h

#include <sip_defs.h>
#include <sip_header.h>
#include <sip_message.h>
#include <sip_transactions.h>
#include <utility/list.h>

__BEGIN_SYS

class SIP_Dialog
{
private:
    friend class SIP_User_Agent;

    /*enum State
    {
        sttIdle,
        //sttEarly,
        sttConfirmed
    }*/

public:
    SIP_Dialog(SIP_Message_Type type, SIP_Call_Status call_status);
    ~SIP_Dialog();

    void set_dialog(const char *call_id, const char *local_tag, const char *remote_tag,
            unsigned int local_sequence_number, unsigned int remote_sequence_number,
            const char *local_uri, const char *remote_uri, const char *remote_target);
    void set_remote_target(const char *remote_target);
    void add_route_back(SIP_Header_Route *route);
    void add_route_front(SIP_Header_Route *route);
    void clear_routes();

    int get_routes_size() { return (int) _routes.size(); }
    SIP_Header_Route *get_route(int pos);

private:
    SIP_Message_Type _type;
    SIP_Call_Status _call_status;

    //State _state;

    char *_call_id;
    char *_local_tag;
    char *_remote_tag;
    unsigned int _local_sequence_number;
    unsigned int _remote_sequence_number;
    char *_local_uri;
    char *_remote_uri;
    char *_remote_target;
    Simple_List<SIP_Header/*Route*/> _routes;

    Simple_List<SIP_Dialog>::Element _link;
};


class SIP_Subscription
{
    friend class SIP_User_Agent;

public:
    SIP_Subscription() : _event_type(SIP_EVENT_PACKAGE_INVALID), _event_id(0)/*, _timer_handler(0), _timer_alarm(0)*/ {}
    ~SIP_Subscription() { clear(); }

    void set_subscription(SIP_Event_Package event_type, const char *event_id);
    //void start_timer(void *p);
    //void stop_timer();

    bool is_active() { return _event_type != SIP_EVENT_PACKAGE_INVALID; }
    void clear();

private:
    SIP_Event_Package _event_type;
    char *_event_id;

    //Functor_Handler *_timer_handler;
    //Alarm *_timer_alarm;
};


class SIP_User_Agent
{
    friend class SIP_Manager;

public:
    SIP_User_Agent(const char *uri);
    ~SIP_User_Agent();

private:
    SIP_Request *create_request(SIP_Message_Type msg_type, SIP_Dialog *dialog = 0, const char *to = 0, SIP_Message *invite = 0);
    SIP_Response *create_response(unsigned short status_code, SIP_Request *request);

public:
    SIP_Request *create_ack(const char *to, SIP_Request *invite);
    SIP_Request *create_bye(const char *to);
    SIP_Request *create_invite(const char *to);
    SIP_Request *create_message(const char *to, const char *data);
    SIP_Request *create_notify(const char *to, SIP_Subscription_State state, SIP_Pidf_Xml_Basic_Element pidf_xml_element, unsigned int expires = 0);
    SIP_Request *create_subscribe(const char *to) { return 0; }

    void send_request(SIP_Request *request);
    void send_response(SIP_Response *response, SIP_Message_Type request_type, SIP_Transaction *transaction);

    bool receive_response(SIP_Response *response);
    bool receive_response(SIP_Request *request, SIP_Response *response, SIP_Transaction *transaction);
    //bool receive_1xx(SIP_Request *request, SIP_Response *response, SIP_Transaction *transaction, SIP_Dialog *dialog) { return true; }
    bool receive_2xx(SIP_Request *request, SIP_Response *response, SIP_Transaction *transaction, SIP_Dialog *dialog);
    bool receive_3xx_6xx(SIP_Request *request, SIP_Response *response, SIP_Transaction *transaction, SIP_Dialog *dialog);

    bool receive_request(SIP_Request *request);
    bool receive_request(SIP_Request *request, SIP_Transaction *transaction);
    //bool receive_ack(SIP_Request *request, SIP_Transaction *transaction, SIP_Dialog *dialog) { return true; }
    bool receive_bye(SIP_Request *request, SIP_Transaction *transaction, SIP_Dialog *dialog);
    bool receive_invite(SIP_Request *request, SIP_Transaction *transaction, SIP_Dialog *dialog);
    bool receive_message(SIP_Request *request, SIP_Transaction *transaction, SIP_Dialog *dialog);
    //bool receive_notify(SIP_Request *request, SIP_Transaction *transaction, SIP_Dialog *dialog) { return true; }
    bool receive_subscribe(SIP_Request *request, SIP_Transaction *transaction, SIP_Dialog *dialog);

    SIP_Dialog *create_dialog_client(SIP_Request *request, SIP_Response *response);
    SIP_Dialog *create_dialog_server(SIP_Request *request, SIP_Response *response);

public:
    SIP_Dialog *matching_dialog(SIP_Message *msg, SIP_Message_Type type);
    SIP_Dialog *matching_dialog(const char *to, SIP_Message_Type type);
    SIP_Transaction *matching_transaction(SIP_Message *msg);

    const char *get_uri() { return _uri; }
    const char *get_text_received() { return _text_received; }
    bool has_subscription() { return _subscription.is_active(); }
    const char *get_subscriber();
    SIP_Dialog *get_call();
    SIP_Call_Status get_call_status();
    bool connected() { return (get_call_status() != SIP_CALL_STATUS_INACTIVE); }

    void add_transaction(SIP_Transaction *transaction) { _transactions.insert(&transaction->_link); }
    void remove_transaction(SIP_Transaction *transaction) { _transactions.remove(&transaction->_link); delete transaction; }
    SIP_Dialog *add_dialog(SIP_Message_Type type, SIP_Call_Status call_status);
    void remove_dialog(SIP_Dialog *dialog) { _dialogs.remove(&dialog->_link); delete dialog; }

private:
    Simple_List<SIP_Dialog> _dialogs;
    SIP_Subscription _subscription;
    Simple_List<SIP_Transaction> _transactions;

    char *_uri;
    const char *_text_received;

    Simple_List<SIP_User_Agent>::Element _link;
};

__END_SYS

#endif
