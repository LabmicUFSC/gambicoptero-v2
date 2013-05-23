#include <sip_transactions.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <sip_user_agent.h>

__BEGIN_SYS

SIP_Transaction::SIP_Transaction(SIP_User_Agent *ua) : _ua(ua), _request_msg(0), _link(this)
{
    _manager = SIP_Manager::get_instance();

    for (int i = 0; i < SIP_TIMER_COUNT; i++)
    {
        _timer_values[i] = 0;
        _timer_handlers[i] = 0;
        _timer_alarms[i] = 0;
    }
}

SIP_Transaction::~SIP_Transaction()
{
    for (int i = 0; i < SIP_TIMER_COUNT; i++)
    {
        if (_timer_handlers[i])
            delete _timer_handlers[i];
        if (_timer_alarms[i])
            delete _timer_alarms[i];
    }
}

void SIP_Transaction::clear()
{
    if (_request_msg)
        delete _request_msg;

    _ua->remove_transaction(this);
}

SIP_Transaction *SIP_Transaction::matching_transaction(SIP_Message *msg)
{
    if ((get_transaction_type() == SIP_TRANSACTION_CLIENT_INVITE) || (get_transaction_type() == SIP_TRANSACTION_CLIENT_NON_INVITE))
    {
        if (msg->get_msg_type() != SIP_RESPONSE)
            return 0;
        return matching_transaction_client(msg);
    }

    if ((get_transaction_type() == SIP_TRANSACTION_SERVER_INVITE) && ((msg->get_msg_type() != SIP_REQUEST_INVITE) &&
        (msg->get_msg_type() != SIP_REQUEST_ACK)))
    {
        return 0;
    }

    if ((get_transaction_type() == SIP_TRANSACTION_SERVER_NON_INVITE) && ((msg->get_msg_type() == SIP_RESPONSE) ||
        (msg->get_msg_type() == SIP_REQUEST_INVITE) || (msg->get_msg_type() == SIP_REQUEST_ACK)))
    {
        return 0;
    }

    return matching_transaction_server(msg);
}

SIP_Transaction *SIP_Transaction::matching_transaction_client(SIP_Message *msg)
{
    SIP_Response *response = (SIP_Response *) msg;
    SIP_Header_Via *via_response = (SIP_Header_Via *) response->get_header(SIP_HEADER_VIA);
    SIP_Header_CSeq *cseq_response = (SIP_Header_CSeq *) response->get_header(SIP_HEADER_CSEQ);

    const char *branch_response = via_response->get_branch();
    SIP_Message_Type msg_type_response = cseq_response->get_method();
    //unsigned int sequence_response = cseq_response->get_sequence();

    if (!branch_response)
        return 0;

    SIP_Header_Via *via = (SIP_Header_Via *) _request_msg->get_header(SIP_HEADER_VIA);
    SIP_Header_CSeq *cseq = (SIP_Header_CSeq *) _request_msg->get_header(SIP_HEADER_CSEQ);

    const char *branch = via->get_branch();
    SIP_Message_Type msg_type = cseq->get_method();
    //unsigned int sequence = cseq->get_sequence();

    if ((!strcmp(branch, branch_response)) && (msg_type == msg_type_response)) // && (sequence == sequence_response))
        return this;

    return 0;
}

SIP_Transaction *SIP_Transaction::matching_transaction_server(SIP_Message *msg)
{
    SIP_Request *request = (SIP_Request *) msg;
    SIP_Header_Via *via_request = (SIP_Header_Via *) request->get_header(SIP_HEADER_VIA);

    SIP_Message_Type method_request = request->get_msg_type();
    const char *branch_request = via_request->get_branch();
    const char *sent_by_request = via_request->get_sent_by();

    if ((!sent_by_request) || (method_request == SIP_MESSAGE_TYPE_INVALID))
        return 0;

    bool magic_cookie = false;
    if ((branch_request) && (start_with(branch_request, "z9hG4bK")))
        magic_cookie = true;

    if (!magic_cookie)
        return 0;

    SIP_Header_Via *via = (SIP_Header_Via *) _request_msg->get_header(SIP_HEADER_VIA);
    SIP_Message_Type method = _request_msg->get_msg_type();
    const char *branch = via->get_branch();
    const char *sent_by = via->get_sent_by();

    if ((!branch) || (!sent_by) || (method == SIP_MESSAGE_TYPE_INVALID))
        return 0;

    if ((!strcmp(branch, branch_request)) && (!strcmp(sent_by, sent_by_request)) &&
        ((method == method_request) || ((method_request == SIP_REQUEST_ACK) && (method == SIP_REQUEST_INVITE))))
    {
        return this;
    }

    return 0;
}

