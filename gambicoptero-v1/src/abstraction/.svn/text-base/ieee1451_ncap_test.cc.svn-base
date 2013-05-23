//#define USE_SIP

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <ieee1451_ncap.h>
#include <ieee1451_objects.h>
#include <utility/list.h>
#ifdef USE_SIP
    #include <sip_defs.h>
    #include <sip_manager.h>
    #include <sip_user_agent.h>
    #include <thread.h>
#endif

__USING_SYS

OStream cout;

class IEEE1451_TEDS_Retriever;

class IEEE1451_NCAP_Application : public IEEE1451_NCAP::Listener
{
private:
    struct TIM_Cache
    {
        IEEE1451_TIM_Channel *_tim;
        Simple_List<TIM_Cache>::Element _link;

#ifdef USE_SIP
        SIP_User_Agent *_ua;
        Thread _session_thread;
        Send_RTP _send_rtp;

        TIM_Cache(IEEE1451_TIM_Channel *tim, SIP_User_Agent *ua) : _tim(tim), _link(this), _ua(ua),
                _session_thread(&IEEE1451_NCAP_Application::send_read_multimedia_data_set_thread,
                IEEE1451_NCAP_Application::get_instance(), _tim->_address, (unsigned short) 0x01) {}
        ~TIM_Cache() { delete _tim; delete _ua; }
#else
        TIM_Cache(IEEE1451_TIM_Channel *tim) : _tim(tim), _link(this) {}
        ~TIM_Cache() { delete _tim; }
#endif
    };

    IEEE1451_NCAP_Application();

public:
    ~IEEE1451_NCAP_Application();

    static IEEE1451_NCAP_Application *get_instance();

    TIM_Cache *get_tim_cache(const IP::Address &address);
#ifdef USE_SIP
    TIM_Cache *get_tim_cache(const char *uri);
#endif
    IEEE1451_TEDS_Retriever *get_retriever(unsigned short trans_id);

    void update_tim(const IP::Address &address);
    void update_tim_completed(IEEE1451_TEDS_Retriever *retriever, IEEE1451_TIM_Channel *tim, IP::Address address);

    void report_tim_connected(const IP::Address &address);
    void report_tim_disconnected(const IP::Address &address);
    //void report_error(unsigned short trans_id, int error_code);
    void report_command_reply(const IP::Address &address, unsigned short trans_id, const char *message, unsigned int length);
    void report_tim_initiated_message(const IP::Address &address, const char *message, unsigned int length);

    void read_temperature(const IP::Address &address, const char *buffer, unsigned short length);
    void read_audio(const IP::Address &address, const char *buffer, unsigned short length);

    unsigned short send_operate(const IP::Address &address, unsigned short channel_number);
    unsigned short send_idle(const IP::Address &address, unsigned short channel_number);
    unsigned short send_read_teds(const IP::Address &address, unsigned short channel_number, char tedsId);
    unsigned short send_read_data_set(const IP::Address &address, unsigned short channel_number);
    static int send_read_multimedia_data_set_thread(IEEE1451_NCAP_Application *ncap, IP::Address address, unsigned short channel_number);

#ifdef USE_SIP
    void send_sip_invite(SIP_User_Agent *ua);
    void send_sip_bye(SIP_User_Agent *ua);
    void send_sip_message(SIP_User_Agent *ua, const char *data);
    void send_sip_notify(SIP_User_Agent *ua, SIP_Subscription_State state, SIP_Pidf_Xml_Basic_Element pidfXml);
    static int message_callback(SIP_Event_Callback event, SIP_User_Agent *ua, const char *remote);
#endif

    //static int read_data_set_thread(IEEE1451_NCAP_Application *ncap, IP::Address address, IEEE1451_TIM_Channel *tim);

    void execute() { IEEE1451_NCAP::get_instance()->execute(); };

private:
    Simple_List<TIM_Cache> _cache;
    Simple_List<IEEE1451_TEDS_Retriever> _retrievers;

    static IEEE1451_NCAP_Application *_application;
};


