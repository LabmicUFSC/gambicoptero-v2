#include <ieee1451_ncap.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

__BEGIN_SYS

//-------------------------------------------

IEEE1451_TEDS_NCAP::IEEE1451_TEDS_NCAP(char id, const char *teds, unsigned short length, bool sub_block) :
    _id(id), _link(this)
{
    unsigned short begin, end;

    if (sub_block)
    {
        begin = 0;
        end = length;
    }else
    {
        begin = 4;
        end = length - 2;
    }

    for (unsigned short i = begin; i < end; )
    {
        unsigned short type = teds[i++] & 0xff;
        unsigned short len = teds[i++] & 0xff;

        char *value = new char[len];
        for (unsigned short j = 0; j < len; j++, i++)
            value[j] = teds[i];

        IEEE1451_TLV *tlv = new IEEE1451_TLV(type, len, value);
        _tlvs.insert(&tlv->_link);
    }
}

IEEE1451_TEDS_NCAP::~IEEE1451_TEDS_NCAP()
{
    Simple_List<IEEE1451_TLV>::Iterator it = _tlvs.begin();
    while (it != _tlvs.end())
    {
        Simple_List<IEEE1451_TLV>::Element *el = it++;
        IEEE1451_TLV *tlv = el->object();
        _tlvs.remove(&tlv->_link);
        delete tlv;
    }
}

IEEE1451_TLV *IEEE1451_TEDS_NCAP::get_tlv(char type)
{
    Simple_List<IEEE1451_TLV>::Iterator it = _tlvs.begin();
    while (it != _tlvs.end())
    {
        IEEE1451_TLV *tlv = it->object();
        it++;

        if (tlv->_type == type)
            return tlv;
    }

    return 0;
}

//-------------------------------------------

IEEE1451_Channel::~IEEE1451_Channel()
{
    Simple_List<IEEE1451_TEDS_NCAP>::Iterator it = _teds.begin();
    while (it != _teds.end())
    {
        Simple_List<IEEE1451_TEDS_NCAP>::Element *el = it++;
        IEEE1451_TEDS_NCAP *teds = el->object();
        _teds.remove(&teds->_link);
        delete teds;
    }
}

IEEE1451_TEDS_NCAP *IEEE1451_Channel::get_teds(char id)
{
    Simple_List<IEEE1451_TEDS_NCAP>::Iterator it = _teds.begin();
    while (it != _teds.end())
    {
        IEEE1451_TEDS_NCAP *teds = it->object();
        it++;

        if (teds->_id == id)
            return teds;
    }

    return 0;
}

//-------------------------------------------

IEEE1451_NCAP *IEEE1451_NCAP::_ieee1451 = 0;

IEEE1451_NCAP::~IEEE1451_NCAP()
{
    Simple_List<Linked_Channel>::Iterator it = _channels.begin();
    while (it != _channels.end())
    {
        Simple_List<Linked_Channel>::Element *el = it++;
        Linked_Channel *chn = el->object();
        _channels.remove(&chn->_link);
        delete chn;
    }
}

IEEE1451_NCAP *IEEE1451_NCAP::get_instance()
{
    if (!_ieee1451)
        _ieee1451 = new IEEE1451_NCAP();
    return _ieee1451;
}

char *IEEE1451_NCAP::create_command(unsigned short channel_number, unsigned short command, const char *args, unsigned int length)
{
    db<IEEE1451_NCAP>(TRC) << "IEEE1451_NCAP - Creating command (type=" << hex << command << ")\n";

    unsigned int size = sizeof(IEEE1451_Command) + length;
    char *buffer = new char[size];

    IEEE1451_Command *cmd = (IEEE1451_Command *) buffer;
    char *msg = buffer + sizeof(IEEE1451_Command);

    cmd->_channel_number = channel_number;
    cmd->_command = command;
    cmd->_length = length;
    memcpy(msg, args, length);

    return buffer;
}

