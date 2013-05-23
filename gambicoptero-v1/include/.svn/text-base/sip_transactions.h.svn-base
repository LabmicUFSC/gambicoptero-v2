#ifndef __sip_transactions_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __sip_transactions_h

#include <sip_defs.h>
#include <sip_manager.h>
#include <sip_message.h>

__BEGIN_SYS

class SIP_Transaction
{
    friend class SIP_User_Agent;

public:
    SIP_Transaction(SIP_User_Agent *ua);
    virtual ~SIP_Transaction();

    virtual SIP_Transaction_Type get_transaction_type() = 0;

    SIP_Transaction *matching_transaction(SIP_Message *msg);
    SIP_Transaction *matching_transaction_client(SIP_Message *msg);
    SIP_Transaction *matching_transaction_server(SIP_Message *msg);
    void clear();

    unsigned long get_timer_value(SIP_Timer timer) { return _timer_values[timer]; }
    void set_timer_value(SIP_Timer timer, unsigned long timer_value) { _timer_values[timer] = timer_value; }
    void start_timer(SIP_Timer timer, SIP_Transaction *p);
    void stop_timer(SIP_Timer timer);

protected:
    SIP_Manager *_manager;
    SIP_User_Agent *_ua;
    SIP_Request *_request_msg;

    unsigned long _timer_values[SIP_TIMER_COUNT];
    Functor_Handler<SIP_Transaction> *_timer_handlers[SIP_TIMER_COUNT];
    Alarm *_timer_alarms[SIP_TIMER_COUNT];

    Simple_List<SIP_Transaction>::Element _link;
};


class SIP_Transaction_Client_Invite : public SIP_Transaction
{
private:
    enum State
    {
        sttIdle,
        sttCalling,
        sttProceeding,
        sttCompleted,
        //sttTerminated
    };

public:
    SIP_Transaction_Client_Invite(SIP_User_Agent *ua) : SIP_Transaction(ua), _state(sttIdle) {}
    ~SIP_Transaction_Client_Invite();

    void send_ack(SIP_Response *msg);

    void send_invite(SIP_Request *msg);
    void receive_1xx(SIP_Response *msg);
    void receive_2xx(SIP_Response *msg);
    void receive_3xx_6xx(SIP_Response *msg);

    static void timer_A_Callback(SIP_Transaction *p);
    static void timer_B_Callback(SIP_Transaction *p);
    static void timer_D_Callback(SIP_Transaction *p);

    SIP_Transaction_Type get_transaction_type() { return SIP_TRANSACTION_CLIENT_INVITE; }

private:
    State _state;
};


class SIP_Transaction_Client_Non_Invite : public SIP_Transaction
{
private:
    enum State
    {
        sttIdle,
        sttTrying,
        sttProceeding,
        sttCompleted,
        //sttTerminated
    };

public:
    SIP_Transaction_Client_Non_Invite(SIP_User_Agent *ua) : SIP_Transaction(ua), _state(sttIdle) {}
    ~SIP_Transaction_Client_Non_Invite();

    void send_request(SIP_Request *msg);
    void receive_1xx(SIP_Response *msg);
    void receive_2xx_6xx(SIP_Response *msg);

    static void timer_E_Callback(SIP_Transaction *p);
    static void timer_F_Callback(SIP_Transaction *p);
    static void timer_K_Callback(SIP_Transaction *p);

    SIP_Transaction_Type get_transaction_type() { return SIP_TRANSACTION_CLIENT_NON_INVITE; }

private:
    State _state;
};


class SIP_Transaction_Server_Invite : public SIP_Transaction
{
private:
    enum State
    {
        sttIdle,
        sttProceeding,
        sttCompleted,
        sttConfirmed,
        //sttTerminated
    };

public:
    SIP_Transaction_Server_Invite(SIP_User_Agent *ua) : SIP_Transaction(ua), _state(sttIdle), _last_response(0) {}
    ~SIP_Transaction_Server_Invite();

    void receive_invite(SIP_Request *msg);
    void receive_ack(SIP_Request *msg);
    void send_1xx(SIP_Response *msg);
    void send_2xx(SIP_Response *msg);
    void send_3xx_6xx(SIP_Response *msg);

    static void timer_G_Callback(SIP_Transaction *p);
    static void timer_H_Callback(SIP_Transaction *p);
    static void timer_I_Callback(SIP_Transaction *p);

    SIP_Transaction_Type get_transaction_type() { return SIP_TRANSACTION_SERVER_INVITE; }

private:
    State _state;
    SIP_Response *_last_response;
};


class SIP_Transaction_Server_Non_Invite : public SIP_Transaction
{
private:
    enum State
    {
        sttIdle,
        sttTrying,
        sttProceeding,
        sttCompleted,
        //sttTerminated
    };

public:
    SIP_Transaction_Server_Non_Invite(SIP_User_Agent *ua) : SIP_Transaction(ua), _state(sttIdle), _last_response(0) {}
    ~SIP_Transaction_Server_Non_Invite();

    void receive_request(SIP_Request *msg);
    void send_1xx(SIP_Response *msg);
    void send_2xx_6xx(SIP_Response *msg);

    static void timer_J_Callback(SIP_Transaction *p);

    SIP_Transaction_Type get_transaction_type() { return SIP_TRANSACTION_SERVER_NON_INVITE; }

private:
    State _state;
    SIP_Response *_last_response;
};

__END_SYS

#endif
