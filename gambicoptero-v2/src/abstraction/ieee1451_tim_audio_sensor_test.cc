#include <system/config.h>

#ifdef __NIC_H

#include <ieee1451_tim.h>
#include <thread.h>
#include <g726.h>

#ifdef __mc13224v__
    #include <mach/mc13224v/audio_sensor.h>
#endif

#define DATASET_SIZE    73 //27 pacotes/seg * (73 * 8 / 2) amostras/pacotes = 7884 amostras/seg
#define SAMPLE_NO       DATASET_SIZE * 8 / 2 //2 bits per sample (16 kbit/s)

__USING_SYS

OStream cout;

enum Operation_Mode
{
    OM_POLLING                  = 0,
    OM_POLLING_OPTIMIZED        = 1,
    OM_TIM_IM                   = 2,
    OM_TIM_IM_OPTIMIZED         = 3,
};

class IEEE1451_Audio_Sensor : public IEEE1451_Transducer
{
public:
    IEEE1451_Audio_Sensor(Operation_Mode operation_mode);
    virtual ~IEEE1451_Audio_Sensor();

protected:
    void init_teds();
    IEEE1451_TEDS_TIM *get_teds(char id);
    //bool running() { return run; };
    void start();
    void stop();
    void read_data_set(unsigned short trans_id, unsigned int offset);
    void start_read_data_set(unsigned short trans_id);
    void stop_read_data_set(unsigned short trans_id);
    void send_data_set();
    void send_start_read_data_set();
    void send_stop_read_data_set();

    void get_audio();
    void pack_audio(char *data);

public:
    int execute();

private:
    unsigned short _channel_number;
    Operation_Mode _operation_mode;

#ifdef __mc13224v__
    Audio_Sensor _audio;
#endif

    short *_audio_buffer;
    unsigned short _pos;

    char _law;
    short _rate;
    short _reset;
    G726_state _state;
    unsigned short _counter;

    char _snd_buffer[DATASET_SIZE];

    Thread *_execute_thread;
    bool _reading_data;

    char *_channel_array;
    char *_audio_sensor_utn_array;
    char *_units_extension_array;
    IEEE1451_TEDS_TIM *_channel_teds;
    IEEE1451_TEDS_TIM *_audio_sensor_utn_teds;
    IEEE1451_TEDS_TIM *_units_extension_teds;
};

//-------------------------------------------

IEEE1451_Audio_Sensor::IEEE1451_Audio_Sensor(Operation_Mode operation_mode)
{
    _channel_number = 0x01; //It just supports one transducer per mote!
    _operation_mode = operation_mode;
    _execute_thread = 0;
    _reading_data = false;

    _audio_buffer = new (kmalloc(SAMPLE_NO * sizeof(short))) short[SAMPLE_NO * sizeof(short)];
    _law = '2'; //PCM
    _rate = 2; //2 bits per sample (16 kbit/s)
    _reset = 1; //Yes
    _counter = 0;

    init_teds();
}

IEEE1451_Audio_Sensor::~IEEE1451_Audio_Sensor()
{
    if (_execute_thread)
        delete _execute_thread;

    if (_channel_array)
        kfree(_channel_array);
    if (_audio_sensor_utn_array)
        kfree(_audio_sensor_utn_array);
    if (_units_extension_array)
        kfree(_units_extension_array);

    if (_channel_teds)
        delete _channel_teds;
    if (_audio_sensor_utn_teds)
        delete _audio_sensor_utn_teds;
    if (_units_extension_teds)
        delete _units_extension_teds;

    if (_audio_buffer)
        kfree(_audio_buffer);
}

