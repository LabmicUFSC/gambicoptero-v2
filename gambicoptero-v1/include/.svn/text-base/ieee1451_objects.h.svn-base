#ifndef __ieee1451_objects_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __ieee1451_objects_h

#include <machine.h>

__BEGIN_SYS

#define IEEE1451_PORT 55667

struct IEEE1451_Command
{
    unsigned short _channel_number;
    unsigned short _command;
    unsigned short _length;
};

struct IEEE1451_Reply
{
    bool _success;
    unsigned short _length;
};

/*struct TEDS_Query_Reply
{
     IEEE1451_Reply _header;
     unsigned char _atributes;
     unsigned char _status;
     unsigned long _size;
     unsigned short _checksum;
     unsigned long _max_size;
};*/

struct IEEE1451_TEDS_Read_Reply
{
    IEEE1451_Reply _header;
    unsigned long _offset;
};

struct IEEE1451_Data_Set_Read_Reply
{
    IEEE1451_Reply _header;
    unsigned long _offset;
};

struct IEEE1451_Packet
{
    unsigned short _trans_id;
    unsigned short _length;
};

enum IEEE1451_Command_Class //IEEE 1451.0 (2007) -> Chapter 7
{
    //COMMAND_CLASS_QUERY_TEDS                              = 0x0101, //Query TEDS
    //COMMAND_CLASS_UPDATE_TEDS                             = 0x0104, //Update TEDS
    COMMAND_CLASS_READ_TEDS_SEGMENT                         = 0x0102, //Read TEDS segment
    COMMAND_CLASS_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT  = 0x0301, //Read TransducerChannel data-set segment
    COMMAND_CLASS_TRANSDUCER_CHANNEL_OPERATE                = 0x0401, //TransducerChannel Operate
    COMMAND_CLASS_TRANSDUCER_CHANNEL_IDLE                   = 0x0402  //TransducerChannel Idle
};

enum IEEE1451_Address_Class //IEEE 1451.0 (2007) -> Chapter 5.3
{
    ADDRESS_CLASS_TIM                       = 0x0000,
    ADDRESS_CLASS_TRANSDUCER_CHANNEL_FIRST  = 0x0001,
    ADDRESS_CLASS_TRANSDUCER_CHANNEL_LAST   = 0x7FFF,
    ADDRESS_CLASS_GROUP_FIRST               = 0x8000,
    ADDRESS_CLASS_GROUP_LAST                = 0xFFFE,
    ADDRESS_CLASS_GLOBAL                    = 0xFFFF
};

enum IEEE1451_TEDS_Access_Code
{
    TEDS_META                   = 1,
    TEDS_TRANSDUCER_CHANNEL     = 3,
    TEDS_USER_TRANSDUCER_NAME   = 12,
    TEDS_PHY                    = 13
};

enum IEEE1451_TEDS_Data_Block_Meta
{
    TEDS_META_TEDS_ID     = 3,
    TEDS_META_UUID        = 4,
    TEDS_META_O_HOLD_OFF  = 10,
    TEDS_META_TEST_TIME   = 12,
    TEDS_META_MAX_CHAN    = 13
};

enum IEEE1451_TEDS_Data_Block_Transducer_Channel
{
    TEDS_TRANSDUCER_CHANNEL_TEDS_ID     = 3,
    TEDS_TRANSDUCER_CHANNEL_CAL_KEY     = 10,
    TEDS_TRANSDUCER_CHANNEL_CHAN_TYPE   = 11,
    TEDS_TRANSDUCER_CHANNEL_PHY_UNITS   = 12,
    TEDS_TRANSDUCER_CHANNEL_UNIT_TYPE   = 50,
    TEDS_TRANSDUCER_CHANNEL_SECONDS     = 55,
    TEDS_TRANSDUCER_CHANNEL_KELVINS     = 57,
    TEDS_TRANSDUCER_CHANNEL_AUDIOS      = 61, //New
    TEDS_TRANSDUCER_CHANNEL_LOW_LIMIT   = 13,
    TEDS_TRANSDUCER_CHANNEL_HI_LIMIT    = 14,
    TEDS_TRANSDUCER_CHANNEL_O_ERROR     = 15,
    TEDS_TRANSDUCER_CHANNEL_SELF_TEST   = 16,
    TEDS_TRANSDUCER_CHANNEL_SAMPLE      = 18,
    TEDS_TRANSDUCER_CHANNEL_DAT_MODEL   = 40,
    TEDS_TRANSDUCER_CHANNEL_MOD_LENGTH  = 41,
    TEDS_TRANSDUCER_CHANNEL_SIG_BITS    = 42,
    TEDS_TRANSDUCER_CHANNEL_DATA_SET    = 19,
    TEDS_TRANSDUCER_CHANNEL_REPEATS     = 43,
    TEDS_TRANSDUCER_CHANNEL_S_ORIGIN    = 44,
    TEDS_TRANSDUCER_CHANNEL_STEP_SIZE   = 45,
    TEDS_TRANSDUCER_CHANNEL_S_UNITS     = 46,
    TEDS_TRANSDUCER_CHANNEL_PRE_TRIGG   = 47,
    TEDS_TRANSDUCER_CHANNEL_R_SETUP_T   = 22,
    TEDS_TRANSDUCER_CHANNEL_S_PERIOD    = 23,
    TEDS_TRANSDUCER_CHANNEL_WARM_UP_T   = 24,
    TEDS_TRANSDUCER_CHANNEL_R_DELAY_T   = 25,
    TEDS_TRANSDUCER_CHANNEL_SAMPLING    = 31,
    TEDS_TRANSDUCER_CHANNEL_SAMP_MODE   = 48,
    TEDS_TRANSDUCER_CHANNEL_S_DEFAULT   = 49,
    TEDS_TRANSDUCER_CHANNEL_DATA_XMIT   = 34
};

enum IEEE1451_TEDS_Data_Block_User_Transducer_Name
{
    TEDS_USER_TRANSDUCER_NAME_TEDS_ID   = 3,
    TEDS_USER_TRANSDUCER_NAME_FORMAT    = 4,
    TEDS_USER_TRANSDUCER_NAME_TC_NAME   = 5
};

enum IEEE1451_TEDS_Data_Block_Phy
{
    TEDS_PHY_TEDS_ID     = 3,
    TEDS_PHY_RADIO       = 10,
    TEDS_PHY_MAX_C_DEV   = 12,
    TEDS_PHY_BATTERY     = 22,
    TEDS_PHY_RECONNECT   = 40
};

__END_SYS

#endif
