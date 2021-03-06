#include <ieee1451_tim.h>

#ifdef __NIC_H

__BEGIN_SYS

IEEE1451_Transducer::IEEE1451_Transducer()
{
    IEEE1451_TIM::get_instance()->set_transducer(this);
}

void IEEE1451_Transducer::receive_msg(unsigned short trans_id, const char *message, unsigned int size)
{
    IEEE1451_Command *cmd = (IEEE1451_Command *) message;
    const char *buffer = message + sizeof(IEEE1451_Command);

    //db<IEEE1451_Transducer>(TRC) << "IEEE1451_Transducer - Message received (trans_id=" << trans_id << ", cmd=" << hex << cmd->_command << ")\n";

    switch (cmd->_command)
    {
        /*case COMMAND_CLASS_QUERY_TEDS:
         case COMMAND_CLASS_UPDATE_TEDS:
         {
             IEEE1451_TEDS_TIM *teds = get_teds(buffer[0]);
             char *reply = IEEE1451_TIM::get_instance()->get_send_buffer();

             IEEE1451_TEDS_Query_Reply *query_reply = (IEEE1451_TEDS_Query_Reply *) reply;
             query_reply->_header._length = 12;
             if (teds)
             {
                 query_reply->_max_size = 0;
                 query_reply->_checksum = *((unsigned short *) teds->_payload);
                 query_reply->_size = teds->_size;
                 query_reply->_status = teds->_status;
                 query_reply->_atributes = teds->_atributes;
                 query_reply->_header._success = true;
             }else
             {
                 query_reply->_max_size = 0;
                 query_reply->_checksum = 0;
                 query_reply->_size = 0;
                 query_reply->_status = 0;
                 query_reply->_atributes = 0x40;
                 query_reply->_header._success = false;
             }

             IEEE1451_TIM::get_instance()->send_msg(trans_id, sizeof(IEEE1451_TEDS_Query_Reply));
             break;
         }*/

        case COMMAND_CLASS_READ_TEDS_SEGMENT:
        {
            IEEE1451_TEDS_TIM *teds = get_teds(buffer[0]);
            if (teds)
            {
                unsigned int reply_size = sizeof(IEEE1451_TEDS_Read_Reply) + teds->_size;
                char *reply = IEEE1451_TIM::get_instance()->get_send_buffer();

                IEEE1451_TEDS_Read_Reply *read_reply = (IEEE1451_TEDS_Read_Reply *) reply;
                read_reply->_header._success = true;
                read_reply->_header._length = teds->_size + sizeof(read_reply->_offset);
                read_reply->_offset = 0;

                const char *teds_payload = teds->_payload;
                for (unsigned short i = 0; i < teds->_size; i++)
                    reply[i + sizeof(IEEE1451_TEDS_Read_Reply)] = teds_payload[i];

                IEEE1451_TIM::get_instance()->send_msg(trans_id, reply_size);
            }else
            {
                char *reply = IEEE1451_TIM::get_instance()->get_send_buffer();

                IEEE1451_TEDS_Read_Reply *read_reply = (IEEE1451_TEDS_Read_Reply *) reply;
                read_reply->_header._success = false;
                read_reply->_header._length = sizeof(read_reply->_offset);
                read_reply->_offset = 0;

                IEEE1451_TIM::get_instance()->send_msg(trans_id, sizeof(IEEE1451_TEDS_Read_Reply));
            }

            break;
        }

        case COMMAND_CLASS_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT:
        {
            unsigned int offset = *((unsigned int *) buffer);
            read_data_set(trans_id, offset);
            break;
        }

        case COMMAND_CLASS_TRANSDUCER_CHANNEL_OPERATE:
            start();
            break;

        case COMMAND_CLASS_TRANSDUCER_CHANNEL_IDLE:
            stop();
            break;

        case COMMAND_CLASS_START_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT:
            start_read_data_set(trans_id);
            break;

        case COMMAND_CLASS_STOP_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT:
            stop_read_data_set(trans_id);
            break;

        default:
            db<IEEE1451_Transducer>(INF) << "IEEE1451_Transducer - Received invalid message\n";
            break;
    }
}

//-------------------------------------------

IEEE1451_TIM *IEEE1451_TIM::_ieee1451 = 0;

IEEE1451_TIM::IEEE1451_TIM() :
    _ncap_address((unsigned long) 0x0a00000b),
    _udp_socket(UDP::Address(IP::instance()->address(), IEEE1451_PORT),
                UDP::Address(Traits<IP>::BROADCAST, IEEE1451_PORT))
{
#ifdef __mc13224v__
    MC13224V_Transceiver::maca_off();
#endif

    _receive_thread = 0;
    _connected = false;
    _transducer = 0;

    _tcp_channel.bind(IEEE1451_PORT);
    //_udp_socket.remote(UDP::Address(_ncap_address, IEEE1451_PORT));
    _send_buffer = new (kmalloc(MAX_BUFFER_SIZE)) char[MAX_BUFFER_SIZE];

    init_teds();
}