void IEEE1451_Audio_Sensor::init_teds()
{
    _channel_array = new (kmalloc(123)) char[123];
    _channel_array[0] = 0x00; _channel_array[1] = 0x00; _channel_array[2] = 0x00; _channel_array[3] = 0x77; _channel_array[4] = TEDS_TRANSDUCER_CHANNEL_TEDS_ID; _channel_array[5] = 0x04; _channel_array[6] = 0x00; _channel_array[7] = TEDS_TRANSDUCER_CHANNEL; _channel_array[8] = 0x01; _channel_array[9] = 0x01; _channel_array[10] = TEDS_TRANSDUCER_CHANNEL_CAL_KEY; _channel_array[11] = 0x01; _channel_array[12] = 0x00; _channel_array[13] = TEDS_TRANSDUCER_CHANNEL_CHAN_TYPE; _channel_array[14] = 0x01; _channel_array[15] = 0x00; _channel_array[16] = TEDS_TRANSDUCER_CHANNEL_PHY_UNITS; _channel_array[17] = 0x06; _channel_array[18] = TEDS_TRANSDUCER_CHANNEL_UNIT_TYPE; _channel_array[19] = 0x01; _channel_array[20] = 0x00; _channel_array[21] = TEDS_TRANSDUCER_CHANNEL_AUDIO_STREAM; _channel_array[22] = 0x01; _channel_array[23] = 0x82; _channel_array[24] = TEDS_TRANSDUCER_CHANNEL_LOW_LIMIT; _channel_array[25] = 0x04; _channel_array[26] = 0x43; _channel_array[27] = 0x69; _channel_array[28] = 0x00; _channel_array[29] = 0x00; _channel_array[30] = TEDS_TRANSDUCER_CHANNEL_HI_LIMIT; _channel_array[31] = 0x04; _channel_array[32] = 0x43; _channel_array[33] = 0xb0; _channel_array[34] = 0x80; _channel_array[35] = 0x00; _channel_array[36] = TEDS_TRANSDUCER_CHANNEL_O_ERROR; _channel_array[37] = 0x04; _channel_array[38] = 0x3f; _channel_array[39] = 0x00; _channel_array[40] = 0x00; _channel_array[41] = 0x00; _channel_array[42] = TEDS_TRANSDUCER_CHANNEL_SELF_TEST; _channel_array[43] = 0x01; _channel_array[44] = 0x00; _channel_array[45] = TEDS_TRANSDUCER_CHANNEL_SAMPLE; _channel_array[46] = 0x09; _channel_array[47] = TEDS_TRANSDUCER_CHANNEL_DAT_MODEL; _channel_array[48] = 0x01; _channel_array[49] = 0x00; _channel_array[50] = TEDS_TRANSDUCER_CHANNEL_MOD_LENGTH; _channel_array[51] = 0x01; _channel_array[52] = 0x01; _channel_array[53] = TEDS_TRANSDUCER_CHANNEL_SIG_BITS; _channel_array[54] = 0x01; _channel_array[55] = 0x08; _channel_array[56] = TEDS_TRANSDUCER_CHANNEL_DATA_SET; _channel_array[57] = 0x1c; _channel_array[58] = TEDS_TRANSDUCER_CHANNEL_REPEATS; _channel_array[59] = 0x02; _channel_array[60] = (SAMPLE_NO >> 8) & 0xff; _channel_array[61] = SAMPLE_NO & 0xff; _channel_array[62] = TEDS_TRANSDUCER_CHANNEL_S_ORIGIN; _channel_array[63] = 0x04; _channel_array[64] = 0x00; _channel_array[65] = 0x00; _channel_array[66] = 0x00; _channel_array[67] = 0x00; _channel_array[68] = TEDS_TRANSDUCER_CHANNEL_STEP_SIZE; _channel_array[69] = 0x04; _channel_array[70] = 0x41; _channel_array[71] = 0x20; _channel_array[72] = 0x00; _channel_array[73] = 0x00; _channel_array[74] = TEDS_TRANSDUCER_CHANNEL_S_UNITS; _channel_array[75] = 0x06; _channel_array[76] = TEDS_TRANSDUCER_CHANNEL_UNIT_TYPE; _channel_array[77] = 0x01; _channel_array[78] = 0x00; _channel_array[79] = TEDS_TRANSDUCER_CHANNEL_SECONDS; _channel_array[80] = 0x01; _channel_array[81] = 0x82; _channel_array[82] = TEDS_TRANSDUCER_CHANNEL_PRE_TRIGG; _channel_array[83] = 0x02; _channel_array[84] = 0x00; _channel_array[85] = 0x00; _channel_array[86] = TEDS_TRANSDUCER_CHANNEL_R_SETUP_T; _channel_array[87] = 0x04; _channel_array[88] = 0x00; _channel_array[89] = 0x00; _channel_array[90] = 0x00; _channel_array[91] = 0x00; _channel_array[92] = TEDS_TRANSDUCER_CHANNEL_S_PERIOD; _channel_array[93] = 0x04; _channel_array[94] = 0x00; _channel_array[95] = 0x00; _channel_array[96] = 0x00; _channel_array[97] = 0x00; _channel_array[98] = TEDS_TRANSDUCER_CHANNEL_WARM_UP_T; _channel_array[99] = 0x04; _channel_array[100] = 0x00; _channel_array[101] = 0x00; _channel_array[102] = 0x00; _channel_array[103] = 0x00; _channel_array[104] = TEDS_TRANSDUCER_CHANNEL_R_DELAY_T; _channel_array[105] = 0x04; _channel_array[106] = 0x3f; _channel_array[107] = 0x00; _channel_array[108] = 0x00; _channel_array[109] = 0x00; _channel_array[110] = TEDS_TRANSDUCER_CHANNEL_SAMPLING; _channel_array[111] = 0x06; _channel_array[112] = TEDS_TRANSDUCER_CHANNEL_SAMP_MODE; _channel_array[113] = 0x01; _channel_array[114] = 0x04; _channel_array[115] = TEDS_TRANSDUCER_CHANNEL_S_DEFAULT; _channel_array[116] = 0x01; _channel_array[117] = 0x04; _channel_array[118] = TEDS_TRANSDUCER_CHANNEL_DATA_XMIT; _channel_array[119] = 0x01; /*_channel_array[120] = 0x02;*/ _channel_array[121] = 0xf4; _channel_array[122] = 0xa5; //checksum errado
    _channel_teds = new IEEE1451_TEDS_TIM(_channel_array, 123);

    if ((_operation_mode == OM_TIM_IM) || (_operation_mode == OM_TIM_IM_OPTIMIZED))
        _channel_array[120] = DATA_XMIT_BUFFER;
    else if ((_operation_mode == OM_POLLING) || (_operation_mode == OM_POLLING_OPTIMIZED))
        _channel_array[120] = DATA_XMIT_COMMANDED;


    char name[] = { "audio_sensor_1" };
    unsigned int name_len = strlen(name);
    unsigned int _audio_sensor_utn_len = 15 + name_len + 2;

    _audio_sensor_utn_array = new (kmalloc(_audio_sensor_utn_len)) char[_audio_sensor_utn_len];
    _audio_sensor_utn_array[0] = (_audio_sensor_utn_len - 4) >> 24; _audio_sensor_utn_array[1] = (_audio_sensor_utn_len - 4) >> 16; _audio_sensor_utn_array[2] = (_audio_sensor_utn_len - 4) >> 8; _audio_sensor_utn_array[3] = _audio_sensor_utn_len - 4; _audio_sensor_utn_array[4] = TEDS_USER_TRANSDUCER_NAME_TEDS_ID; _audio_sensor_utn_array[5] = 0x04; _audio_sensor_utn_array[6] = 0x00; _audio_sensor_utn_array[7] = TEDS_USER_TRANSDUCER_NAME; _audio_sensor_utn_array[8] = 0x01; _audio_sensor_utn_array[9] = 0x01; _audio_sensor_utn_array[10] = TEDS_USER_TRANSDUCER_NAME_FORMAT; _audio_sensor_utn_array[11] = 0x01; _audio_sensor_utn_array[12] = 0x00; _audio_sensor_utn_array[13] = TEDS_USER_TRANSDUCER_NAME_TC_NAME; _audio_sensor_utn_array[14] = name_len;

    unsigned int i = 15;
    for (unsigned int j = 0; j < name_len; i++, j++)
        _audio_sensor_utn_array[i] = name[j];
    _audio_sensor_utn_array[i++] = 0x00; _audio_sensor_utn_array[i++] = 0x00; //checksum errado

    _audio_sensor_utn_teds = new IEEE1451_TEDS_TIM(_audio_sensor_utn_array, _audio_sensor_utn_len);


    //char text_block[] = { "<?xml version=\"1.0\" encoding=\"UTF-8\"?><UnitsExtensionDataBlock xmlns=\"http://grouper.ieee.org/groups/1451/0/1451HTTPAPI\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://grouper.ieee.org/groups/1451/0/1451HTTPAPIUnitsExtensionDataBlock.xsd\"><UnitsExtensionText>G.726 - 16 kbits/s</UnitsExtensionText></UnitsExtensionDataBlock>" }; //Too big
    char text_block[] = { "<?xml version=\"1.0\" encoding=\"UTF-8\"?><UnitsExtensionDataBlock><UnitsExtensionText>G.726 - 16 kbits/s</UnitsExtensionText></UnitsExtensionDataBlock>" };
    unsigned int text_block_len = strlen(text_block);
    unsigned int unit_extension_len = 38 + text_block_len + 2;

    _units_extension_array = new (kmalloc(unit_extension_len)) char[unit_extension_len];
    _units_extension_array[0] = (unit_extension_len - 4) >> 24; _units_extension_array[1] = (unit_extension_len - 4) >> 16; _units_extension_array[2] = (unit_extension_len - 4) >> 8; _units_extension_array[3] = unit_extension_len - 4; _units_extension_array[4] = TEDS_TEXT_BASED_TEDS_ID; _units_extension_array[5] = 0x04; _units_extension_array[6] = 0x00; _units_extension_array[7] = TEDS_UNITS_EXTENSION; _units_extension_array[8] = 0x01; _units_extension_array[9] = 0x01; _units_extension_array[10] = TEDS_TEXT_BASED_NUM_LANG; _units_extension_array[11] = 0x01; _units_extension_array[12] = 0x01; _units_extension_array[13] = TEDS_TEXT_BASED_DIR_BLOCK; _units_extension_array[14] = 0x13; _units_extension_array[15] = TEDS_TEXT_BASED_LANG_CODE; _units_extension_array[16] = 0x02; _units_extension_array[17] = 'e'; _units_extension_array[18] = 'n'; _units_extension_array[19] = TEDS_TEXT_BASED_OFFSET; _units_extension_array[20] = 0x04; _units_extension_array[21] = 0x00; _units_extension_array[22] = 0x00; _units_extension_array[23] = 0x00; _units_extension_array[24] = 38 - 24; _units_extension_array[25] = TEDS_TEXT_BASED_LENGTH; _units_extension_array[26] = 0x04; _units_extension_array[27] = (text_block_len + 2) >> 24; _units_extension_array[28] = (text_block_len + 2) >> 16; _units_extension_array[29] = (text_block_len + 2) >> 8; _units_extension_array[30] = text_block_len + 2; _units_extension_array[31] = TEDS_TEXT_BASED_COMPRESS; _units_extension_array[32] = 0x01; _units_extension_array[33] = 0x00; _units_extension_array[34] = TEDS_TEXT_BASED_SUB_SUM; _units_extension_array[35] = 0x02; _units_extension_array[36] = 0x00; _units_extension_array[37] = 0x00; //checksum errado

    i = 38;
    for (unsigned int j = 0; j < text_block_len; i++, j++)
        _units_extension_array[i] = text_block[j];
    _units_extension_array[i++] = 0x00; _units_extension_array[i++] = 0x00; //checksum errado

    _units_extension_teds = new IEEE1451_TEDS_TIM(_units_extension_array, unit_extension_len);
}

