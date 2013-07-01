#include <ieee1451_tim.h>

#ifdef __NIC_H

#include <mutex.h>
#include <thread.h>
#ifdef __mc13224v__
    #include <sensor.h>
#endif

#define DATASET_SIZE    10
#define READ_INTERVAL   TIME_500_MS * 20

__USING_SYS

OStream cout;

enum Operation_Mode
{
    OM_POLLING                  = 0,
    OM_TIM_IM                   = 1,
    OM_TIM_IM_OPTIMIZED         = 2,
    OM_POLLING_TIM_IM           = 3
};

class IEEE1451_Temperature_Sensor : public IEEE1451_Transducer
{
public:
    IEEE1451_Temperature_Sensor(Operation_Mode operation_mode);
    virtual ~IEEE1451_Temperature_Sensor();

protected:
    void init_teds();
    IEEE1451_TEDS_TIM *get_teds(char id);
    //bool running() { return run; };
    void start();
    void stop();
    void read_data_set(unsigned short trans_id, unsigned int offset);
    void send_data_set();

public:
    int execute();

private:
    unsigned short _channel_number;
    Operation_Mode _operation_mode;

    float *_data_set;
    Mutex _data_set_mutex;
    int _pos;

#ifdef __mc13224v__
    Temperature_Sensor _temperature;
#endif

    Thread *_execute_thread;

    char *_channel_array;
    char *_temp_sensor_utn_array;
    IEEE1451_TEDS_TIM *_channel_teds;
    IEEE1451_TEDS_TIM *_temp_sensor_utn_teds;
};

//-------------------------------------------

IEEE1451_Temperature_Sensor::IEEE1451_Temperature_Sensor(Operation_Mode operation_mode)
{
    _channel_number = 0x01; //It just supports one transducer per mote!
    _operation_mode = operation_mode;
    _pos = 0;
    _execute_thread = 0;

    _data_set = new (kmalloc(DATASET_SIZE * sizeof(float))) float[DATASET_SIZE * sizeof(float)];

    init_teds();
}

IEEE1451_Temperature_Sensor::~IEEE1451_Temperature_Sensor()
{
    if (_execute_thread)
        delete _execute_thread;

    if (_channel_array)
        kfree(_channel_array);
    if (_temp_sensor_utn_array)
        kfree(_temp_sensor_utn_array);

    if (_channel_teds)
        delete _channel_teds;
    if (_temp_sensor_utn_teds)
        delete _temp_sensor_utn_teds;

    if (_data_set)
        kfree (_data_set);
}