class IEEE1451_TEDS_Retriever
{
private:
    friend class IEEE1451_NCAP_Application;

    enum State
    {
        meta_teds = 0,
        tim_transducer_name_teds = 1,
        phy_teds = 2,
        transducer_channel_teds = 3,
        transducer_name_teds = 4
    };

public:
    IEEE1451_TEDS_Retriever(const IP::Address &address, IEEE1451_NCAP_Application *application);
    ~IEEE1451_TEDS_Retriever() {}

    //void repeat() { execute(); }
    void process(const char *message, unsigned int length);
    void execute();

private:
    IP::Address _address;
    IEEE1451_NCAP_Application *_application;
    IEEE1451_Transducer_Channel *_transducer;
    IEEE1451_TIM_Channel *_tim;

    short _state;
    char _teds_id;
    unsigned short _last_trans_id;

    Simple_List<IEEE1451_TEDS_Retriever>::Element _link;
};

//-------------------------------------------

IEEE1451_NCAP_Application *IEEE1451_NCAP_Application::_application = 0;

IEEE1451_NCAP_Application::IEEE1451_NCAP_Application()
{
    IEEE1451_NCAP::get_instance()->set_application(this);

#ifdef USE_SIP
    SIP_Manager::get_instance()->init();
    SIP_Manager::register_user_handler(message_callback);
#endif
}

IEEE1451_NCAP_Application::~IEEE1451_NCAP_Application()
{
    Simple_List<TIM_Cache>::Iterator it1 = _cache.begin();
    while (it1 != _cache.end())
    {
        Simple_List<TIM_Cache>::Element *el = it1++;
        TIM_Cache *tim_cache = el->object();
        _cache.remove(&tim_cache->_link);
        delete tim_cache;
    }

    Simple_List<IEEE1451_TEDS_Retriever>::Iterator it2 = _retrievers.begin();
    while (it2 != _retrievers.end())
    {
        Simple_List<IEEE1451_TEDS_Retriever>::Element *el = it2++;
        IEEE1451_TEDS_Retriever *retriever = el->object();
        _retrievers.remove(&retriever->_link);
        delete retriever->_tim; //This TIM is not in cache yet!
        delete retriever;
    }
}

IEEE1451_NCAP_Application *IEEE1451_NCAP_Application::get_instance()
{
    if (!_application)
        _application = new IEEE1451_NCAP_Application();
    return _application;
}

IEEE1451_NCAP_Application::TIM_Cache *IEEE1451_NCAP_Application::get_tim_cache(const IP::Address &address)
{
    Simple_List<TIM_Cache>::Iterator it = _cache.begin();
    while (it != _cache.end())
    {
        TIM_Cache *tim_cache = it->object();
        it++;

        if (tim_cache->_tim->_address == address)
            return tim_cache;
    }

    return 0;
}

#ifdef USE_SIP

IEEE1451_NCAP_Application::TIM_Cache *IEEE1451_NCAP_Application::get_tim_cache(const char *uri)
{
    Simple_List<TIM_Cache>::Iterator it = _cache.begin();
    while (it != _cache.end())
    {
        TIM_Cache *tim_cache = it->object();
        it++;

        if (!strcmp(tim_cache->_ua->get_uri(), uri))
            return tim_cache;
    }

    return 0;
}

#endif

IEEE1451_TEDS_Retriever *IEEE1451_NCAP_Application::get_retriever(unsigned short trans_id)
{
    Simple_List<IEEE1451_TEDS_Retriever>::Iterator it = _retrievers.begin();
    while (it != _retrievers.end())
    {
        IEEE1451_TEDS_Retriever *retriever = it->object();
        it++;

        if (retriever->_last_trans_id == trans_id)
            return retriever;
    }

    return 0;
}

void IEEE1451_NCAP_Application::update_tim(const IP::Address &address)
{
    IEEE1451_TEDS_Retriever *retriever = new IEEE1451_TEDS_Retriever(address, this);
    _retrievers.insert(&retriever->_link);
    retriever->execute();
}

