#include <sip_user_agent.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

__BEGIN_SYS

SIP_Dialog::SIP_Dialog(SIP_Message_Type type, SIP_Call_Status call_status) :
    _type(type), _call_status(call_status), _link(this)
{
    //_state = sttIdle;

    _call_id = 0;
    _local_tag = 0;
    _remote_tag = 0;
    _local_sequence_number = 0;
    _remote_sequence_number = 0;
    _local_uri = 0;
    _remote_uri = 0;
    _remote_target = 0;
}

SIP_Dialog::~SIP_Dialog()
{
    if (_call_id)
        delete _call_id;

    if (_local_tag)
        delete _local_tag;

    if (_remote_tag)
        delete _remote_tag;

    if (_local_uri)
        delete _local_uri;

    if (_remote_uri)
        delete _remote_uri;

    if (_remote_target)
        delete _remote_target;

    clear_routes();
}

void SIP_Dialog::set_dialog(const char *call_id, const char *local_tag, const char *remote_tag,
        unsigned int local_sequence_number, unsigned int remote_sequence_number,
        const char *local_uri, const char *remote_uri, const char *remote_target)
{
    if (_call_id)
        delete _call_id;
    _call_id = create_string(call_id);

    if (_local_tag)
        delete _local_tag;
    _local_tag = create_string(local_tag);

    if (_remote_tag)
        delete _remote_tag;
    _remote_tag = create_string(remote_tag);

    _local_sequence_number = local_sequence_number;

    _remote_sequence_number = remote_sequence_number;

    if (_local_uri)
        delete _local_uri;
    _local_uri = create_string(local_uri);

    if (_remote_uri)
        delete _remote_uri;
    _remote_uri = create_string(remote_uri);

    set_remote_target(remote_target);

    //_state = sttConfirmed;
}

void SIP_Dialog::set_remote_target(const char *remote_target)
{
    if (_remote_target)
        delete _remote_target;
    _remote_target = create_string(remote_target);
}

void SIP_Dialog::add_route_back(SIP_Header_Route *route)
{
    SIP_Header_Route *route2 = new SIP_Header_Route(*route);
    _routes.insert_tail(&route2->_link);
}

void SIP_Dialog::add_route_front(SIP_Header_Route *route)
{
    SIP_Header_Route *route2 = new SIP_Header_Route(*route);
    _routes.insert_head(&route2->_link);
}

void SIP_Dialog::clear_routes()
{
    Simple_List<SIP_Header/*Route*/>::Iterator it = _routes.begin();
    while (it != _routes.end())
    {
        SIP_Header_Route *route = (SIP_Header_Route *) it->object();
        _routes.remove(it++);
        delete route;
    }
}

SIP_Header_Route *SIP_Dialog::get_route(int pos)
{
    int i = 0;
    Simple_List<SIP_Header/*Route*/>::Iterator it = _routes.begin();
    while (it != _routes.end())
    {
        if (i == pos)
            return (SIP_Header_Route *) it->object();
        i++; it++;
    }
    return 0;
}

//-------------------------------------------

void SIP_Subscription::set_subscription(SIP_Event_Package event_type, const char *event_id)
{
    _event_type = event_type;

    if (_event_id)
        delete _event_id;
    _event_id = create_string(event_id);
}

/*void SIP_Subscription::start_timer(void *p)
{
    _timer_handler = new Functor_Handler(&, p);
    _timer_alarm = new Alarm(_timer_value, _timer_handler, 1);
}

void SIP_Subscription::stop_timer()
{
    if (_timer_handler)
    {
        delete _timer_handler;
        _timer_handler = 0;
    }

    if (_timer_alarm)
    {
        delete _timer_alarm;
        _timer_alarm = 0;
    }
}*/

void SIP_Subscription::clear()
{
    _event_type = SIP_EVENT_PACKAGE_INVALID;

    if (_event_id)
        delete _event_id;
    _event_id = 0;

    //stop_timer();
}

//-------------------------------------------

SIP_User_Agent::SIP_User_Agent(const char *uri) : _link(this)
{
    _uri = create_string(uri);
    _text_received = 0;
}

SIP_User_Agent::~SIP_User_Agent()
{
    Simple_List<SIP_Transaction>::Iterator it = _transactions.begin();
    while (it != _transactions.end())
    {
        SIP_Transaction *transaction = it->object();
        _transactions.remove(it++);
        delete transaction;
    }
}

