#include <ieee1451_tim.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <mutex.h>
#include <thread.h>
#ifdef __mc13224v__
    #include <sensor.h>
#endif

#define DATASET_SIZE    10
#define READ_INTERVAL   TIME_500_MS * 20

__USING_SYS

OStream cout;

class IEEE1451_Temperature_Sensor : public IEEE1451_Transducer
{
public:
    IEEE1451_Temperature_Sensor(bool tim_im, bool polling);
    ~IEEE1451_Temperature_Sensor();

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

    bool _tim_im;
    bool _polling;

    float _data_set[DATASET_SIZE * sizeof(float)];
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

IEEE1451_Temperature_Sensor::IEEE1451_Temperature_Sensor(bool tim_im, bool polling)
{
    _channel_number = 0x01; //It just supports one transducer per mote!
    _tim_im = tim_im;
    _polling = polling;
    _pos = 0;
    _execute_thread = 0;

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
}

void IEEE1451_Temperature_Sensor::init_teds()
{
    _channel_array = new (kmalloc(123)) char[123];
    _channel_array[0] = 0x00; _channel_array[1] = 0x00; _channel_array[2] = 0x00; _channel_array[3] = 0x77; _channel_array[4] = TEDS_TRANSDUCER_CHANNEL_TEDS_ID; _channel_array[5] = 0x04; _channel_array[6] = 0x00; _channel_array[7] = TEDS_TRANSDUCER_CHANNEL; _channel_array[8] = 0x01; _channel_array[9] = 0x01; _channel_array[10] = TEDS_TRANSDUCER_CHANNEL_CAL_KEY; _channel_array[11] = 0x01; _channel_array[12] = 0x00; _channel_array[13] = TEDS_TRANSDUCER_CHANNEL_CHAN_TYPE; _channel_array[14] = 0x01; _channel_array[15] = 0x00; _channel_array[16] = TEDS_TRANSDUCER_CHANNEL_PHY_UNITS; _channel_array[17] = 0x06; _channel_array[18] = TEDS_TRANSDUCER_CHANNEL_UNIT_TYPE; _channel_array[19] = 0x01; _channel_array[20] = 0x00; _channel_array[21] = TEDS_TRANSDUCER_CHANNEL_KELVINS; _channel_array[22] = 0x01; _channel_array[23] = 0x82; _channel_array[24] = TEDS_TRANSDUCER_CHANNEL_LOW_LIMIT; _channel_array[25] = 0x04; _channel_array[26] = 0x43; _channel_array[27] = 0x69; _channel_array[28] = 0x00; _channel_array[29] = 0x00; _channel_array[30] = TEDS_TRANSDUCER_CHANNEL_HI_LIMIT; _channel_array[31] = 0x04; _channel_array[32] = 0x43; _channel_array[33] = 0xb0; _channel_array[34] = 0x80; _channel_array[35] = 0x00; _channel_array[36] = TEDS_TRANSDUCER_CHANNEL_O_ERROR; _channel_array[37] = 0x04; _channel_array[38] = 0x3f; _channel_array[39] = 0x00; _channel_array[40] = 0x00; _channel_array[41] = 0x00; _channel_array[42] = TEDS_TRANSDUCER_CHANNEL_SELF_TEST; _channel_array[43] = 0x01; _channel_array[44] = 0x00; _channel_array[45] = TEDS_TRANSDUCER_CHANNEL_SAMPLE; _channel_array[46] = 0x09; _channel_array[47] = TEDS_TRANSDUCER_CHANNEL_DAT_MODEL; _channel_array[48] = 0x01; _channel_array[49] = 0x01; _channel_array[50] = TEDS_TRANSDUCER_CHANNEL_MOD_LENGTH; _channel_array[51] = 0x01; _channel_array[52] = 0x04; _channel_array[53] = TEDS_TRANSDUCER_CHANNEL_SIG_BITS; _channel_array[54] = 0x01; _channel_array[55] = 0x00; _channel_array[56] = TEDS_TRANSDUCER_CHANNEL_DATA_SET; _channel_array[57] = 0x1c; _channel_array[58] = TEDS_TRANSDUCER_CHANNEL_REPEATS; _channel_array[59] = 0x02; _channel_array[60] = 0x00; _channel_array[61] = 0x0a; _channel_array[62] = TEDS_TRANSDUCER_CHANNEL_S_ORIGIN; _channel_array[63] = 0x04; _channel_array[64] = 0x00; _channel_array[65] = 0x00; _channel_array[66] = 0x00; _channel_array[67] = 0x00; _channel_array[68] = TEDS_TRANSDUCER_CHANNEL_STEP_SIZE; _channel_array[69] = 0x04; _channel_array[70] = 0x41; _channel_array[71] = 0x20; _channel_array[72] = 0x00; _channel_array[73] = 0x00; _channel_array[74] = TEDS_TRANSDUCER_CHANNEL_S_UNITS; _channel_array[75] = 0x06; _channel_array[76] = TEDS_TRANSDUCER_CHANNEL_UNIT_TYPE; _channel_array[77] = 0x01; _channel_array[78] = 0x00; _channel_array[79] = TEDS_TRANSDUCER_CHANNEL_SECONDS; _channel_array[80] = 0x01; _channel_array[81] = 0x82; _channel_array[82] = TEDS_TRANSDUCER_CHANNEL_PRE_TRIGG; _channel_array[83] = 0x02; _channel_array[84] = 0x00; _channel_array[85] = 0x00; _channel_array[86] = TEDS_TRANSDUCER_CHANNEL_R_SETUP_T; _channel_array[87] = 0x04; _channel_array[88] = 0x00; _channel_array[89] = 0x00; _channel_array[90] = 0x00; _channel_array[91] = 0x00; _channel_array[92] = TEDS_TRANSDUCER_CHANNEL_S_PERIOD; _channel_array[93] = 0x04; _channel_array[94] = 0x3d; _channel_array[95] = 0xcc; _channel_array[96] = 0xcc; _channel_array[97] = 0xcd; _channel_array[98] = TEDS_TRANSDUCER_CHANNEL_WARM_UP_T; _channel_array[99] = 0x04; _channel_array[100] = 0x00; _channel_array[101] = 0x00; _channel_array[102] = 0x00; _channel_array[103] = 0x00; _channel_array[104] = TEDS_TRANSDUCER_CHANNEL_R_DELAY_T; _channel_array[105] = 0x04; _channel_array[106] = 0x3f; _channel_array[107] = 0x00; _channel_array[108] = 0x00; _channel_array[109] = 0x00; _channel_array[110] = TEDS_TRANSDUCER_CHANNEL_SAMPLING; _channel_array[111] = 0x06; _channel_array[112] = TEDS_TRANSDUCER_CHANNEL_SAMP_MODE; _channel_array[113] = 0x01; _channel_array[114] = 0x04; _channel_array[115] = TEDS_TRANSDUCER_CHANNEL_S_DEFAULT; _channel_array[116] = 0x01; _channel_array[117] = 0x04; _channel_array[118] = TEDS_TRANSDUCER_CHANNEL_DATA_XMIT; _channel_array[119] = 0x01; /*_channel_array[120] = 0x02;*/ _channel_array[121] = 0xf4; _channel_array[122] = 0xa5; //checksum errado
    _channel_teds = new IEEE1451_TEDS_TIM(_channel_array, 123);

    if (!_tim_im)
        _channel_array[120] = 0x01;
    else if (_tim_im)
        _channel_array[120] = 0x02;

    _temp_sensor_utn_array = new (kmalloc(43)) char[43];
    _temp_sensor_utn_array[0] = 0x00; _temp_sensor_utn_array[1] = 0x00; _temp_sensor_utn_array[2] = 0x00; _temp_sensor_utn_array[3] = 0x27; _temp_sensor_utn_array[4] = TEDS_USER_TRANSDUCER_NAME_TEDS_ID; _temp_sensor_utn_array[5] = 0x04; _temp_sensor_utn_array[6] = 0x00; _temp_sensor_utn_array[7] = TEDS_USER_TRANSDUCER_NAME; _temp_sensor_utn_array[8] = 0x01; _temp_sensor_utn_array[9] = 0x01; _temp_sensor_utn_array[10] = TEDS_USER_TRANSDUCER_NAME_FORMAT; _temp_sensor_utn_array[11] = 0x01; _temp_sensor_utn_array[12] = 0x00; _temp_sensor_utn_array[13] = TEDS_USER_TRANSDUCER_NAME_TC_NAME; _temp_sensor_utn_array[14] = 0x1a; _temp_sensor_utn_array[15] = 0x4f; _temp_sensor_utn_array[16] = 0x6e; _temp_sensor_utn_array[17] = 0x62; _temp_sensor_utn_array[18] = 0x6f; _temp_sensor_utn_array[19] = 0x61; _temp_sensor_utn_array[20] = 0x72; _temp_sensor_utn_array[21] = 0x64; _temp_sensor_utn_array[22] = 0x20; _temp_sensor_utn_array[23] = 0x54; _temp_sensor_utn_array[24] = 0x65; _temp_sensor_utn_array[25] = 0x6d; _temp_sensor_utn_array[26] = 0x70; _temp_sensor_utn_array[27] = 0x65; _temp_sensor_utn_array[28] = 0x72; _temp_sensor_utn_array[29] = 0x61; _temp_sensor_utn_array[30] = 0x74; _temp_sensor_utn_array[31] = 0x75; _temp_sensor_utn_array[32] = 0x72; _temp_sensor_utn_array[33] = 0x65; _temp_sensor_utn_array[34] = 0x20; _temp_sensor_utn_array[35] = 0x53; _temp_sensor_utn_array[36] = 0x65; _temp_sensor_utn_array[37] = 0x6e; _temp_sensor_utn_array[38] = 0x73; _temp_sensor_utn_array[39] = 0x6f; _temp_sensor_utn_array[40] = 0x72; _temp_sensor_utn_array[41] = 0xf5; _temp_sensor_utn_array[42] = 0x92; //checksum errado
    _temp_sensor_utn_teds = new IEEE1451_TEDS_TIM(_temp_sensor_utn_array, 43);
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
    char buffer[size];

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

    IEEE1451_TIM::get_instance()->send_msg(trans_id, buffer, size);
}

void IEEE1451_Temperature_Sensor::send_data_set()
{
    cout << "Sending data set (tim_im)...\n";

    unsigned int size = sizeof(IEEE1451_Command) + DATASET_SIZE * sizeof(float);
    char buffer[size];

    IEEE1451_Command *cmd = (IEEE1451_Command *) buffer;
    float *data = (float *) (buffer + sizeof(IEEE1451_Command));

    cmd->_channel_number = _channel_number;
    cmd->_command = COMMAND_CLASS_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT;
    cmd->_length = DATASET_SIZE * sizeof(float);

    _data_set_mutex.lock();
    for (int i = 0, j = _pos; i < DATASET_SIZE; i++)
    {
        data[i] = _data_set[j];
        j = (j + 1) % DATASET_SIZE;
    }
    _data_set_mutex.unlock();

    IEEE1451_TIM::get_instance()->send_msg(0, buffer, size);
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
        if ((_tim_im) && (!_polling) && (_pos == 0))
            tim->disconnect();

        cout << "Collecting data (pos = " << _pos << ")...\n";

        //while (!_temperature.enable());
        //while (!_temperature.data_ready());

        _data_set_mutex.lock();

#ifdef __mc13224v__
        _data_set[_pos] = _temperature.sample();
#else
        _data_set[_pos] = count++;
#endif

        _pos = (_pos + 1) % DATASET_SIZE;
        _data_set_mutex.unlock();

        //_temperature.disable();
        Alarm::delay(READ_INTERVAL);

        if ((_tim_im) && (!_polling) && (_pos == 0))
        {
            tim->connect();
            _execute_thread->suspend();
        }

        if ((_tim_im) && (_pos == 0))
            send_data_set();
    }

    return 0;
}

//-------------------------------------------

int main()
{
    Alarm::delay(TIME_500_MS * 4);
    cout << "+++++ Starting wtim +++++\n";

    IP *ip = IP::instance();
    ip->set_address(IP::Address(10, 0, 0, 111));
    ip->set_gateway(IP::Address(10, 0, 0, 1));
    ip->set_netmask(IP::Address(255, 255, 255, 0));

    Alarm::delay(TIME_500_MS * 4);

    IEEE1451_TIM *tim = IEEE1451_TIM::get_instance();
    tim->set_ncap_address(IP::Address(10, 0, 0, 110));

    IEEE1451_Temperature_Sensor sensor(true, true);
    sensor.execute();
    return 0;
}

/* IEEE 1451.0 (2007) -> Chapter 5.10
    -> TIM-Initiated Message
       Sampling modes: Continuous sampling mode
       Data transmission mode: Streaming when a buffer is full mode
       Streaming operation = Continuous sampling + Streaming when a buffer is full

    -> Polling
       Sampling modes: Continuous sampling mode
       Data transmission mode: Only when commanded mode */