void IEEE1451_NCAP_Application::update_tim_completed(IEEE1451_TEDS_Retriever *retriever, IEEE1451_TIM_Channel *tim, IP::Address address)
{
#ifdef USE_SIP
    char uri[100], remote[20], local[20];
    address.to_string(remote);
    ((IP::Address &) IP::instance(0)->address()).to_string(local);
    strcpy(uri, "sip:");
    strcat(uri, remote);
    strcat(uri, "@");
    strcat(uri, local);

    TIM_Cache *tim_cache = new TIM_Cache(tim, SIP_Manager::get_instance()->create_user_agent(uri));
#else
    TIM_Cache *tim_cache = new TIM_Cache(tim);
#endif

    _cache.insert(&tim_cache->_link);

    _retrievers.remove(&retriever->_link);
    delete retriever;

    report_tim_connected(address);

//#ifndef USE_SIP
//	new Thread(&IEEE1451_NCAP_Application::read_data_set_thread, this, address, (IEEE1451_TIM_Channel *) tim);
//#endif
}

void IEEE1451_NCAP_Application::report_tim_connected(const IP::Address &address)
{
    TIM_Cache *tim_cache = get_tim_cache(address);

    if (!tim_cache)
        update_tim(address);
    else
    {
        cout << "++ TIM CONNECTED (address=" << address << ") ++\n";
        tim_cache->_tim->_connected = true;

#ifdef USE_SIP
        if (tim_cache->_ua->has_subscription())
            send_sip_notify(tim_cache->_ua, SIP_SUBSCRIPTION_STATE_ACTIVE, SIP_PIDF_XML_OPEN);
#endif

        IEEE1451_Transducer_Channel *transducer = tim_cache->_tim->_transducer;
        send_operate(address, transducer->_channel_number);
    }
}

void IEEE1451_NCAP_Application::report_tim_disconnected(const IP::Address &address)
{
    TIM_Cache *tim_cache = get_tim_cache(address);
    if (tim_cache)
    {
        tim_cache->_tim->_connected = false;

#ifdef USE_SIP
        if (tim_cache->_ua->has_subscription())
            send_sip_notify(tim_cache->_ua, SIP_SUBSCRIPTION_STATE_ACTIVE, SIP_PIDF_XML_CLOSED);
#endif

        cout << "++ TIM DISCONNECTED (address=" << address << ") ++\n";
    }
}

/*void IEEE1451_NCAP_Application::report_error(unsigned short trans_id, int error_code)
{
    IEEE1451_TEDS_Retriever *retriever = get_retriever(trans_id);

    if (retriever)
        retriever->repeat();
    //else
    //  cout << "++ ERROR (" << trans_id << "): " << error_code << " ++\n";
 }*/