IEEE1451_TEDS_TIM *IEEE1451_Audio_Sensor::get_teds(char id)
{
    if (id == TEDS_TRANSDUCER_CHANNEL)
        return _channel_teds;
    else if (id == TEDS_USER_TRANSDUCER_NAME)
        return _audio_sensor_utn_teds;
    else if (id == TEDS_UNITS_EXTENSION)
        return _units_extension_teds;
    return 0;
}

void IEEE1451_Audio_Sensor::start()
{
    cout << "Audio sensor start\n";
    _execute_thread->resume();
}

void IEEE1451_Audio_Sensor::stop()
{
    cout << "Audio sensor stop\n";
    _execute_thread->suspend();
}

void IEEE1451_Audio_Sensor::read_data_set(unsigned short trans_id, unsigned int offset)
{
    //cout << "Reading data set (polling, trans_id=" << trans_id << ")...\n";

    unsigned int size = sizeof(IEEE1451_Data_Set_Read_Reply) + DATASET_SIZE;
    char *buffer = IEEE1451_TIM::get_instance()->get_send_buffer();

    IEEE1451_Data_Set_Read_Reply *reply = (IEEE1451_Data_Set_Read_Reply *) buffer;
    char *data = buffer + sizeof(IEEE1451_Data_Set_Read_Reply);

    reply->_header._success = true;
    reply->_header._length = DATASET_SIZE + sizeof(reply->_offset);
    reply->_offset = _counter++;

    pack_audio(data);

    IEEE1451_TIM::get_instance()->send_msg(trans_id, size, true);
}