void IEEE1451_Temperature_Sensor::init_teds()
{
    _channel_array = new (kmalloc(123)) char[123];
    _channel_array[0] = 0x00; _channel_array[1] = 0x00; _channel_array[2] = 0x00; _channel_array[3] = 0x77; _channel_array[4] = TEDS_TRANSDUCER_CHANNEL_TEDS_ID; _channel_array[5] = 0x04; _channel_array[6] = 0x00; _channel_array[7] = TEDS_TRANSDUCER_CHANNEL; _channel_array[8] = 0x01; _channel_array[9] = 0x01; _channel_array[10] = TEDS_TRANSDUCER_CHANNEL_CAL_KEY; _channel_array[11] = 0x01; _channel_array[12] = 0x00; _channel_array[13] = TEDS_TRANSDUCER_CHANNEL_CHAN_TYPE; _channel_array[14] = 0x01; _channel_array[15] = 0x00; _channel_array[16] = TEDS_TRANSDUCER_CHANNEL_PHY_UNITS; _channel_array[17] = 0x06; _channel_array[18] = TEDS_TRANSDUCER_CHANNEL_UNIT_TYPE; _channel_array[19] = 0x01; _channel_array[20] = 0x00; _channel_array[21] = TEDS_TRANSDUCER_CHANNEL_KELVINS; _channel_array[22] = 0x01; _channel_array[23] = 0x82; _channel_array[24] = TEDS_TRANSDUCER_CHANNEL_LOW_LIMIT; _channel_array[25] = 0x04; _channel_array[26] = 0x43; _channel_array[27] = 0x69; _channel_array[28] = 0x00; _channel_array[29] = 0x00; _channel_array[30] = TEDS_TRANSDUCER_CHANNEL_HI_LIMIT; _channel_array[31] = 0x04; _channel_array[32] = 0x43; _channel_array[33] = 0xb0; _channel_array[34] = 0x80; _channel_array[35] = 0x00; _channel_array[36] = TEDS_TRANSDUCER_CHANNEL_O_ERROR; _channel_array[37] = 0x04; _channel_array[38] = 0x3f; _channel_array[39] = 0x00; _channel_array[40] = 0x00; _channel_array[41] = 0x00; _channel_array[42] = TEDS_TRANSDUCER_CHANNEL_SELF_TEST; _channel_array[43] = 0x01; _channel_array[44] = 0x00; _channel_array[45] = TEDS_TRANSDUCER_CHANNEL_SAMPLE; _channel_array[46] = 0x09; _channel_array[47] = TEDS_TRANSDUCER_CHANNEL_DAT_MODEL; _channel_array[48] = 0x01; _channel_array[49] = 0x01; _channel_array[50] = TEDS_TRANSDUCER_CHANNEL_MOD_LENGTH; _channel_array[51] = 0x01; _channel_array[52] = 0x04; _channel_array[53] = TEDS_TRANSDUCER_CHANNEL_SIG_BITS; _channel_array[54] = 0x01; _channel_array[55] = 0x00; _channel_array[56] = TEDS_TRANSDUCER_CHANNEL_DATA_SET; _channel_array[57] = 0x1c; _channel_array[58] = TEDS_TRANSDUCER_CHANNEL_REPEATS; _channel_array[59] = 0x02; _channel_array[60] = 0x00; _channel_array[61] = 0x0a; _channel_array[62] = TEDS_TRANSDUCER_CHANNEL_S_ORIGIN; _channel_array[63] = 0x04; _channel_array[64] = 0x00; _channel_array[65] = 0x00; _channel_array[66] = 0x00; _channel_array[67] = 0x00; _channel_array[68] = TEDS_TRANSDUCER_CHANNEL_STEP_SIZE; _channel_array[69] = 0x04; _channel_array[70] = 0x41; _channel_array[71] = 0x20; _channel_array[72] = 0x00; _channel_array[73] = 0x00; _channel_array[74] = TEDS_TRANSDUCER_CHANNEL_S_UNITS; _channel_array[75] = 0x06; _channel_array[76] = TEDS_TRANSDUCER_CHANNEL_UNIT_TYPE; _channel_array[77] = 0x01; _channel_array[78] = 0x00; _channel_array[79] = TEDS_TRANSDUCER_CHANNEL_SECONDS; _channel_array[80] = 0x01; _channel_array[81] = 0x82; _channel_array[82] = TEDS_TRANSDUCER_CHANNEL_PRE_TRIGG; _channel_array[83] = 0x02; _channel_array[84] = 0x00; _channel_array[85] = 0x00; _channel_array[86] = TEDS_TRANSDUCER_CHANNEL_R_SETUP_T; _channel_array[87] = 0x04; _channel_array[88] = 0x00; _channel_array[89] = 0x00; _channel_array[90] = 0x00; _channel_array[91] = 0x00; _channel_array[92] = TEDS_TRANSDUCER_CHANNEL_S_PERIOD; _channel_array[93] = 0x04; _channel_array[94] = 0x3d; _channel_array[95] = 0xcc; _channel_array[96] = 0xcc; _channel_array[97] = 0xcd; _channel_array[98] = TEDS_TRANSDUCER_CHANNEL_WARM_UP_T; _channel_array[99] = 0x04; _channel_array[100] = 0x00; _channel_array[101] = 0x00; _channel_array[102] = 0x00; _channel_array[103] = 0x00; _channel_array[104] = TEDS_TRANSDUCER_CHANNEL_R_DELAY_T; _channel_array[105] = 0x04; _channel_array[106] = 0x3f; _channel_array[107] = 0x00; _channel_array[108] = 0x00; _channel_array[109] = 0x00; _channel_array[110] = TEDS_TRANSDUCER_CHANNEL_SAMPLING; _channel_array[111] = 0x06; _channel_array[112] = TEDS_TRANSDUCER_CHANNEL_SAMP_MODE; _channel_array[113] = 0x01; _channel_array[114] = 0x04; _channel_array[115] = TEDS_TRANSDUCER_CHANNEL_S_DEFAULT; _channel_array[116] = 0x01; _channel_array[117] = 0x04; _channel_array[118] = TEDS_TRANSDUCER_CHANNEL_DATA_XMIT; _channel_array[119] = 0x01; /*_channel_array[120] = 0x02;*/ _channel_array[121] = 0xf4; _channel_array[122] = 0xa5; //checksum errado
    _channel_teds = new IEEE1451_TEDS_TIM(_channel_array, 123);

    if (_operation_mode == OM_POLLING_TIM_IM)
        _channel_array[120] = DATA_XMIT_COMMANDED_BUFFER;
    else if ((_operation_mode == OM_TIM_IM) || (_operation_mode == OM_TIM_IM_OPTIMIZED))
        _channel_array[120] = DATA_XMIT_BUFFER;
    else if (_operation_mode == OM_POLLING)
        _channel_array[120] = DATA_XMIT_COMMANDED;


    char name[] = { "temperature_sensor_1" };
    unsigned int name_len = strlen(name);
    unsigned int _temp_sensor_utn_len = 15 + name_len + 2;

    _temp_sensor_utn_array = new (kmalloc(_temp_sensor_utn_len)) char[_temp_sensor_utn_len];
    _temp_sensor_utn_array[0] = (_temp_sensor_utn_len - 4) >> 24; _temp_sensor_utn_array[1] = (_temp_sensor_utn_len - 4) >> 16; _temp_sensor_utn_array[2] = (_temp_sensor_utn_len - 4) >> 8; _temp_sensor_utn_array[3] = _temp_sensor_utn_len - 4; _temp_sensor_utn_array[4] = TEDS_USER_TRANSDUCER_NAME_TEDS_ID; _temp_sensor_utn_array[5] = 0x04; _temp_sensor_utn_array[6] = 0x00; _temp_sensor_utn_array[7] = TEDS_USER_TRANSDUCER_NAME; _temp_sensor_utn_array[8] = 0x01; _temp_sensor_utn_array[9] = 0x01; _temp_sensor_utn_array[10] = TEDS_USER_TRANSDUCER_NAME_FORMAT; _temp_sensor_utn_array[11] = 0x01; _temp_sensor_utn_array[12] = 0x00; _temp_sensor_utn_array[13] = TEDS_USER_TRANSDUCER_NAME_TC_NAME; _temp_sensor_utn_array[14] = name_len;

    unsigned int i = 15;
    for (unsigned int j = 0; j < name_len; i++, j++)
        _temp_sensor_utn_array[i] = name[j];
    _temp_sensor_utn_array[i++] = 0x00; _temp_sensor_utn_array[i++] = 0x00; //checksum errado

    _temp_sensor_utn_teds = new IEEE1451_TEDS_TIM(_temp_sensor_utn_array, _temp_sensor_utn_len);
}