void IEEE1451_NCAP_Application::report_command_reply(const IP::Address &address, unsigned short trans_id, const char *message, unsigned int length)
{
    IEEE1451_TEDS_Retriever *retriever = get_retriever(trans_id);

    if (retriever)
    {
        IEEE1451_TEDS_Read_Reply *reply = (IEEE1451_TEDS_Read_Reply *) message;
        const char *buffer = message + sizeof(IEEE1451_TEDS_Read_Reply);

        if ((!reply->_header._success) || (reply->_header._length < sizeof(reply->_offset)))
        {
            cout << "Error in report command reply (teds)\n";
            //report_error(trans_id, 0);
            return;
        }

        cout << "++ REPLY RECEIVED (teds) (trans_id=" << trans_id << ", address=" << address << ", length=" << reply->_header._length << ") ++\n";

        retriever->process(buffer, length - sizeof(IEEE1451_TEDS_Read_Reply));
    }else
    {
        IEEE1451_Data_Set_Read_Reply *reply = (IEEE1451_Data_Set_Read_Reply *) message;
        const char *buffer = message + sizeof(IEEE1451_Data_Set_Read_Reply);
        unsigned short buffer_len = reply->_header._length - sizeof(reply->_offset);

        if ((!reply->_header._success) || (reply->_header._length < sizeof(reply->_offset)))
        {
            cout << "Error in report command reply (data set)\n";
            //report_error(trans_id, 0);
            return;
        }

        cout << "++ REPLY RECEIVED (data set) (trans_id=" << trans_id << ", address=" << address << ", length=" << reply->_header._length << ") ++\n";

        TIM_Cache *tim_cache = get_tim_cache(address);
        if (!tim_cache)
            return;

        IEEE1451_Transducer_Channel *transducer = tim_cache->_tim->_transducer;
        IEEE1451_TEDS_NCAP *channel = transducer->get_teds(TEDS_TRANSDUCER_CHANNEL);

        IEEE1451_TLV *phy = channel->get_tlv(TEDS_TRANSDUCER_CHANNEL_PHY_UNITS);
        IEEE1451_TEDS_NCAP aux_phy(0xff, phy->_value, phy->_length, true);

        IEEE1451_TLV *kelvin = aux_phy.get_tlv(TEDS_TRANSDUCER_CHANNEL_KELVINS);
        IEEE1451_TLV *audio = aux_phy.get_tlv(TEDS_TRANSDUCER_CHANNEL_AUDIOS);

        if (kelvin)
            read_temperature(address, buffer, buffer_len);
        else if (audio)
            read_audio(address, buffer, buffer_len);
    }
}

void IEEE1451_NCAP_Application::report_tim_initiated_message(const IP::Address &address, const char *message, unsigned int length)
{
    IEEE1451_Command *cmd = (IEEE1451_Command *) message;
    const char *buffer = message + sizeof(IEEE1451_Command);

    cout << "++ TIM MESSAGE RECEIVED (address=" << address << ") ++\n";

    if ((cmd->_channel_number == 1) && (cmd->_command == COMMAND_CLASS_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT))
    {
        TIM_Cache *tim_cache = get_tim_cache(address);
        if (!tim_cache)
            return;

        IEEE1451_Transducer_Channel *transducer = tim_cache->_tim->_transducer;
        IEEE1451_TEDS_NCAP *channel = transducer->get_teds(TEDS_TRANSDUCER_CHANNEL);

        IEEE1451_TLV *phy = channel->get_tlv(TEDS_TRANSDUCER_CHANNEL_PHY_UNITS);
        IEEE1451_TEDS_NCAP aux_phy(0xff, phy->_value, phy->_length, true);

        IEEE1451_TLV *kelvin = aux_phy.get_tlv(TEDS_TRANSDUCER_CHANNEL_KELVINS);
        IEEE1451_TLV *audio = aux_phy.get_tlv(TEDS_TRANSDUCER_CHANNEL_AUDIOS);

        if (kelvin)
            read_temperature(address, buffer, cmd->_length);
        else if (audio)
        {
#ifdef USE_SIP
            if ((cmd->_length == 1) && (buffer[0] == 1) && (!tim_cache->_ua->connected()))
                send_sip_invite(tim_cache->_ua);

            else if ((cmd->_length == 1) && (buffer[0] == 2) && (tim_cache->_ua->connected()))
            {
                send_sip_bye(tim_cache->_ua);
                tim_cache->_session_thread.suspend();
            }else
#endif
                read_audio(address, buffer, cmd->_length);
        }
    }
}