void IEEE1451_Audio_Sensor::start_read_data_set(unsigned short trans_id)
{
    cout << "Starting read data set (optimized polling)...\n";
    _reading_data = true;
    _execute_thread->resume();
}

void IEEE1451_Audio_Sensor::stop_read_data_set(unsigned short trans_id)
{
    cout << "Stopping read data set (optimized polling)...\n";
    _reading_data = false;
}

void IEEE1451_Audio_Sensor::send_data_set()
{
    //cout << "Sending data set (tim_im)...\n";

    unsigned int size = sizeof(IEEE1451_Command) + sizeof(unsigned long) + DATASET_SIZE;
    char *buffer = IEEE1451_TIM::get_instance()->get_send_buffer();

    IEEE1451_Command *cmd = (IEEE1451_Command *) buffer;
    unsigned long *offset = (unsigned long *) (buffer + sizeof(IEEE1451_Command));
    char *data = buffer + sizeof(IEEE1451_Command) + sizeof(unsigned long);

    cmd->_channel_number = _channel_number;
    cmd->_command = COMMAND_CLASS_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT;
    cmd->_length = DATASET_SIZE + sizeof(unsigned long);
    offset[0] = _counter++;

    pack_audio(data);

    IEEE1451_TIM::get_instance()->send_msg(0, size, true);
}