SIP_Request *SIP_User_Agent::create_request(SIP_Message_Type msg_type, SIP_Dialog *dialog, const char *to, SIP_Message *invite)
{
    SIP_Request *request = new SIP_Request(msg_type);

    /*if (!request)
    {
        db<SIP_User_Agent>(WRN) << "SIP_User_Agent::create_request -> Failed to create request (message type = " << msg_type << ")\n";
        return 0;
    }*/

    SIP_Header_Via *header_via = new SIP_Header_Via();
    char branch[20];
    strcpy(branch, "z9hG4bK");
    SIP_Manager::random(&branch[7]);
    header_via->set_via("SIP", "2.0", SIP_TRANSPORT_UDP, SIP_Manager::get_instance()->get_host_ip(), SIP_Manager::get_instance()->get_host_port(), branch);
    request->add_header(header_via);

    SIP_Header_To *header_to = new SIP_Header_To();
    request->add_header(header_to);

    SIP_Header_From *header_from = new SIP_Header_From();
    request->add_header(header_from);

    SIP_Header_Call_ID *header_call_id = new SIP_Header_Call_ID();
    request->add_header(header_call_id);

    SIP_Header_CSeq *header_cseq = new SIP_Header_CSeq();
    request->add_header(header_cseq);

    SIP_Header_Max_Forwards *header_max_forwards = new SIP_Header_Max_Forwards();
    header_max_forwards->set_number(70);
    request->add_header(header_max_forwards);

    SIP_Header_Contact *header_contact = new SIP_Header_Contact();
    request->add_header(header_contact);

    if (!dialog)
    {
        header_to->set_address(to);

        char tag[20];
        SIP_Manager::random(tag);
        header_from->set_address(_uri);
        header_from->set_tag(tag);

        char call_id[100], host[255], aux[255];
        //static int local_id_resgister = 2900;
        strcpy(host, _uri);
        match(host, ":" , aux);
        skip(host, " \t");
        match(host, "@" , aux);
        skip(host, " \t");
        SIP_Manager::random(call_id);
        strcat(call_id, "@");
        strcat(call_id, host);
        header_call_id->set_string(call_id);

        static unsigned int sequence = 1;
        header_cseq->set_cseq(msg_type, sequence++);

        header_contact->set_address(_uri);

        request->set_request_line(msg_type, to, SIP_VERSION);
    }else
    {
        header_to->set_address(dialog->_remote_uri);
        if (dialog->_remote_tag)
            header_to->set_tag(dialog->_remote_tag);

        header_from->set_address(dialog->_local_uri);
        if (dialog->_local_tag)
            header_from->set_tag(dialog->_local_tag);

        header_call_id->set_string(dialog->_call_id);

        if (dialog->_local_sequence_number == 0)
            dialog->_local_sequence_number = 1;
        unsigned int sequence = 0;
        if (msg_type == SIP_REQUEST_ACK)
        {
            SIP_Header_CSeq *cseq = (SIP_Header_CSeq *) invite->get_header(SIP_HEADER_CSEQ);
            sequence = cseq->get_sequence();
        }else
        {
            sequence = dialog->_local_sequence_number + 1;
            dialog->_local_sequence_number = sequence;
        }
        header_cseq->set_cseq(msg_type, sequence);

        header_contact->set_address(dialog->_local_uri);

        const char *remote = 0;
        int routes_size = dialog->get_routes_size();
        if (routes_size == 0)
            remote = dialog->_remote_target;
        else if (routes_size > 0)
        {
            SIP_Header_Route *route = dialog->get_route(0);
            if (route->is_lr())
            {
                remote = dialog->_remote_target;

                for (int i = 0; i < routes_size; i++)
                    request->add_header(new SIP_Header_Route(*(dialog->get_route(i))));
            }else
            {
                remote = dialog->get_route(0)->get_address();

                for (int i = 1; i < routes_size; i++)
                request->add_header(new SIP_Header_Route(*(dialog->get_route(i))));

                SIP_Header_Route *header = new SIP_Header_Route();
                header->set_address(dialog->_remote_target, true);
                request->add_header(header);
            }
        }

        char p_remote[512];
        strcpy(p_remote, remote);
        int size = strlen(p_remote);
        for (int i = size - 1; i >= size - 6; i--)
        {
            if (p_remote[i] == ':')
            {
                p_remote[i] = 0;
                break;
            }
        }
        request->set_request_line(msg_type, p_remote, SIP_VERSION);
    }

    return request;
}