void IEEE1451_NCAP_Application::read_temperature(const IP::Address &address, const char *buffer, unsigned short length)
{
    TIM_Cache *tim_cache = get_tim_cache(address);
    if (!tim_cache)
        return;

    IEEE1451_Transducer_Channel *transducer = tim_cache->_tim->_transducer;
    IEEE1451_TEDS_NCAP *channel = transducer->get_teds(TEDS_TRANSDUCER_CHANNEL);

    IEEE1451_TLV *sample = channel->get_tlv(TEDS_TRANSDUCER_CHANNEL_SAMPLE);
    IEEE1451_TEDS_NCAP auxSample(0xff, sample->_value, sample->_length, true);

    IEEE1451_TLV *data_model = auxSample.get_tlv(TEDS_TRANSDUCER_CHANNEL_DAT_MODEL);
    unsigned short model = data_model->_value[0];

    if (model == 1)
    {
        IEEE1451_TLV *data_set = channel->get_tlv(TEDS_TRANSDUCER_CHANNEL_DATA_SET);
        IEEE1451_TEDS_NCAP aux_data_set(0xff, data_set->_value, data_set->_length, true);

        IEEE1451_TLV *max_data_repetitions = aux_data_set.get_tlv(TEDS_TRANSDUCER_CHANNEL_REPEATS);
        unsigned short repeats = (((unsigned char) max_data_repetitions->_value[0]) << 8) | ((unsigned char) max_data_repetitions->_value[1]);

        char data[50]; data[0] = 0;
        char aux[11];

        for (unsigned short i = 0, j = 0; i < repeats; i++, j += 4)
        {
            float value = *((float *)(&buffer[j]));

            itoa((int) value, aux);
            strcat(data, aux);
            if (i != (repeats - 1))
                strcat(data, ", ");
        }

        cout << "Read temperature (" << repeats << "): " << data << "\n";

#ifdef USE_SIP
        if (tim_cache->_ua->has_subscription())
            send_sip_message(tim_cache->_ua, data);
#endif
    }
}

void IEEE1451_NCAP_Application::read_audio(const IP::Address &address, const char *buffer, unsigned short length)
{
    TIM_Cache *tim_cache = get_tim_cache(address);
    if (!tim_cache)
        return;

    IEEE1451_Transducer_Channel *transducer = tim_cache->_tim->_transducer;
    IEEE1451_TEDS_NCAP *channel = transducer->get_teds(TEDS_TRANSDUCER_CHANNEL);

    IEEE1451_TLV *data_set = channel->get_tlv(TEDS_TRANSDUCER_CHANNEL_DATA_SET);
    IEEE1451_TEDS_NCAP aux_data_set(0xff, data_set->_value, data_set->_length, true);

    IEEE1451_TLV *max_data_repetitions = aux_data_set.get_tlv(TEDS_TRANSDUCER_CHANNEL_REPEATS);
    unsigned short repeats = (((unsigned char) max_data_repetitions->_value[0]) << 8) | ((unsigned char) max_data_repetitions->_value[1]);

    cout << "Read audio (" << repeats << ")\n";

#ifdef USE_SIP
    if ((tim_cache->_ua->has_subscription()) && (tim_cache->_ua->connected()))
        tim_cache->_send_rtp.send_data(tim_cache->_ua->get_subscriber(), 5000, buffer, repeats);
#endif
}

unsigned short IEEE1451_NCAP_Application::send_operate(const IP::Address &address, unsigned short channel_number)
{
    IEEE1451_NCAP *ncap = IEEE1451_NCAP::get_instance();
    char *cmd = ncap->create_command(channel_number, COMMAND_CLASS_TRANSDUCER_CHANNEL_OPERATE);
    unsigned short trans_id = ncap->send_command(address, cmd, sizeof(IEEE1451_Command));
    delete[] cmd;
    return trans_id;
}

unsigned short IEEE1451_NCAP_Application::send_idle(const IP::Address &address, unsigned short channel_number)
{
    IEEE1451_NCAP *ncap = IEEE1451_NCAP::get_instance();
    char *cmd = ncap->create_command(channel_number, COMMAND_CLASS_TRANSDUCER_CHANNEL_IDLE);
    unsigned short trans_id = ncap->send_command(address, cmd, sizeof(IEEE1451_Command));
    delete[] cmd;
    return trans_id;
}

unsigned short IEEE1451_NCAP_Application::send_read_teds(const IP::Address &address, unsigned short channel_number, char tedsId)
{
    IEEE1451_NCAP *ncap = IEEE1451_NCAP::get_instance();
    char *cmd = ncap->create_command(channel_number, COMMAND_CLASS_READ_TEDS_SEGMENT, &tedsId, 1);
    unsigned short trans_id = ncap->send_command(address, cmd, sizeof(IEEE1451_Command) + 1);
    delete[] cmd;
    return trans_id;
}