IEEE1451_TEDS_TIM *IEEE1451_Temperature_Sensor::get_teds(char id)
{
    if (id == TEDS_TRANSDUCER_CHANNEL)
        return _channel_teds;
    else if (id == TEDS_USER_TRANSDUCER_NAME)
        return _temp_sensor_utn_teds;
    return 0;
}

void IEEE1451_Temperature_Sensor::start()
{
    cout << "Temperature sensor start\n";
    _execute_thread->resume();
}

void IEEE1451_Temperature_Sensor::stop()
{
    cout << "Temperature sensor stop\n";
    _execute_thread->suspend();
}

void IEEE1451_Temperature_Sensor::read_data_set(unsigned short trans_id, unsigned int offset)
{
    cout << "Reading data set (polling)...\n";

    unsigned int size = sizeof(IEEE1451_Data_Set_Read_Reply) + DATASET_SIZE * sizeof(float);
    char *buffer = IEEE1451_TIM::get_instance()->get_send_buffer();

    IEEE1451_Data_Set_Read_Reply *reply = (IEEE1451_Data_Set_Read_Reply *) buffer;
    float *data = (float *) (buffer + sizeof(IEEE1451_Data_Set_Read_Reply));

    reply->_header._success = true;
    reply->_header._length = DATASET_SIZE * sizeof(float) + sizeof(reply->_offset);
    reply->_offset = 0;

    _data_set_mutex.lock();
    for (int i = 0, j = _pos; i < DATASET_SIZE; i++)
    {
        data[i] = _data_set[j];
        j = (j + 1) % DATASET_SIZE;
    }
    _data_set_mutex.unlock();

    IEEE1451_TIM::get_instance()->send_msg(trans_id, size);
}