SIP_Response *SIP_User_Agent::create_response(unsigned short status_code, SIP_Request *request)
{
    if ((status_code < 100) || (status_code > 699) || (!request))
    {
        db<SIP_User_Agent>(WRN) << "SIP_User_Agent::create_response -> Invalid parameters\n";
        return 0;
    }

    int via_num = request->get_num_header(SIP_HEADER_VIA);
    SIP_Header_From *from = (SIP_Header_From *) request->get_header(SIP_HEADER_FROM);
    SIP_Header_Call_ID *call_id = (SIP_Header_Call_ID *) request->get_header(SIP_HEADER_CALLID);
    SIP_Header_CSeq *cseq = (SIP_Header_CSeq *) request->get_header(SIP_HEADER_CSEQ);
    SIP_Header_To *to = (SIP_Header_To *) request->get_header(SIP_HEADER_TO);

    SIP_Response *response = new SIP_Response(status_code);

    for (int i = 0; i < via_num; i++)
    {
        SIP_Header_Via *via = (SIP_Header_Via *) request->get_header(SIP_HEADER_VIA, i);
        response->add_header(new SIP_Header_Via(*via));
    }

    SIP_Header_To *header_to = new SIP_Header_To(*to);
    response->add_header(header_to);
    const char *to_tag = header_to->get_tag();
    if ((!to_tag) && (status_code != 100))
    {
        char tag[20];
        SIP_Manager::random(tag);
        header_to->set_tag(tag);
    }

    response->add_header(new SIP_Header_From(*from));
    response->add_header(new SIP_Header_Call_ID(*call_id));
    response->add_header(new SIP_Header_CSeq(*cseq));

    SIP_Header_Max_Forwards *header_max_forwards = new SIP_Header_Max_Forwards();
    header_max_forwards->set_number(70);
    response->add_header(header_max_forwards);

    if ((request->get_msg_type() == SIP_REQUEST_INVITE) || (request->get_msg_type() == SIP_REQUEST_SUBSCRIBE))
    {
        SIP_Header_Contact *header_contact = new SIP_Header_Contact();
        const char *address = header_to->get_address();
        header_contact->set_address(address);
        response->add_header(header_contact);
    }

    if (request->get_msg_type() == SIP_REQUEST_INVITE)
    {
        SIP_Header_Content_Disposition *header_content_disposition = new SIP_Header_Content_Disposition();
        header_content_disposition->set_string("session");
        response->add_header(header_content_disposition);

        SIP_SDP_Body *sdp = new SIP_SDP_Body();
        response->set_body(sdp);
    }

    return response;
}

SIP_Request *SIP_User_Agent::create_ack(const char *to, SIP_Request *invite)
{
    SIP_Dialog *dialog = matching_dialog(to, SIP_REQUEST_INVITE);

    if ((!dialog) || (!invite))
    {
        db<SIP_User_Agent>(WRN) << "SIP_User_Agent::create_ack -> Failed to create request\n";
        return 0;
    }

    return create_request(SIP_REQUEST_ACK, dialog, 0, invite);
}

SIP_Request *SIP_User_Agent::create_bye(const char *to)
{
    SIP_Dialog *dialog = matching_dialog(to, SIP_REQUEST_INVITE);

    if (!dialog)
    {
        db<SIP_User_Agent>(WRN) << "SIP_User_Agent::create_bye -> Failed to create request\n";
        return 0;
    }

    return create_request(SIP_REQUEST_BYE, dialog);
}

SIP_Request *SIP_User_Agent::create_invite(const char *to)
{
    SIP_Dialog *dialog = matching_dialog(to, SIP_REQUEST_INVITE);

    if ((dialog) || (!to))
    {
        db<SIP_User_Agent>(WRN) << "SIP_User_Agent::create_invite -> Failed to create request\n";
        return 0;
    }

    SIP_Request *invite = create_request(SIP_REQUEST_INVITE, 0, to);

    SIP_Header_Allow *header_allow = new SIP_Header_Allow();
    for (int i = 0; i < (SIP_MESSAGE_TYPE_INVALID - 1); i++)
        header_allow->add_allowed((SIP_Message_Type) i);
    invite->add_header(header_allow);

    SIP_Header_Content_Disposition *header_content_disposition = new SIP_Header_Content_Disposition();
    header_content_disposition->set_string("session");
    invite->add_header(header_content_disposition);

    SIP_SDP_Body *sdp = new SIP_SDP_Body();
    invite->set_body(sdp);

    return invite;
}

SIP_Request *SIP_User_Agent::create_message(const char *to, const char *data)
{
    //SIP_Dialog *dialog = matching_dialog(to);

    if ((!to) || (!data))
    {
        db<SIP_User_Agent>(WRN) << "SIP_User_Agent::create_message -> Failed to create request\n";
        return 0;
    }

    SIP_Request *message = create_request(SIP_REQUEST_MESSAGE, 0, to);

    SIP_Text_Plain_Body *text = new SIP_Text_Plain_Body();
    text->set_text(data);
    message->set_body(text);

    return message;
}

SIP_Request *SIP_User_Agent::create_notify(const char *to, SIP_Subscription_State state, SIP_Pidf_Xml_Basic_Element pidf_xml_element, unsigned int expires)
{
    SIP_Dialog *dialog = matching_dialog(to, SIP_REQUEST_SUBSCRIBE);

    if ((!dialog) || (!_subscription.is_active()))
    {
        db<SIP_User_Agent>(WRN) << "SIP_User_Agent::create_notify -> Failed to create request\n";
        return 0;
    }

    SIP_Request *notify = create_request(SIP_REQUEST_NOTIFY, dialog);

    SIP_Header_Event *header_event = new SIP_Header_Event();
    header_event->set_event(_subscription._event_type, _subscription._event_id);
    notify->add_header(header_event);

    SIP_Header_Subscription_State *header_subscription_state = new SIP_Header_Subscription_State();
    header_subscription_state->set_subscription_state(state, expires);
    notify->add_header(header_subscription_state);

    SIP_Pidf_Xml_Body *pidf = new SIP_Pidf_Xml_Body();
    pidf->set_element(pidf_xml_element);
    notify->set_body(pidf);

    return notify;
}