unsigned short IEEE1451_NCAP_Application::send_read_data_set(const IP::Address &address, unsigned short channel_number)
{
    IEEE1451_NCAP *ncap = IEEE1451_NCAP::get_instance();
    unsigned int offset = 0;
    char *cmd = ncap->create_command(channel_number, COMMAND_CLASS_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT, (char *) &offset, sizeof(offset));
    unsigned short trans_id = ncap->send_command(address, cmd, sizeof(IEEE1451_Command) + sizeof(offset));
    delete[] cmd;
    return trans_id;
}

int IEEE1451_NCAP_Application::send_read_multimedia_data_set_thread(IEEE1451_NCAP_Application *ncap, IP::Address address, unsigned short channel_number)
{
    cout << "== Send read multimedia data set thread (address=" << address << ") ==\n";
    Alarm::delay(TIME_50_MS);

#ifdef USE_SIP
    TIM_Cache *tim_cache = ncap->get_tim_cache(address);
#endif
    Thread::self()->suspend();

    while (1)
    {
#ifdef USE_SIP
        if (!tim_cache->_ua->connected())
            Thread::self()->suspend();
#endif

        //cout << "-- Reading Multimedia Data Set (address=" << address << ")...\n";
        ncap->send_read_data_set(address, channel_number);
        Alarm::delay(TIME_50_MS);
    }
    return 0;
}

#ifdef USE_SIP

void IEEE1451_NCAP_Application::send_sip_invite(SIP_User_Agent *ua)
{
    SIP_Request *invite = ua->create_invite(ua->get_subscriber());
    if (!invite)
        return;

    ua->send_request(invite);
}

void IEEE1451_NCAP_Application::send_sip_bye(SIP_User_Agent *ua)
{
    SIP_Request *bye = ua->create_bye(ua->get_subscriber());
    if (!bye)
        return;

    ua->send_request(bye);
}

void IEEE1451_NCAP_Application::send_sip_message(SIP_User_Agent *ua, const char *data)
{
    SIP_Request *message = ua->create_message(ua->get_subscriber(), data);
    if (!message)
        return;

    ua->send_request(message);
}

void IEEE1451_NCAP_Application::send_sip_notify(SIP_User_Agent *ua, SIP_Subscription_State state, SIP_Pidf_Xml_Basic_Element pidfXml)
{
    SIP_Request *notify = ua->create_notify(ua->get_subscriber(), state, pidfXml, 3600);
    if (!notify)
        return;

    ua->send_request(notify);
}

int IEEE1451_NCAP_Application::message_callback(SIP_Event_Callback event, SIP_User_Agent *ua, const char *remote)
{
    IEEE1451_NCAP_Application *app = IEEE1451_NCAP_Application::get_instance();
    TIM_Cache *tim_cache = app->get_tim_cache(ua->get_uri());
    if (!tim_cache)
    {
        cout << "++ Invalid User Agent ++\n";
        return 1;
    }

    switch (event)
    {
        case SIP_SESSION_INITIATED:
        {
            cout << "++ Session Initiated (" << tim_cache->_ua->get_call_status() << ") ++\n";
            if (tim_cache->_ua->get_call_status() == SIP_CALL_STATUS_INCOMING)
                tim_cache->_session_thread.resume();
            break;
        }

        case SIP_SESSION_TERMINATED:
        {
            cout << "++ Session Terminated ++\n";
            tim_cache->_session_thread.suspend();
            break;
        }

        case SIP_SUBSCRIPTION_INITIATED:
        {
            cout << "++ Subscription Initiated ++\n";

            if (tim_cache->_tim->_connected)
                app->send_sip_notify(ua, SIP_SUBSCRIPTION_STATE_ACTIVE, SIP_PIDF_XML_OPEN);
            else
                app->send_sip_notify(ua, SIP_SUBSCRIPTION_STATE_ACTIVE, SIP_PIDF_XML_CLOSED);
            break;
        }

        case SIP_SUBSCRIPTION_TERMINATED:
            cout << "++ Subscription Terminated ++\n";
            break;

        case SIP_MESSAGE_RECEIVED:
        {
            cout << "++ Message Received ++\n";

            if (!strncmp(ua->get_text_received(), "request data", 12))
                app->send_read_data_set(tim_cache->_tim->_address, 0x01);
            break;
        }

        default:
            break;
    }

    return 0;
}