void SIP_Transaction::start_timer(SIP_Timer timer, SIP_Transaction *p)
{
    switch (timer)
    {
        case SIP_TIMER_A: _timer_handlers[SIP_TIMER_A] = new Functor_Handler<SIP_Transaction>(&SIP_Transaction_Client_Invite::timer_A_Callback, p);     break;
        case SIP_TIMER_B: _timer_handlers[SIP_TIMER_B] = new Functor_Handler<SIP_Transaction>(&SIP_Transaction_Client_Invite::timer_B_Callback, p);     break;
        //case SIP_TIMER_C: _timer_handlers[SIP_TIMER_C] = new Functor_Handler<SIP_Transaction>(&timer_C_Callback);                                     break;
        case SIP_TIMER_D: _timer_handlers[SIP_TIMER_D] = new Functor_Handler<SIP_Transaction>(&SIP_Transaction_Client_Invite::timer_D_Callback, p);     break;
        case SIP_TIMER_E: _timer_handlers[SIP_TIMER_E] = new Functor_Handler<SIP_Transaction>(&SIP_Transaction_Client_Non_Invite::timer_E_Callback, p); break;
        case SIP_TIMER_F: _timer_handlers[SIP_TIMER_F] = new Functor_Handler<SIP_Transaction>(&SIP_Transaction_Client_Non_Invite::timer_F_Callback, p); break;
        case SIP_TIMER_G: _timer_handlers[SIP_TIMER_G] = new Functor_Handler<SIP_Transaction>(&SIP_Transaction_Server_Invite::timer_G_Callback, p);     break;
        case SIP_TIMER_H: _timer_handlers[SIP_TIMER_H] = new Functor_Handler<SIP_Transaction>(&SIP_Transaction_Server_Invite::timer_H_Callback, p);     break;
        case SIP_TIMER_I: _timer_handlers[SIP_TIMER_I] = new Functor_Handler<SIP_Transaction>(&SIP_Transaction_Server_Invite::timer_I_Callback, p);     break;
        case SIP_TIMER_J: _timer_handlers[SIP_TIMER_J] = new Functor_Handler<SIP_Transaction>(&SIP_Transaction_Server_Non_Invite::timer_J_Callback, p); break;
        case SIP_TIMER_K: _timer_handlers[SIP_TIMER_K] = new Functor_Handler<SIP_Transaction>(&SIP_Transaction_Client_Non_Invite::timer_K_Callback, p); break;
        default: return;
    }

    _timer_alarms[timer] = new Alarm(_timer_values[timer], _timer_handlers[timer], 1);
}

void SIP_Transaction::stop_timer(SIP_Timer timer)
{
    if (_timer_handlers[timer])
    {
        delete _timer_handlers[timer];
        _timer_handlers[timer] = 0;
    }

    if (_timer_alarms[timer])
    {
        delete _timer_alarms[timer];
        _timer_alarms[timer] = 0;
    }
}

//-------------------------------------------

SIP_Transaction_Client_Invite::~SIP_Transaction_Client_Invite()
{
    stop_timer(SIP_TIMER_A);
    stop_timer(SIP_TIMER_B);
    stop_timer(SIP_TIMER_D);
}

void SIP_Transaction_Client_Invite::send_ack(SIP_Response *msg)
{
    const char *request_uri = _request_msg->get_request_uri();
    SIP_Header_Via *via = (SIP_Header_Via *) _request_msg->get_header(SIP_HEADER_VIA);
    SIP_Header_From *from = (SIP_Header_From *) _request_msg->get_header(SIP_HEADER_FROM);
    SIP_Header_Call_ID *call_id = (SIP_Header_Call_ID *) _request_msg->get_header(SIP_HEADER_CALLID);
    SIP_Header_CSeq *cseq = (SIP_Header_CSeq *) _request_msg->get_header(SIP_HEADER_CSEQ);
    SIP_Header_To *to = (SIP_Header_To *) msg->get_header(SIP_HEADER_TO);

    SIP_Request ack(SIP_REQUEST_ACK);
    ack.set_request_line(SIP_REQUEST_ACK, request_uri, SIP_VERSION);
    ack.add_header(new SIP_Header_Via(*via));
    ack.add_header(new SIP_Header_To(*to));
    ack.add_header(new SIP_Header_From(*from));
    ack.add_header(new SIP_Header_Call_ID(*call_id));

    SIP_Header_Max_Forwards *header_max_forwards = new SIP_Header_Max_Forwards();
    header_max_forwards->set_number(70);
    ack.add_header(header_max_forwards);

    SIP_Header_CSeq *header_cseq = new SIP_Header_CSeq();
    header_cseq->set_cseq(SIP_REQUEST_ACK, cseq->get_sequence());
    ack.add_header(header_cseq);

    int route_num = _request_msg->get_num_header(SIP_HEADER_ROUTE);
    for (int i = 0; i < route_num; i++)
    {
        SIP_Header_Route *header = (SIP_Header_Route *) _request_msg->get_header(SIP_HEADER_ROUTE, i);
        ack.add_header(new SIP_Header_Route(*header));
    }

    _manager->send_message(&ack);
}