void SIP_User_Agent::send_request(SIP_Request *request)
{
    if (!request)
    {
        db<SIP_User_Agent>(WRN) << "SIP_User_Agent::send_request -> Invalid parameter\n";
        return;
    }

    if (request->get_msg_type() == SIP_REQUEST_INVITE)
    {
        SIP_Transaction_Client_Invite *transaction = new SIP_Transaction_Client_Invite(this);
        add_transaction(transaction);
        transaction->send_invite(request);
    }else
    {
        SIP_Transaction_Client_Non_Invite *transaction = new SIP_Transaction_Client_Non_Invite(this);
        add_transaction(transaction);
        transaction->send_request(request);
    }

    if (request->get_can_delete())
        delete request;
}

void SIP_User_Agent::send_response(SIP_Response *response, SIP_Message_Type request_type, SIP_Transaction *transaction)
{
    if ((!response) || (!transaction))
    {
        db<SIP_User_Agent>(WRN) << "SIP_User_Agent::send_response -> Invalid parameters\n";
        return;
    }

    unsigned short status_code = response->get_status_code();

    if (request_type == SIP_REQUEST_INVITE)
    {
        if ((status_code >= 100) && (status_code <= 199))
            ((SIP_Transaction_Server_Invite *) transaction)->send_1xx(response);

        else if ((status_code >= 200) && (status_code <= 299))
            ((SIP_Transaction_Server_Invite *) transaction)->send_2xx(response);

        else if ((status_code >= 300) && (status_code <= 699))
            ((SIP_Transaction_Server_Invite *) transaction)->send_3xx_6xx(response);
    }else
    {
        if ((status_code >= 100) && (status_code <= 199))
            ((SIP_Transaction_Server_Non_Invite *) transaction)->send_1xx(response);

        else if ((status_code >= 200) && (status_code <= 699))
            ((SIP_Transaction_Server_Non_Invite *) transaction)->send_2xx_6xx(response);
    }

    if (response->get_can_delete())
        delete response;
}

bool SIP_User_Agent::receive_response(SIP_Response *response)
{
    if (response->get_num_header(SIP_HEADER_VIA) != 1)
        return false;

    unsigned short status_code = response->get_status_code();
    SIP_Transaction *transaction = matching_transaction(response);

    if (!transaction)
    {
        db<SIP_User_Agent>(WRN) << "SIP_User_Agent::receive_response -> Ignoring invalid response\n";
        return false;
    }

    if (transaction->get_transaction_type() == SIP_TRANSACTION_CLIENT_INVITE)
    {
        if ((status_code >= 100) && (status_code <= 199))
            ((SIP_Transaction_Client_Invite *) transaction)->receive_1xx(response);

        else if ((status_code >= 200) && (status_code <= 299))
            ((SIP_Transaction_Client_Invite *) transaction)->receive_2xx(response);

        else if ((status_code >= 300) && (status_code <= 699))
            ((SIP_Transaction_Client_Invite *) transaction)->receive_3xx_6xx(response);

    }else if (transaction->get_transaction_type() == SIP_TRANSACTION_CLIENT_NON_INVITE)
    {
        if ((status_code >= 100) && (status_code <= 199))
            ((SIP_Transaction_Client_Non_Invite *) transaction)->receive_1xx(response);

        else if ((status_code >= 200) && (status_code <= 699))
            ((SIP_Transaction_Client_Non_Invite *) transaction)->receive_2xx_6xx(response);
    }

    return true;
}

bool SIP_User_Agent::receive_response(SIP_Request *request, SIP_Response *response, SIP_Transaction *transaction)
{
    unsigned short status_code = response->get_status_code();
    SIP_Dialog *dialog = matching_dialog(response, request->get_msg_type());

    if ((status_code >= 200) && (status_code <= 299))
        receive_2xx(request, response, transaction, dialog);

    else if ((status_code >= 300) && (status_code <= 699))
        receive_3xx_6xx(request, response, transaction, dialog);

    return true;
}