#endif

/*int IEEE1451_NCAP_Application::read_data_set_thread(IEEE1451_NCAP_Application *ncap, IP::Address address, IEEE1451_TIM_Channel *tim)
{
    cout << "== NCAP request data thread created (address=" << address << ") ==\n";

    while (1)
    {
        Alarm::delay(TIME_500_MS * 100);
        if (tim->_connected)
        {
            cout << "-- Reading DataSet (address=" << address << ")...\n";
            ncap->send_read_data_set(address, 0x01);
        }else
            cout << "-- Address " << address << " is disconnected.\n";
    }
    return 0;
}*/

//-------------------------------------------

IEEE1451_TEDS_Retriever::IEEE1451_TEDS_Retriever(const IP::Address &address, IEEE1451_NCAP_Application *application) :
    _address(address), _application(application), _link(this)
{
    _transducer = new IEEE1451_Transducer_Channel;
    _transducer->_address = address;
    _transducer->_channel_number = 0x01; //It just supports one transducer per mote!

    _tim = new IEEE1451_TIM_Channel;
    _tim->_address = address;
    _tim->_transducer = _transducer;

    _state = meta_teds;
    _last_trans_id = 0;
}

void IEEE1451_TEDS_Retriever::process(const char *message, unsigned int length)
{
    IEEE1451_TEDS_NCAP *teds = new IEEE1451_TEDS_NCAP(_teds_id, message, length);

    switch (_state)
    {
        case meta_teds:
        case tim_transducer_name_teds:
        case phy_teds:
            _tim->add_teds(teds);
            break;

        case transducer_channel_teds:
        case transducer_name_teds:
            _transducer->add_teds(teds);
            break;

        default:
            delete teds;
            break;
    }

    _state++;
    execute();
}

void IEEE1451_TEDS_Retriever::execute()
{
    unsigned short channel_number = 0xff;

    switch (_state)
    {
        case meta_teds:
            cout << ">> Getting meta teds...\n";
            _teds_id = TEDS_META;
            channel_number = 0x00;
            break;

        case tim_transducer_name_teds:
            cout << ">> Getting tim's transducer name teds...\n";
            _teds_id = TEDS_USER_TRANSDUCER_NAME;
            channel_number = 0x00;
            break;

        case phy_teds:
            cout << ">> Getting phy teds...\n";
            _teds_id = TEDS_PHY;
            channel_number = 0x00;
            break;

        case transducer_channel_teds:
            cout << ">> Getting transducer channel teds...\n";
            _teds_id = TEDS_TRANSDUCER_CHANNEL;
            channel_number = 0x01;
            break;

        case transducer_name_teds:
            cout << ">> Getting transducer's transducer name teds...\n";
            _teds_id = TEDS_USER_TRANSDUCER_NAME;
            channel_number = 0x01;
            break;

        default:
            cout << ">> TEDS completed...\n";
            _teds_id = 0;
            _application->update_tim_completed(this, _tim, _address);
            return;
    }

    _last_trans_id = _application->send_read_teds(_address, channel_number, _teds_id);
}

//-------------------------------------------

int main()
{
    Alarm::delay(TIME_500_MS * 4);
    cout << "+++++ Starting ncap +++++\n";

    IP *ip = IP::instance();
    ip->set_address(IP::Address(10, 0, 0, 110));
    ip->set_gateway(IP::Address(10, 0, 0, 1));
    ip->set_netmask(IP::Address(255, 255, 255, 0));

    IEEE1451_NCAP_Application *app = IEEE1451_NCAP_Application::get_instance();
    app->execute();
    return 0;
}