IEEE1451_TIM::~IEEE1451_TIM()
{
    if (_send_buffer)
        kfree(_send_buffer);

    if (_receive_thread)
        delete _receive_thread;

    if (_meta_array)
        kfree(_meta_array);
    if (_tim_utn_array)
        kfree(_tim_utn_array);
    if (_phy_array)
        kfree(_phy_array);

    if (_meta_teds)
        delete _meta_teds;
    if (_tim_utn_teds)
        delete _tim_utn_teds;
    if (_phy_teds)
        delete _phy_teds;
}

IEEE1451_TIM *IEEE1451_TIM::get_instance()
{
    if (!_ieee1451)
        _ieee1451 = new IEEE1451_TIM();
    return _ieee1451;
}

void IEEE1451_TIM::init_teds()
{
    _meta_array = new (kmalloc(40)) char[40];
    _meta_array[0] = 0x00; _meta_array[1] = 0x00; _meta_array[2] = 0x00; _meta_array[3] = 0x24; _meta_array[4] = TEDS_META_TEDS_ID; _meta_array[5] = 0x04; _meta_array[6] = 0x00; _meta_array[7] = TEDS_META; _meta_array[8] = 0x01; _meta_array[9] = 0x01; _meta_array[10] = TEDS_META_UUID; _meta_array[11] = 0x0a; _meta_array[12] = 0x00; _meta_array[13] = 0x00; _meta_array[14] = 0x00; _meta_array[15] = 0x00; _meta_array[16] = 0x00; _meta_array[17] = 0x00; _meta_array[18] = 0x00; _meta_array[19] = 0x00; _meta_array[20] = 0x00; _meta_array[21] = 0x00; _meta_array[22] = TEDS_META_O_HOLD_OFF; _meta_array[23] = 0x04; _meta_array[24] = 0x41; _meta_array[25] = 0x20; _meta_array[26] = 0x00; _meta_array[27] = 0x00; _meta_array[28] = TEDS_META_TEST_TIME; _meta_array[29] = 0x04; _meta_array[30] = 0x00; _meta_array[31] = 0x00; _meta_array[32] = 0x00; _meta_array[33] = 0x00; _meta_array[34] = TEDS_META_MAX_CHAN; _meta_array[35] = 0x02; _meta_array[36] = 0x00; _meta_array[37] = 0x01; _meta_array[38] = 0xfe; _meta_array[39] = 0xb6; //checksum errado
    _meta_teds = new IEEE1451_TEDS_TIM(_meta_array, 40);

    _tim_utn_array = new (kmalloc(26)) char[26];
    _tim_utn_array[0] = 0x00; _tim_utn_array[1] = 0x00; _tim_utn_array[2] = 0x00; _tim_utn_array[3] = 0x16; _tim_utn_array[4] = TEDS_USER_TRANSDUCER_NAME_TEDS_ID; _tim_utn_array[5] = 0x04; _tim_utn_array[6] = 0x00; _tim_utn_array[7] = TEDS_USER_TRANSDUCER_NAME; _tim_utn_array[8] = 0x01; _tim_utn_array[9] = 0x01; _tim_utn_array[10] = TEDS_USER_TRANSDUCER_NAME_FORMAT; _tim_utn_array[11] = 0x01; _tim_utn_array[12] = 0x00; _tim_utn_array[13] = TEDS_USER_TRANSDUCER_NAME_TC_NAME; _tim_utn_array[14] = 0x09; _tim_utn_array[15] = 0x45; _tim_utn_array[16] = 0x50; _tim_utn_array[17] = 0x4f; _tim_utn_array[18] = 0x53; _tim_utn_array[19] = 0x2f; _tim_utn_array[20] = 0x4d; _tim_utn_array[21] = 0x6f; _tim_utn_array[22] = 0x74; _tim_utn_array[23] = 0x65; _tim_utn_array[24] = 0xff; _tim_utn_array[25] = 0xff; //checksum errado
    _tim_utn_teds = new IEEE1451_TEDS_TIM(_tim_utn_array, 26);

    _phy_array = new (kmalloc(25)) char[25];
    _phy_array[0] = 0x00; _phy_array[1] = 0x00; _phy_array[2] = 0x00; _phy_array[3] = 0x15; _phy_array[4] = TEDS_PHY_TEDS_ID; _phy_array[5] = 0x04; _phy_array[6] = 0x05; _phy_array[7] = TEDS_PHY; _phy_array[8] = 0x01; _phy_array[9] = 0x01; _phy_array[10] = TEDS_PHY_RADIO; _phy_array[11] = 0x01; _phy_array[12] = 0xff; _phy_array[13] = TEDS_PHY_MAX_C_DEV; _phy_array[14] = 0x02; _phy_array[15] = 0x00; _phy_array[16] = 0x01; _phy_array[17] = TEDS_PHY_BATTERY; _phy_array[18] = 0x01; _phy_array[19] = 0x01; _phy_array[20] = TEDS_PHY_RECONNECT; _phy_array[21] = 0x01; _phy_array[22] = 0x01; _phy_array[23] = 0xfe; _phy_array[24] = 0x74; //checksum errado
    _phy_teds = new IEEE1451_TEDS_TIM(_phy_array, 25);
}