void SIP_Transaction_Client_Invite::send_invite(SIP_Request *msg)
{
    switch (_state)
    {
        case sttIdle:
            _state = sttCalling;
            _request_msg = msg;
            msg->set_can_delete(false);
            _manager->send_message(_request_msg);
            set_timer_value(SIP_TIMER_A, SIP_TIMER_1);
            start_timer(SIP_TIMER_A, this);
            set_timer_value(SIP_TIMER_B, SIP_TIMER_1 * 64);
            start_timer(SIP_TIMER_B, this);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Client_Invite::receive_1xx(SIP_Response *msg)
{
    switch (_state)
    {
        case sttCalling:
            _state = sttProceeding;
            stop_timer(SIP_TIMER_A);
            stop_timer(SIP_TIMER_B);
            _ua->receive_response(_request_msg, msg, this);
            break;

        case sttProceeding:
            //_state = sttProceeding;
            _ua->receive_response(_request_msg, msg, this);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Client_Invite::receive_2xx(SIP_Response *msg)
{
    switch (_state)
    {
        case sttCalling:
            //_state = sttTerminated;
            stop_timer(SIP_TIMER_A);
            stop_timer(SIP_TIMER_B);
            _ua->receive_response(_request_msg, msg, this);
            clear();
            break;

        case sttProceeding:
            //_state = sttTerminated;
            _ua->receive_response(_request_msg, msg, this);
            clear();
            break;

        default:
            break;
    }
}

void SIP_Transaction_Client_Invite::receive_3xx_6xx(SIP_Response *msg)
{
    switch (_state)
    {
        case sttCalling:
            _state = sttCompleted;
            send_ack(msg);
            set_timer_value(SIP_TIMER_D, SIP_TIMER_32s);
            start_timer(SIP_TIMER_D, this);
            stop_timer(SIP_TIMER_A);
            stop_timer(SIP_TIMER_B);
            _ua->receive_response(_request_msg, msg, this);
            break;

        case sttProceeding:
            _state = sttCompleted;
            send_ack(msg);
            set_timer_value(SIP_TIMER_D, SIP_TIMER_32s);
            start_timer(SIP_TIMER_D, this);
            _ua->receive_response(_request_msg, msg, this);
            break;

        case sttCompleted:
            //_state = sttCompleted;
            send_ack(msg);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Client_Invite::timer_A_Callback(SIP_Transaction *p)
{
    SIP_Transaction_Client_Invite *transaction = (SIP_Transaction_Client_Invite *) p;

    switch (transaction->_state)
    {
        case sttCalling:
            //transaction->_state = sttCalling;
            transaction->set_timer_value(SIP_TIMER_A, transaction->get_timer_value(SIP_TIMER_A) * 2);
            transaction->start_timer(SIP_TIMER_A, transaction);
            transaction->_manager->send_message(transaction->_request_msg);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Client_Invite::timer_B_Callback(SIP_Transaction *p)
{
    SIP_Transaction_Client_Invite *transaction = (SIP_Transaction_Client_Invite *) p;

    switch (transaction->_state)
    {
        case sttCalling:
        {
            //transaction->_state = sttTerminated;
            transaction->stop_timer(SIP_TIMER_A);

            SIP_Response response(408);
            transaction->_ua->receive_response(transaction->_request_msg, &response, transaction);
            transaction->clear();
            break;
        }

        default:
            break;
    }
}

void SIP_Transaction_Client_Invite::timer_D_Callback(SIP_Transaction *p)
{
    SIP_Transaction_Client_Invite *transaction = (SIP_Transaction_Client_Invite *) p;

    switch (transaction->_state)
    {
        case sttCompleted:
            //transaction->_state = sttTerminated;
            transaction->clear();
            break;

        default:
            break;
    }
}

//-------------------------------------------

SIP_Transaction_Client_Non_Invite::~SIP_Transaction_Client_Non_Invite()
{
    stop_timer(SIP_TIMER_E);
    stop_timer(SIP_TIMER_F);
    stop_timer(SIP_TIMER_K);
}

void SIP_Transaction_Client_Non_Invite::send_request(SIP_Request *msg)
{
    switch (_state)
    {
        case sttIdle:
            _state = sttTrying;
            _request_msg = msg;
            msg->set_can_delete(false);
            _manager->send_message(_request_msg);
            set_timer_value(SIP_TIMER_E, SIP_TIMER_1);
            start_timer(SIP_TIMER_E, this);
            set_timer_value(SIP_TIMER_F, SIP_TIMER_1 * 64);
            start_timer(SIP_TIMER_F, this);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Client_Non_Invite::receive_1xx(SIP_Response *msg)
{
    switch (_state)
    {
        case sttTrying:
        case sttProceeding:
            _state = sttProceeding;
            _ua->receive_response(_request_msg, msg, this);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Client_Non_Invite::receive_2xx_6xx(SIP_Response *msg)
{
    switch (_state)
    {
        case sttTrying:
        case sttProceeding:
            _state = sttCompleted;
            set_timer_value(SIP_TIMER_K, SIP_TIMER_4);
            start_timer(SIP_TIMER_K, this);
            stop_timer(SIP_TIMER_E);
            stop_timer(SIP_TIMER_F);
            _ua->receive_response(_request_msg, msg, this);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Client_Non_Invite::timer_E_Callback(SIP_Transaction *p)
{
    SIP_Transaction_Client_Non_Invite *transaction = (SIP_Transaction_Client_Non_Invite *) p;

    switch (transaction->_state)
    {
        case sttTrying:
        {
            //_state = sttTrying;
            unsigned long value = ((transaction->get_timer_value(SIP_TIMER_E) * 2) < SIP_TIMER_2) ? transaction->get_timer_value(SIP_TIMER_E) * 2 : SIP_TIMER_2;
            transaction->set_timer_value(SIP_TIMER_E, value);
            transaction->start_timer(SIP_TIMER_E, transaction);
            transaction->_manager->send_message(transaction->_request_msg);
            break;
        }

        case sttProceeding:
            //_state = sttProceeding;
            transaction->set_timer_value(SIP_TIMER_E, SIP_TIMER_2);
            transaction->start_timer(SIP_TIMER_E, transaction);
            transaction->_manager->send_message(transaction->_request_msg);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Client_Non_Invite::timer_F_Callback(SIP_Transaction *p)
{
    SIP_Transaction_Client_Non_Invite *transaction = (SIP_Transaction_Client_Non_Invite *) p;

    switch (transaction->_state)
    {
        case sttTrying:
        case sttProceeding:
        {
            //transaction->_state = sttTerminated;
            transaction->stop_timer(SIP_TIMER_E);

            SIP_Response response(408);
            transaction->_ua->receive_response(transaction->_request_msg, &response, transaction);
            transaction->clear();
            break;
        }

        default:
            break;
    }
}

void SIP_Transaction_Client_Non_Invite::timer_K_Callback(SIP_Transaction *p)
{
    SIP_Transaction_Client_Non_Invite *transaction = (SIP_Transaction_Client_Non_Invite *) p;

    switch (transaction->_state)
    {
        case sttCompleted:
            //transaction->_state = sttTerminated;
            transaction->clear();
            break;

        default:
            break;
    }
}

//-------------------------------------------

SIP_Transaction_Server_Invite::~SIP_Transaction_Server_Invite()
{
    stop_timer(SIP_TIMER_G);
    stop_timer(SIP_TIMER_H);
    stop_timer(SIP_TIMER_I);

    if (_last_response)
        delete _last_response;
}

void SIP_Transaction_Server_Invite::receive_invite(SIP_Request *msg)
{
    switch (_state)
    {
        case sttIdle:
            _state = sttProceeding;
            _request_msg = msg;
            msg->set_can_delete(false);
            _ua->receive_request(msg, this);
            break;

        case sttProceeding:
            //_state = sttProceeding;
            if (_last_response)
                _manager->send_message(_last_response);
            break;

        case sttCompleted:
            //_state = sttCompleted;
            if (_last_response)
                _manager->send_message(_last_response);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Server_Invite::receive_ack(SIP_Request *msg)
{
    switch (_state)
    {
        case sttCompleted:
            _state = sttConfirmed;
            set_timer_value(SIP_TIMER_I, SIP_TIMER_4);
            start_timer(SIP_TIMER_I, this);
            stop_timer(SIP_TIMER_G);
            stop_timer(SIP_TIMER_H);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Server_Invite::send_1xx(SIP_Response *msg)
{
    switch (_state)
    {
        case sttProceeding:
            //_state = sttProceeding;
            if (_last_response)
                delete _last_response;
            _last_response = msg;
            msg->set_can_delete(false);
            _manager->send_message(_last_response);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Server_Invite::send_2xx(SIP_Response *msg)
{
    switch (_state)
    {
        case sttProceeding:
            //_state = sttTerminated;
            _manager->send_message(msg);
            clear();
            break;

        default:
            break;
    }
}

void SIP_Transaction_Server_Invite::send_3xx_6xx(SIP_Response *msg)
{
    switch (_state)
    {
        case sttProceeding:
            _state = sttCompleted;
            if (_last_response)
                delete _last_response;
            _last_response = msg;
            msg->set_can_delete(false);
            _manager->send_message(_last_response);
            set_timer_value(SIP_TIMER_G, SIP_TIMER_1);
            start_timer(SIP_TIMER_G, this);
            set_timer_value(SIP_TIMER_H, SIP_TIMER_1 * 64);
            start_timer(SIP_TIMER_H, this);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Server_Invite::timer_G_Callback(SIP_Transaction *p)
{
    SIP_Transaction_Server_Invite *transaction = (SIP_Transaction_Server_Invite *) p;

    switch (transaction->_state)
    {
        case sttCompleted:
        {
            //transaction->_state = sttCompleted;
            unsigned long value = ((transaction->get_timer_value(SIP_TIMER_G) * 2) < SIP_TIMER_2) ? transaction->get_timer_value(SIP_TIMER_G) * 2 : SIP_TIMER_2;
            transaction->set_timer_value(SIP_TIMER_G, value);
            transaction->start_timer(SIP_TIMER_G, transaction);
            if (transaction->_last_response)
                transaction->_manager->send_message(transaction->_last_response);
            break;
        }

        default:
            break;
    }
}

void SIP_Transaction_Server_Invite::timer_H_Callback(SIP_Transaction *p)
{
    SIP_Transaction_Server_Invite *transaction = (SIP_Transaction_Server_Invite *) p;

    switch (transaction->_state)
    {
        case sttCompleted:
            //transaction->_state = sttTerminated;
            transaction->stop_timer(SIP_TIMER_G);
            //transaction->_ua->receive_msg(); //TODO: InformTU("Timeout occurred - TimerH");
            transaction->clear();
            break;

        default:
            break;
    }
}

void SIP_Transaction_Server_Invite::timer_I_Callback(SIP_Transaction *p)
{
    SIP_Transaction_Server_Invite *transaction = (SIP_Transaction_Server_Invite *) p;

    switch (transaction->_state)
    {
        case sttConfirmed:
            //transaction->_state = sttTerminated;
            transaction->clear();
            break;

        default:
            break;
    }
}

//-------------------------------------------

SIP_Transaction_Server_Non_Invite::~SIP_Transaction_Server_Non_Invite()
{
    stop_timer(SIP_TIMER_J);

    if (_last_response)
        delete _last_response;
}

void SIP_Transaction_Server_Non_Invite::receive_request(SIP_Request *msg)
{
    switch (_state)
    {
        case sttIdle:
            _state = sttTrying;
            _request_msg = msg;
            msg->set_can_delete(false);
            _ua->receive_request(msg, this);
            break;

        case sttProceeding:
            //_state = sttProceeding;
            if (_last_response)
                _manager->send_message(_last_response);
            break;

        case sttCompleted:
            //_state = sttCompleted;
            if (_last_response)
                _manager->send_message(_last_response);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Server_Non_Invite::send_1xx(SIP_Response *msg)
{
    switch (_state)
    {
        case sttTrying:
        case sttProceeding:
            _state = sttProceeding;
            if (_last_response)
                delete _last_response;
            _last_response = msg;
            msg->set_can_delete(false);
            _manager->send_message(_last_response);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Server_Non_Invite::send_2xx_6xx(SIP_Response *msg)
{
    switch (_state)
    {
        case sttTrying:
        case sttProceeding:
            _state = sttCompleted;
            if (_last_response)
                delete _last_response;
            _last_response = msg;
            msg->set_can_delete(false);
            _manager->send_message(_last_response);
            set_timer_value(SIP_TIMER_J, SIP_TIMER_1 * 64);
            start_timer(SIP_TIMER_J, this);
            break;

        default:
            break;
    }
}

void SIP_Transaction_Server_Non_Invite::timer_J_Callback(SIP_Transaction *p)
{
    SIP_Transaction_Server_Non_Invite *transaction = (SIP_Transaction_Server_Non_Invite *) p;

    switch (transaction->_state)
    {
        case sttCompleted:
            //transaction->_state = sttTerminated;
            transaction->clear();
            break;

        default:
            break;
    }
}

__END_SYS