bool SIP_User_Agent::receive_2xx(SIP_Request *request, SIP_Response *response, SIP_Transaction *transaction, SIP_Dialog *dialog)
{
    switch (request->get_msg_type())
    {
        case SIP_REQUEST_INVITE:
        {
            if (!dialog)
            {
                SIP_Dialog *new_dialog = create_dialog_client(request, response);
                if (new_dialog)
                {
                    SIP_Request *ack = create_ack(new_dialog->_remote_uri, request);
                    SIP_Manager::get_instance()->send_message(ack);
                    delete ack;
                }
            }else
            {
                SIP_Header_Contact *contact = (SIP_Header_Contact *) response->get_header(SIP_HEADER_CONTACT);
                if (contact)
                {
                    const char *target = contact->get_address();
                    if (target)
                        dialog->set_remote_target(target);
                }
            }
            break;
        }

        case SIP_REQUEST_BYE:
        {
            if (dialog)
            {
                SIP_Manager::_callback(SIP_SESSION_TERMINATED, this, dialog->_remote_uri);
                remove_dialog(dialog);
            }
            break;
        }

        default:
            break;
    }

    return true;
}

bool SIP_User_Agent::receive_3xx_6xx(SIP_Request *request, SIP_Response *response, SIP_Transaction *transaction, SIP_Dialog *dialog)
{
    unsigned short status_code = response->get_status_code();

    if ((status_code == 408) || (status_code == 481) || (request->get_msg_type() == SIP_REQUEST_BYE))
    {
        if (dialog)
        {
            SIP_Manager::_callback(SIP_SESSION_TERMINATED, this, dialog->_remote_uri);
            remove_dialog(dialog);
        }
        return true;
    }

    if (dialog)
    {
        SIP_Request *bye = create_bye(dialog->_remote_uri);
        send_request(bye);

        SIP_Manager::_callback(SIP_SESSION_TERMINATED, this, dialog->_remote_uri);
        remove_dialog(dialog);
    }

    return true;
}

bool SIP_User_Agent::receive_request(SIP_Request *request)
{
    SIP_Message_Type request_type = request->get_msg_type();
    SIP_Transaction *transaction = matching_transaction(request);

    if ((!transaction) && (request_type == SIP_REQUEST_ACK))
    {
        db<SIP_User_Agent>(TRC) << "SIP_User_Agent::receive_request -> Received ACK message\n";
        return true;

    }else if ((!transaction) && (request_type == SIP_REQUEST_INVITE))
    {
        if (get_call()) //If there is a dialog, it does not receive new INVITE!
        {
            db<SIP_User_Agent>(WRN) << "SIP_User_Agent::receive_request -> There is already a dialog\n";
            return false;
        }

        transaction = new SIP_Transaction_Server_Invite(this);
        add_transaction(transaction);

    }else if (!transaction) //&& (request_type != SIP_REQUEST_INVITE) && (request_type != SIP_REQUEST_ACK))
    {
        transaction = new SIP_Transaction_Server_Non_Invite(this);
        add_transaction(transaction);
    }

    db<SIP_User_Agent>(TRC) << "SIP_User_Agent::receive_request -> New request received\n";

    if (request_type == SIP_REQUEST_INVITE)
        ((SIP_Transaction_Server_Invite *) transaction)->receive_invite(request);

    else if (request_type == SIP_REQUEST_ACK)
        ((SIP_Transaction_Server_Invite *) transaction)->receive_ack(request);

    else
        ((SIP_Transaction_Server_Non_Invite *) transaction)->receive_request(request);

    return true;
}

bool SIP_User_Agent::receive_request(SIP_Request *request, SIP_Transaction *transaction)
{
    SIP_Message_Type request_type = request->get_msg_type();
    SIP_Dialog *dialog = matching_dialog(request, request_type);

    if (request_type != SIP_REQUEST_ACK)
    {
        SIP_Header_Require *require = (SIP_Header_Require *) request->get_header(SIP_HEADER_REQUIRE);
        if (require)
        {
            SIP_Response *response = create_response(420, request);
            SIP_Header_Unsupported *unsupported = new SIP_Header_Unsupported();
            response->add_header(unsupported);

            for (int i = 0; i < require->get_option_size(); i++)
            {
                const char *option = require->get_option(i);
                unsupported->add_option(option);
            }

            send_response(response, request_type, transaction);
            return false;
        }
    }

    if (!dialog)
    {
        SIP_Header_To *header_to = (SIP_Header_To *) request->get_header(SIP_HEADER_TO);
        const char *remote_tag = header_to->get_tag();
        if (remote_tag)
        {
            SIP_Response *response = create_response(481, request);
            send_response(response, request_type, transaction);
            return false;
        }
    }else
    {
        SIP_Header_CSeq *header_cseq = (SIP_Header_CSeq *) request->get_header(SIP_HEADER_CSEQ);
        unsigned int sequence_number = header_cseq->get_sequence();
        if ((dialog->_remote_sequence_number == 0) || (sequence_number > dialog->_remote_sequence_number))
            dialog->_remote_sequence_number = sequence_number;
        /*else if (sequence_number < dialog->_remote_sequence_number) //TODO: Descomentar quando verificar Dialog!
        {
            SIP_Response *response = create_response(500, request);
            send_response(response, request_type, transaction);
            return false;
        }*/
    }

    switch (request_type)
    {
        case SIP_REQUEST_ACK:       return true; //receive_ack(request, transaction, dialog);
        case SIP_REQUEST_BYE:       return receive_bye(request, transaction, dialog);
        case SIP_REQUEST_INVITE:    return receive_invite(request, transaction, dialog);
        case SIP_REQUEST_MESSAGE:   return receive_message(request, transaction, dialog);
        case SIP_REQUEST_NOTIFY:    return true; //receive_notify(request, transaction, dialog);
        case SIP_REQUEST_SUBSCRIBE: return receive_subscribe(request, transaction, dialog);
        default: break;
    }

    return false;
}