IEEE1451_TEDS_TIM *IEEE1451_TIM::get_teds(char id)
{
    if (id == TEDS_META)
        return _meta_teds;
    else if (id == TEDS_USER_TRANSDUCER_NAME)
        return _tim_utn_teds;
    else if (id == TEDS_PHY)
        return _phy_teds;
    return 0;
}

void IEEE1451_TIM::connect()
{
#ifdef __mc13224v__
    MC13224V_Transceiver::maca_on();
#endif

    db<IEEE1451_TIM>(TRC) << "IEEE1451_TIM - Connecting...\n";

    TCP::Address addr(_ncap_address, IEEE1451_PORT);

    while (!_tcp_channel.connect(addr))
        Alarm::delay(TIME_500_MS * 10);

    _connected = true;
    _connected_cond.signal();

    db<IEEE1451_TIM>(TRC) << "IEEE1451_TIM - Connected!\n";
}

void IEEE1451_TIM::disconnect()
{
    db<IEEE1451_TIM>(TRC) << "IEEE1451_TIM - Disconnecting...\n";

    _connected = false;
    Thread::self()->yield();

    if (!_tcp_channel.close())
    {
        db<IEEE1451_TIM>(WRN) << "IEEE1451_TIM - Error when disconnecting...\n";
        _tcp_channel.state(TCP::Socket::CLOSED);
    }

#ifdef __mc13224v__
    MC13224V_Transceiver::maca_off();
#endif

    db<IEEE1451_TIM>(TRC) << "IEEE1451_TIM - Disconnected!\n";
}

void IEEE1451_TIM::receive_msg(unsigned short trans_id, const char *message, unsigned int size)
{
    IEEE1451_Command *cmd = (IEEE1451_Command *) message;
    const char *buffer = message + sizeof(IEEE1451_Command);

    //db<IEEE1451_TIM>(TRC) << "IEEE1451_TIM - Message received (trans_id=" << trans_id << ", cmd=" << hex << cmd->_command << ")\n";

    if (cmd->_channel_number == ADDRESS_CLASS_TIM)
    {
        switch (cmd->_command)
        {
            /*case COMMAND_CLASS_QUERY_TEDS:
             case COMMAND_CLASS_UPDATE_TEDS:
             {
                 IEEE1451_TEDS_TIM *teds = get_teds(buffer[0]);
                 char *reply = get_send_buffer();

                 IEEE1451_TEDS_Query_Reply *query_reply = (IEEE1451_TEDS_Query_Reply *) reply;
                 query_reply->_header._length = 12;
                 if (teds)
                 {
                     query_reply->_max_size = 0;
                     query_reply->_checksum = *((unsigned short *) teds->_payload);
                     query_reply->_size = teds->_size;
                     query_reply->_status = teds->_status;
                     query_reply->_atributes = teds->_atributes;
                     query_reply->_header._success = true;
                 }else
                 {
                     query_reply->_max_size = 0;
                     query_reply->_checksum = 0;
                     query_reply->_size = 0;
                     query_reply->_status = 0;
                     query_reply->_atributes = 0x40;
                     query_reply->_header._success = false;
                 }

                 send_msg(trans_id, sizeof(IEEE1451_TEDS_Query_Reply));
                 break;
             }*/

            case COMMAND_CLASS_READ_TEDS_SEGMENT:
            {
                IEEE1451_TEDS_TIM *teds = get_teds(buffer[0]);
                if (teds)
                {
                    unsigned int reply_size = sizeof(IEEE1451_TEDS_Read_Reply) + teds->_size;
                    char *reply = get_send_buffer();

                    IEEE1451_TEDS_Read_Reply *read_reply = (IEEE1451_TEDS_Read_Reply *) reply;
                    read_reply->_header._success = true;
                    read_reply->_header._length = teds->_size + sizeof(read_reply->_offset);
                    read_reply->_offset = 0;

                    const char *teds_payload = teds->_payload;
                    for (unsigned short i = 0; i < teds->_size; i++)
                        reply[i + sizeof(IEEE1451_TEDS_Read_Reply)] = teds_payload[i];

                    send_msg(trans_id, reply_size);
                }else
                {
                    char *reply = get_send_buffer();

                    IEEE1451_TEDS_Read_Reply *read_reply = (IEEE1451_TEDS_Read_Reply *) reply;
                    read_reply->_header._success = false;
                    read_reply->_header._length = sizeof(read_reply->_offset);
                    read_reply->_offset = 0;

                    send_msg(trans_id, sizeof(IEEE1451_TEDS_Read_Reply));
                }
                break;
            }

            default:
                db<IEEE1451_TIM>(INF) << "IEEE1451_TIM - Received invalid message\n";
                break;
        }
    }else if (((cmd->_channel_number >= ADDRESS_CLASS_TRANSDUCER_CHANNEL_FIRST) && (cmd->_channel_number <= ADDRESS_CLASS_TRANSDUCER_CHANNEL_LAST)) ||
               (cmd->_channel_number == ADDRESS_CLASS_GLOBAL))
    {
        _transducer->receive_msg(trans_id, message, size);

    }else if ((cmd->_channel_number >= ADDRESS_CLASS_GROUP_FIRST) && (cmd->_channel_number <= ADDRESS_CLASS_GROUP_LAST))
    {
        //group
    }
}