void IEEE1451_Temperature_Sensor::send_data_set()
{
    cout << "Sending data set (tim_im)...\n";

    unsigned int size = sizeof(IEEE1451_Command) + sizeof(unsigned long) + DATASET_SIZE * sizeof(float);
    char *buffer = IEEE1451_TIM::get_instance()->get_send_buffer();

    IEEE1451_Command *cmd = (IEEE1451_Command *) buffer;
    unsigned long *offset = (unsigned long *) (buffer + sizeof(IEEE1451_Command));
    float *data = (float *) (buffer + sizeof(IEEE1451_Command) + sizeof(unsigned long));

    cmd->_channel_number = _channel_number;
    cmd->_command = COMMAND_CLASS_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT;
    cmd->_length = sizeof(unsigned long) + DATASET_SIZE * sizeof(float);
    offset[0] = 0;

    _data_set_mutex.lock();
    for (int i = 0, j = _pos; i < DATASET_SIZE; i++)
    {
        data[i] = _data_set[j];
        j = (j + 1) % DATASET_SIZE;
    }
    _data_set_mutex.unlock();

    IEEE1451_TIM::get_instance()->send_msg(0, size);
}

int IEEE1451_Temperature_Sensor::execute()
{
    cout << "Temperature sensor execute thread created\n";

    _execute_thread = Thread::self();
    IEEE1451_TIM *tim = IEEE1451_TIM::get_instance();
    tim->execute();

    tim->connect();
    _execute_thread->suspend();

#ifndef __mc13224v__
    int count = 1;
#endif

    while (true)
    {
        if ((_operation_mode == OM_TIM_IM) && (_pos == 0))
        {
            tim->disconnect();

        }else if ((_operation_mode == OM_TIM_IM_OPTIMIZED) && (_pos == 0))
        {
            tim->_connected = false;
#ifdef __mc13224v__
            MC13224V_Transceiver::maca_off();
#endif
        }

        cout << "Collecting data (pos = " << _pos << ")...\n";

        _data_set_mutex.lock();

#ifdef __mc13224v__
        _data_set[_pos] = _temperature.sample();
#else
        _data_set[_pos] = count++;
#endif

        _pos = (_pos + 1) % DATASET_SIZE;
        _data_set_mutex.unlock();

        Alarm::delay(READ_INTERVAL);

        if ((_operation_mode == OM_TIM_IM) && (_pos == 0))
        {
            tim->connect();
            _execute_thread->suspend();

        }else if ((_operation_mode == OM_TIM_IM_OPTIMIZED) && (_pos == 0))
        {
#ifdef __mc13224v__
            MC13224V_Transceiver::maca_on();
#endif
            tim->_connected = true;
            tim->_connected_cond.signal();
        }

        if ((_operation_mode != OM_POLLING) && (_pos == 0))
            send_data_set();
    }

    return 0;
}

//-------------------------------------------

int main()
{
    unsigned int *GPIO_BASE = (unsigned int *) 0x80000000;
    *GPIO_BASE = 0;

    Alarm::delay(TIME_500_MS * 2);
    cout << "+++++ Starting wtim +++++\n";

    IP *ip = IP::instance();
    ip->set_address(IP::Address(10, 0, 0, 111));
    ip->set_gateway(IP::Address(10, 0, 0, 1));
    ip->set_netmask(IP::Address(255, 255, 255, 0));

    IEEE1451_TIM *tim = IEEE1451_TIM::get_instance();
    tim->set_ncap_address(IP::Address(10, 0, 0, 110));

    IEEE1451_Temperature_Sensor sensor(OM_POLLING);
    sensor.execute();
    return 0;
}

#else

int main() { return 0; }

#endif