bool SIP_User_Agent::receive_bye(SIP_Request *request, SIP_Transaction *transaction, SIP_Dialog *dialog)
{
    if (!dialog)
    {
        SIP_Response *response = create_response(481, request);
        send_response(response, SIP_REQUEST_BYE, transaction);
    }else
    {
        SIP_Response *response = create_response(200, request);
        send_response(response, SIP_REQUEST_BYE, transaction);

        SIP_Manager::_callback(SIP_SESSION_TERMINATED, this, dialog->_remote_uri);
        remove_dialog(dialog);
    }

    return true;
}

bool SIP_User_Agent::receive_invite(SIP_Request *request, SIP_Transaction *transaction, SIP_Dialog *dialog)
{
    if (!dialog)
    {
        SIP_Response *response = create_response(200, request);

        int record_route_num = request->get_num_header(SIP_HEADER_RECORD_ROUTE);
        for (int i = 0; i < record_route_num; i++)
        {
            SIP_Header_Record_Route *header_record_route = (SIP_Header_Record_Route *) request->get_header(SIP_HEADER_RECORD_ROUTE, i);
            response->add_header(new SIP_Header_Record_Route(*header_record_route));
        }

        SIP_Header_Allow *header_allow = new SIP_Header_Allow();
        for (int i = 0; i < (SIP_MESSAGE_TYPE_INVALID - 1); i++)
            header_allow->add_allowed((SIP_Message_Type) i);
        response->add_header(header_allow);

        create_dialog_server(request, response);
        send_response(response, SIP_REQUEST_INVITE, transaction);

        //TODO: 13.3.1.4 The INVITE is Accepted: Reenviar periodicamente a resposta até receber ACK
    }else
    {
        SIP_Header_Contact *contact = (SIP_Header_Contact *) request->get_header(SIP_HEADER_CONTACT);
        if (contact)
        {
            const char *target = contact->get_address();
            if ((target) && (!strcmp(dialog->_remote_target, target)))
                dialog->set_remote_target(target);
        }
    }

    return true;
}

bool SIP_User_Agent::receive_message(SIP_Request *request, SIP_Transaction *transaction, SIP_Dialog *dialog)
{
    SIP_Response *response = create_response(200, request);
    send_response(response, SIP_REQUEST_MESSAGE, transaction);

    SIP_Body *body = request->get_body();
    if ((!body) || (body->get_body_type() != SIP_BODY_TEXT_PLAIN))
        return false;

    const char *remote_uri = ((SIP_Header_From *) request->get_header(SIP_HEADER_FROM))->get_address();
    _text_received = ((SIP_Text_Plain_Body *) body)->get_text();

    SIP_Manager::_callback(SIP_MESSAGE_RECEIVED, this, remote_uri);

    _text_received = 0;
    return true;
}

bool SIP_User_Agent::receive_subscribe(SIP_Request *request, SIP_Transaction *transaction, SIP_Dialog *dialog)
{
    SIP_Header_Event *header_event = (SIP_Header_Event *) request->get_header(SIP_HEADER_EVENT);
    if (!header_event)
        return false;

    SIP_Event_Package event_type = header_event->get_type();

    if (((!_subscription.is_active()) && (event_type == SIP_EVENT_PRESENCE)) ||
        ((_subscription.is_active()) && (event_type == _subscription._event_type)))
    {
        SIP_Response *response = create_response(200, request);

        SIP_Header_Expires *expires = (SIP_Header_Expires *) request->get_header(SIP_HEADER_EXPIRES);
        SIP_Header_Expires *header_expires = new SIP_Header_Expires();
        unsigned int time = (expires) ? expires->get_number() : 3600;
        header_expires->set_number(time);
        response->add_header(header_expires);

        if ((!dialog) && (time > 0))
            create_dialog_server(request, response);

        send_response(response, SIP_REQUEST_SUBSCRIBE, transaction);

        if (dialog)
        {
            if (time > 0)
            {
                //TODO: Acionar ou atualizar timer por tempo do Expires
            }else
            {
                SIP_Manager::_callback(SIP_SUBSCRIPTION_TERMINATED, this, dialog->_remote_uri);
                _subscription.clear();
                remove_dialog(dialog); //TODO: Deletar mesmo se tem INVITE session?
            }
        }
    }else
    {
        SIP_Response *response = create_response(489, request);
        send_response(response, SIP_REQUEST_SUBSCRIBE, transaction);
    }

    return true;
}