void IEEE1451_TIM::send_msg(unsigned short trans_id, unsigned int length, bool multimedia)
{
    //db<IEEE1451_TIM>(TRC) << "IEEE1451_TIM - Sending message (trans_id=" << trans_id << ", len=" << length << ", media=" << multimedia << ")\n";

    if (!_connected)
    {
        db<IEEE1451_TIM>(INF) << "IEEE1451_TIM - TIM not connected (trans_id=" << trans_id << ")\n";
        _send_buffer_mutex.unlock();
        return;
    }

    unsigned int size = sizeof(IEEE1451_Packet) + length;

    IEEE1451_Packet *out = (IEEE1451_Packet *) _send_buffer;

    out->_trans_id = trans_id;
    out->_length = length;

    int ret;
    if (multimedia)
    {
        _udp_socket.remote(UDP::Address(_ncap_address, IEEE1451_PORT));
        ret = _udp_socket.send(_send_buffer, size);
    }else
        ret = _tcp_channel.send(_send_buffer, size);

    if (ret < 0)
        db<IEEE1451_TIM>(INF) << "IEEE1451_TIM - Failed sending message (trans_id=" << trans_id << ", ret=" << ret << ")\n";
    //else
    //  db<IEEE1451_TIM>(TRC) << "IEEE1451_TIM - Sent " << ret << " bytes (trans_id=" << trans_id << ")\n";

    _send_buffer_mutex.unlock();
}

int IEEE1451_TIM::receive(IEEE1451_TIM *tim, TCP::Channel *channel)
{
    db<IEEE1451_TIM>(TRC) << "IEEE1451_TIM - Receive thread created (ip=" << channel->remote().ip() << ")\n";

    char *_receive_buffer = new (kmalloc(MAX_BUFFER_SIZE)) char[MAX_BUFFER_SIZE];
    IEEE1451_Packet *in;
    const char *msg;
    int ret;

    while (true)
    {
        if (!tim->_connected)
        {
            db<IEEE1451_TIM>(TRC) << "IEEE1451_TIM - TIM not connected!\n";
            tim->_connected_cond.wait();
            continue;
        }

        //db<IEEE1451_TIM>(TRC) << "IEEE1451_TIM - Receiving...\n";
        ret = channel->receive(_receive_buffer, MAX_BUFFER_SIZE);

        if (ret < (int) sizeof(IEEE1451_Packet))
            continue;

        in = (IEEE1451_Packet *) _receive_buffer;
        msg = _receive_buffer + sizeof(IEEE1451_Packet);

        if (in->_length > 0)
            tim->receive_msg(in->_trans_id, msg, in->_length);
    }

    db<IEEE1451_TIM>(TRC) << "IEEE1451_TIM - Receive thread finished (ip=" << channel->remote().ip() << ")\n";
    kfree(_receive_buffer);
    return 0;
}

void IEEE1451_TIM::UDP_Socket::received(const UDP::Address &src, const char *data, unsigned int size)
{
    if (size < sizeof(IEEE1451_Packet))
        return;

    IEEE1451_Packet *in = (IEEE1451_Packet *) data;
    const char *msg = data + sizeof(IEEE1451_Packet);

    if (in->_length > 0)
        IEEE1451_TIM::get_instance()->receive_msg(in->_trans_id, msg, in->_length);
}

__END_SYS

#endif