unsigned short IEEE1451_NCAP::send_command(const IP::Address &destination, const char *message, unsigned int length)
{
    static unsigned int id_generator = 1;
    unsigned short trans_id = id_generator++;

    db<IEEE1451_NCAP>(TRC) << "IEEE1451_NCAP - Sending command (trans_id=" << trans_id << ", dst=" << destination << ")\n";

    Linked_Channel *channel = get_channel(destination);
    if (!channel)
    {
        db<IEEE1451_NCAP>(INF) << "IEEE1451_NCAP - Failed to send command (trans_id=" << trans_id << ")\n";
        return 0;
    }

    unsigned int size = sizeof(IEEE1451_Packet) + length;
    char *buffer = new char[size];

    IEEE1451_Packet *out = (IEEE1451_Packet *) buffer;
    char *msg = buffer + sizeof(IEEE1451_Packet);

    out->_trans_id = trans_id;
    out->_length = length;
    memcpy(msg, message, length);

    int ret = channel->send(buffer, size);
    if (ret < 0)
        db<IEEE1451_NCAP>(INF) << "IEEE1451_NCAP - Failed sending message (trans_id=" << trans_id << ", ret=" << ret << ")\n";
    else
        db<IEEE1451_NCAP>(TRC) << "IEEE1451_NCAP - Sent " << ret << " bytes (trans_id=" << trans_id << ")\n";

    return trans_id;
}

Linked_Channel *IEEE1451_NCAP::get_channel(const IP::Address &addr)
{
    Simple_List<Linked_Channel>::Iterator it = _channels.begin();
    while (it != _channels.end())
    {
        Linked_Channel *channel = it->object();
        it++;

        if (channel->remote().ip() == addr)
            return channel;
    }

    return 0;
}

void IEEE1451_NCAP::execute()
{
    db<IEEE1451_NCAP>(INF) << "IEEE1451_NCAP - Executing...\n";

    Linked_Channel *channel = new Linked_Channel();
    channel->bind(IEEE1451_PORT);

    while (true)
    {
        if (!channel->listen())
            continue;

        db<IEEE1451_NCAP>(TRC) << "IEEE1451_NCAP - New channel connected (ip=" << channel->remote().ip() << ")\n";

        Simple_List<Linked_Channel>::Iterator it = _channels.begin();
        while (it != _channels.end())
        {
            Linked_Channel *chn = it->object();
            it++;

            if ((TCP::Address) channel->remote() == (TCP::Address) chn->remote())
            {
                db<IEEE1451_NCAP>(TRC) << "IEEE1451_NCAP - Deleting old channel...\n";
                _channels.remove(&chn->_link);
                delete chn;
            }
        }

        _channels.insert(&channel->_link);
        new Thread(receive, this, channel);

        channel = new Linked_Channel();
        channel->bind(IEEE1451_PORT);
    }
}

int IEEE1451_NCAP::receive(IEEE1451_NCAP *ncap, Linked_Channel *channel)
{
    db<IEEE1451_NCAP>(TRC) << "IEEE1451_NCAP - Receive thread created (ip=" << channel->remote().ip() << ")\n";

    char data[200];
    unsigned int size = 200;
    int ret;

    ncap->_application->report_tim_connected(channel->remote().ip());

    while ((ret = channel->receive(data, size)) >= 0)
    {
        if (ret < (int) sizeof(IEEE1451_Packet))
            continue;

        IEEE1451_Packet *in = (IEEE1451_Packet *) data;
        const char *msg = data + sizeof(IEEE1451_Packet);

        if (in->_length > 0)
        {
            if (in->_trans_id == 0)
                ncap->_application->report_tim_initiated_message(channel->remote().ip(), msg, in->_length);
            else
                ncap->_application->report_command_reply(channel->remote().ip(), in->_trans_id, msg, in->_length);
        }
    }

    db<IEEE1451_NCAP>(TRC) << "IEEE1451_NCAP - Closing channel (ip=" << channel->remote().ip() << ")...\n";

    while (!channel->close())
        Alarm::delay(TIME_500_MS);

    db<IEEE1451_NCAP>(TRC) << "IEEE1451_NCAP - Channel closed (ip=" << channel->remote().ip() << ")\n";

    ncap->_application->report_tim_disconnected(channel->remote().ip());

    db<IEEE1451_TIM>(TRC) << "IEEE1451_NCAP - Receive thread finished (ip=" << channel->remote().ip() << ")\n";

    ncap->_channels.remove(&channel->_link);
    delete channel;
    delete Thread::self();
    return 0;
}

__END_SYS