SIP_Dialog *SIP_User_Agent::create_dialog_client(SIP_Request *request, SIP_Response *response)
{
    SIP_Header_From *from_request = (SIP_Header_From *) request->get_header(SIP_HEADER_FROM);
    SIP_Header_From *from_response = (SIP_Header_From *) response->get_header(SIP_HEADER_FROM);
    SIP_Header_Call_ID *call_id = (SIP_Header_Call_ID *) request->get_header(SIP_HEADER_CALLID);
    SIP_Header_Contact *contact = (SIP_Header_Contact *) response->get_header(SIP_HEADER_CONTACT);
    SIP_Header_CSeq *cseq = (SIP_Header_CSeq *) request->get_header(SIP_HEADER_CSEQ);
    SIP_Header_To *to = (SIP_Header_To *) response->get_header(SIP_HEADER_TO);

    if (!contact)
        return 0;

    const char *id = call_id->get_string();
    const char *local_tag = from_request->get_tag();
    const char *remote_tag = to->get_tag(); //It can be NULL, compatibility with RFC 2543
    const char *local_uri = from_response->get_address();
    const char *remote_uri = to->get_address();
    const char *target = contact->get_address();
    unsigned int sequence_number = cseq->get_sequence();

    if ((!id) || (!local_tag) || (!local_uri) || (!remote_uri) || (!target))
        return 0;

    SIP_Dialog *dialog = add_dialog(SIP_REQUEST_INVITE, SIP_CALL_STATUS_OUTGOING);
    dialog->set_dialog(id, local_tag, remote_tag, sequence_number, 0, local_uri, remote_uri, target);

    int record_route_num = response->get_num_header(SIP_HEADER_RECORD_ROUTE);
    for (int i = 0; i < record_route_num; i++)
    {
        SIP_Header_Route *route = (SIP_Header_Route *) response->get_header(SIP_HEADER_RECORD_ROUTE, i);
        dialog->add_route_front(route);
    }

    SIP_Manager::_callback(SIP_SESSION_INITIATED, this, remote_uri);
    return dialog;
}

SIP_Dialog *SIP_User_Agent::create_dialog_server(SIP_Request *request, SIP_Response *response)
{
    SIP_Header_From *from = (SIP_Header_From *) request->get_header(SIP_HEADER_FROM);
    SIP_Header_Call_ID *call_id = (SIP_Header_Call_ID *) request->get_header(SIP_HEADER_CALLID);
    SIP_Header_CSeq *cseq = (SIP_Header_CSeq *) request->get_header(SIP_HEADER_CSEQ);
    SIP_Header_Contact *contact = (SIP_Header_Contact *) request->get_header(SIP_HEADER_CONTACT);
    SIP_Header_To *to_request = (SIP_Header_To *) request->get_header(SIP_HEADER_TO);
    SIP_Header_To *to_response = (SIP_Header_To *) response->get_header(SIP_HEADER_TO);

    if (!contact)
        return 0;

    const char *id = call_id->get_string();
    const char *local_tag = to_response->get_tag();
    const char *remote_tag = from->get_tag(); //Pode ser NULL, compatibilidade com RFC 2543
    const char *local_uri = to_request->get_address();
    const char *remote_uri = from->get_address();
    const char *target = contact->get_address();
    unsigned int sequence_number = cseq->get_sequence();

    if ((!id) || (!local_tag) || (!local_uri) || (!remote_uri) || (!target))
        return 0;

    SIP_Message_Type type = request->get_msg_type();
    if (type == SIP_REQUEST_SUBSCRIBE)
    {
        SIP_Header_Event *event = (SIP_Header_Event *) request->get_header(SIP_HEADER_EVENT);
        if (!event)
            return 0;

        SIP_Event_Package event_type = event->get_type();
        const char *event_id = event->get_id();

        if (event_type == SIP_EVENT_PACKAGE_INVALID)
            return 0;

        _subscription.set_subscription(event_type, event_id);
    }

    SIP_Dialog *dialog = add_dialog(type, SIP_CALL_STATUS_INCOMING);
    dialog->set_dialog(id, local_tag, remote_tag, 0, sequence_number, local_uri, remote_uri, target);

    int record_route_num = response->get_num_header(SIP_HEADER_RECORD_ROUTE);
    for (int i = 0; i < record_route_num; i++)
    {
        SIP_Header_Route *route = (SIP_Header_Route *) response->get_header(SIP_HEADER_RECORD_ROUTE, i);
        dialog->add_route_back(route);
    }

    if (type == SIP_REQUEST_SUBSCRIBE)
        SIP_Manager::_callback(SIP_SUBSCRIPTION_INITIATED, this, remote_uri);
    else //if (type == SIP_REQUEST_INVITE)
        SIP_Manager::_callback(SIP_SESSION_INITIATED, this, remote_uri);
    return dialog;
}