void IEEE1451_Audio_Sensor::send_start_read_data_set()
{
    cout << "Sending start read data set...\n";

    char *buffer = IEEE1451_TIM::get_instance()->get_send_buffer();
    IEEE1451_Command *cmd = (IEEE1451_Command *) buffer;

    cmd->_channel_number = _channel_number;
    cmd->_command = COMMAND_CLASS_START_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT;
    cmd->_length = 0;

    IEEE1451_TIM::get_instance()->send_msg(0, sizeof(IEEE1451_Command));
}

void IEEE1451_Audio_Sensor::send_stop_read_data_set()
{
    cout << "Sending stop read data set...\n";

    char *buffer = IEEE1451_TIM::get_instance()->get_send_buffer();
    IEEE1451_Command *cmd = (IEEE1451_Command *) buffer;

    cmd->_channel_number = _channel_number;
    cmd->_command = COMMAND_CLASS_STOP_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT;
    cmd->_length = 0;

    IEEE1451_TIM::get_instance()->send_msg(0, sizeof(IEEE1451_Command));
}

int IEEE1451_Audio_Sensor::execute()
{
    cout << "Audio sensor execute thread created\n";

    _execute_thread = Thread::self();
    IEEE1451_TIM *tim = IEEE1451_TIM::get_instance();
    tim->execute();

    tim->connect();
    _execute_thread->suspend();

    if (_operation_mode == OM_POLLING)
    {
        while (true)
        {
            //27 * 5 packets = 5020 seconds
            Thread::yield();
            for (unsigned int j = 0; j < SAMPLE_NO; j++)
            {
                if ((j + 1) % (SAMPLE_NO / 5) == 0)
                    Thread::yield();
                get_audio(); //27 packets = 1.0041 seconds
            }
        }
    }else if (_operation_mode == OM_POLLING_OPTIMIZED)
    {
        while (true)
        {
            //27 * 5 packets = 5120 seconds
            for (unsigned int i = 0; i < 4; i++)
                Thread::yield();

            for (unsigned int j = 0; j < SAMPLE_NO; j++)
            {
                if ((j + 1) % (SAMPLE_NO / 5) == 0)
                    Thread::yield();
                get_audio(); //27 packets = 1.0276 seconds
            }

            for (unsigned int i = 0; i < 4; i++)
                Thread::yield();

            if (_reading_data)
                send_data_set();
        }
    }else //if ((_operation_mode == OM_TIM_IM) || (_operation_mode == OM_TIM_IM_OPTIMIZED))
    {
        const unsigned short active_period = 5; //seconds
        const unsigned short sleep_period = 5; //seconds

        while (true)
        {
            if (_operation_mode == OM_TIM_IM)
            {
                tim->disconnect();

            }else if (_operation_mode == OM_TIM_IM_OPTIMIZED)
            {
                tim->_connected = false;
#ifdef __mc13224v__
                MC13224V_Transceiver::maca_off();
#endif
            }

            for (unsigned short i = 0; i < sleep_period * 27; i++)
            {
                for (unsigned int j = 0; j < SAMPLE_NO; j++)
                {
                    Thread::yield();
                    get_audio();
                }
            }

            if (_operation_mode == OM_TIM_IM)
            {
                tim->connect();
                _execute_thread->suspend();

            }else if (_operation_mode == OM_TIM_IM_OPTIMIZED)
            {
#ifdef __mc13224v__
                MC13224V_Transceiver::maca_on();
#endif
                tim->_connected = true;
                tim->_connected_cond.signal();
            }

            send_start_read_data_set();

            for (unsigned short i = 0; i < active_period * 27; i++)
            {
                //27 * 5 packets = 5108 seconds
                for (unsigned int j = 0; j < SAMPLE_NO; j++)
                {
                    if ((j + 1) % (SAMPLE_NO / 4) == 0)
                        Thread::yield();
                    get_audio(); //27 packets = 1.0272 seconds
                }

                send_data_set();
            }

            send_stop_read_data_set();
        }
    }

    return 0;
}