SIP_Dialog *SIP_User_Agent::matching_dialog(SIP_Message *msg, SIP_Message_Type type)
{
    if ((type == SIP_REQUEST_ACK) || (type == SIP_REQUEST_BYE) || (type == SIP_REQUEST_INVITE))
        type = SIP_REQUEST_INVITE;
    else //if ((type == SIP_REQUEST_MESSAGE) || (type == SIP_REQUEST_NOTIFY) || (type == SIP_REQUEST_SUBSCRIBE))
        type = SIP_REQUEST_SUBSCRIBE;

    Simple_List<SIP_Dialog>::Iterator it = _dialogs.begin();
    while (it != _dialogs.end())
    {
        SIP_Dialog *dialog = it->object();
        it++;

        if ((dialog->_type != type) || (!dialog->_call_id) || (!dialog->_local_tag)) //|| (!dialog->_remote_tag))
            continue;

        SIP_Header_From *header_from = (SIP_Header_From *) msg->get_header(SIP_HEADER_FROM);
        SIP_Header_Call_ID *header_call_id = (SIP_Header_Call_ID *) msg->get_header(SIP_HEADER_CALLID);
        SIP_Header_To *header_to = (SIP_Header_To *) msg->get_header(SIP_HEADER_TO);

        const char *call_id = header_call_id->get_string();
        const char *local_tag = 0; //INVITE does not have yet!
        const char *remote_tag = 0; //It can be NULL, compatibility with RFC 2543

        if (msg->get_msg_type() == SIP_RESPONSE)
        {
            local_tag = header_from->get_tag();
            remote_tag = header_to->get_tag();
        }else
        {
            local_tag = header_to->get_tag();
            remote_tag = header_from->get_tag();
        }

        if ((!call_id)) //|| (!local_tag) || (!remote_tag))
            continue;

        bool ret1 = !strcmp(dialog->_call_id, call_id);
        bool ret2 = (local_tag) ? !strcmp(dialog->_local_tag, local_tag) : true;
        bool ret3 = (remote_tag) ? !strcmp(dialog->_remote_tag, remote_tag) : true;

        if ((local_tag) && (remote_tag) && (!strcmp(local_tag, remote_tag)) && ((ret2) || (ret3)))
        {
            ret2 = true;
            ret3 = true;
        }

        if ((ret1) && (ret2) && (ret3))
            return dialog;
    }

    return 0;
}

SIP_Dialog *SIP_User_Agent::matching_dialog(const char *to, SIP_Message_Type type)
{
    Simple_List<SIP_Dialog>::Iterator it = _dialogs.begin();
    while (it != _dialogs.end())
    {
        SIP_Dialog *dialog = it->object();
        it++;

        if ((dialog->_type != type) || (!dialog->_remote_uri))
            continue;

        if (!strcmp(to, dialog->_remote_uri))
            return dialog;
    }
    return 0;
}

SIP_Transaction *SIP_User_Agent::matching_transaction(SIP_Message *msg)
{
    Simple_List<SIP_Transaction>::Iterator it = _transactions.begin();
    while (it != _transactions.end())
    {
        SIP_Transaction *transaction = it->object();
        it++;

        if (transaction->matching_transaction(msg))
            return transaction;
    }
    return 0;
}

const char *SIP_User_Agent::get_subscriber()
{
    Simple_List<SIP_Dialog>::Iterator it = _dialogs.begin();
    while (it != _dialogs.end())
    {
        SIP_Dialog *dialog = it->object();
        it++;

        if (dialog->_type != SIP_REQUEST_SUBSCRIBE)
            continue;

        return dialog->_remote_uri;
    }
    return 0;
}

SIP_Dialog *SIP_User_Agent::get_call()
{
    Simple_List<SIP_Dialog>::Iterator it = _dialogs.begin();
    while (it != _dialogs.end())
    {
        SIP_Dialog *dialog = it->object();
        it++;

        if (dialog->_type == SIP_REQUEST_INVITE)
            return dialog;
    }
    return 0;
}

SIP_Call_Status SIP_User_Agent::get_call_status()
{
    SIP_Dialog *dialog = get_call();
    if (dialog)
        return dialog->_call_status;

    return SIP_CALL_STATUS_INACTIVE;
}

SIP_Dialog *SIP_User_Agent::add_dialog(SIP_Message_Type type, SIP_Call_Status call_status)
{
    SIP_Dialog *dialog = new SIP_Dialog(type, call_status);
    _dialogs.insert(&dialog->_link);
    return dialog;
}

__END_SYS