void IEEE1451_Audio_Sensor::get_audio()
{
#ifdef __mc13224v__
    _audio_buffer[_pos] = (short) _audio.sample();
#else
    _audio_buffer[_pos] = 0x1234;
#endif

    G726_encode(&_audio_buffer[_pos], &_audio_buffer[_pos], 1, &_law, _reset, _rate, &_state);
    _reset = 0;

    ((_pos + 1) >= SAMPLE_NO) ? _pos = 0 : _pos++;
}

void IEEE1451_Audio_Sensor::pack_audio(char *data)
{
    for (unsigned int i = 0, j = _pos; i < DATASET_SIZE; i++)
    {
        data[i] = (_audio_buffer[j] << 6) | (_audio_buffer[j + 1] << 4) |
                  (_audio_buffer[j + 2] << 2) | (_audio_buffer[j + 3]);

        ((j + 4) >= SAMPLE_NO) ? j = 0 : j += 4;
    }
}

//-------------------------------------------

int main()
{
    unsigned int *GPIO_BASE = (unsigned int *) 0x80000000;
    *GPIO_BASE = 0;

    Alarm::delay(TIME_500_MS * 2);
    cout << "+++++ Starting wtim +++++\n";

    IP *ip = IP::instance();
    ip->set_address(IP::Address(10, 0, 0, 112));
    ip->set_gateway(IP::Address(10, 0, 0, 1));
    ip->set_netmask(IP::Address(255, 255, 255, 0));

    IEEE1451_TIM *tim = IEEE1451_TIM::get_instance();
    tim->set_ncap_address(IP::Address(10, 0, 0, 110));

    IEEE1451_Audio_Sensor sensor(OM_POLLING_OPTIMIZED);
    sensor.execute();
    return 0;
}

#else

int main() { return 0; }

#endif

