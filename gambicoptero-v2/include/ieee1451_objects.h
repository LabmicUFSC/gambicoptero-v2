#ifndef __ieee1451_objects_h
#define __ieee1451_objects_h

#include <machine.h>
#include <ip.h>

__BEGIN_SYS

#define IEEE1451_PORT   55667
#define MAX_BUFFER_SIZE 250

#ifdef __mc13224v__
    #define TIME_50_MS  50000
#else
    #define TIME_50_MS  2500
#endif

#define TIME_500_MS     TIME_50_MS * 10

struct IEEE1451_Command
{
    unsigned short _channel_number;
    unsigned short _command;
    unsigned short _length;
    char _padding[2];
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

struct IEEE1451_UART
{
    IP_Address _address; //4 chars
    unsigned char _type;
    char _padding[3];
};

enum Packet_Type
{
    PACKET_TYPE_CONNECT             = 0x01,
    PACKET_TYPE_DISCONNECT          = 0x02,
    PACKET_TYPE_MESSAGE             = 0x03,
    PACKET_TYPE_MESSAGE_MULTIMEDIA  = 0x04,
    PACKET_TYPE_LOG                 = 0x05
};

enum IEEE1451_Command_Class //IEEE 1451.0 (2007) -> Chapter 7
{
    //COMMAND_CLASS_QUERY_TEDS                                      = 0x0101, //Query TEDS
    //COMMAND_CLASS_UPDATE_TEDS                                     = 0x0104, //Update TEDS
    COMMAND_CLASS_READ_TEDS_SEGMENT                                 = 0x0102, //Read TEDS segment
    COMMAND_CLASS_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT          = 0x0301, //Read TransducerChannel data-set segment
    COMMAND_CLASS_START_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT    = 0x0305, //Start Read TransducerChannel data-set segment (New)
    COMMAND_CLASS_STOP_READ_TRANSDUCER_CHANNEL_DATA_SET_SEGMENT     = 0x0306, //Stop Read TransducerChannel data-set segment (New)
    COMMAND_CLASS_TRANSDUCER_CHANNEL_OPERATE                        = 0x0401, //TransducerChannel Operate
    COMMAND_CLASS_TRANSDUCER_CHANNEL_IDLE                           = 0x0402  //TransducerChannel Idle
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
    TEDS_PHY                    = 13,
    TEDS_UNITS_EXTENSION        = 15
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
    TEDS_TRANSDUCER_CHANNEL_TEDS_ID         = 3,
    TEDS_TRANSDUCER_CHANNEL_CAL_KEY         = 10,
    TEDS_TRANSDUCER_CHANNEL_CHAN_TYPE       = 11,
    TEDS_TRANSDUCER_CHANNEL_PHY_UNITS       = 12,
    TEDS_TRANSDUCER_CHANNEL_UNIT_TYPE       = 50,
    TEDS_TRANSDUCER_CHANNEL_SECONDS         = 55,
    TEDS_TRANSDUCER_CHANNEL_KELVINS         = 57,
    TEDS_TRANSDUCER_CHANNEL_AUDIO_STREAM    = 61, //New
    TEDS_TRANSDUCER_CHANNEL_VIDEO_STREAM    = 62, //New
    TEDS_TRANSDUCER_CHANNEL_LOW_LIMIT       = 13,
    TEDS_TRANSDUCER_CHANNEL_HI_LIMIT        = 14,
    TEDS_TRANSDUCER_CHANNEL_O_ERROR         = 15,
    TEDS_TRANSDUCER_CHANNEL_SELF_TEST       = 16,
    TEDS_TRANSDUCER_CHANNEL_SAMPLE          = 18,
    TEDS_TRANSDUCER_CHANNEL_DAT_MODEL       = 40,
    TEDS_TRANSDUCER_CHANNEL_MOD_LENGTH      = 41,
    TEDS_TRANSDUCER_CHANNEL_SIG_BITS        = 42,
    TEDS_TRANSDUCER_CHANNEL_DATA_SET        = 19,
    TEDS_TRANSDUCER_CHANNEL_REPEATS         = 43,
    TEDS_TRANSDUCER_CHANNEL_S_ORIGIN        = 44,
    TEDS_TRANSDUCER_CHANNEL_STEP_SIZE       = 45,
    TEDS_TRANSDUCER_CHANNEL_S_UNITS         = 46,
    TEDS_TRANSDUCER_CHANNEL_PRE_TRIGG       = 47,
    TEDS_TRANSDUCER_CHANNEL_R_SETUP_T       = 22,
    TEDS_TRANSDUCER_CHANNEL_S_PERIOD        = 23,
    TEDS_TRANSDUCER_CHANNEL_WARM_UP_T       = 24,
    TEDS_TRANSDUCER_CHANNEL_R_DELAY_T       = 25,
    TEDS_TRANSDUCER_CHANNEL_SAMPLING        = 31,
    TEDS_TRANSDUCER_CHANNEL_SAMP_MODE       = 48,
    TEDS_TRANSDUCER_CHANNEL_S_DEFAULT       = 49,
    TEDS_TRANSDUCER_CHANNEL_DATA_XMIT       = 34
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

enum IEEE1451_TEDS_Data_Block_Text_Based
{
    TEDS_TEXT_BASED_TEDS_ID     = 3,
    TEDS_TEXT_BASED_NUM_LANG    = 10,
    TEDS_TEXT_BASED_DIR_BLOCK   = 11,
    TEDS_TEXT_BASED_LANG_CODE   = 20,
    TEDS_TEXT_BASED_OFFSET      = 21,
    TEDS_TEXT_BASED_LENGTH      = 22,
    TEDS_TEXT_BASED_COMPRESS    = 23,
    TEDS_TEXT_BASED_SUB_SUM     = 12,
};

enum IEEE1451_TEDS_Transducer_Channel_Data_Xmit
{
    DATA_XMIT_COMMANDED                     = 1,
    DATA_XMIT_COMMANDED_BUFFER              = 2,
    DATA_XMIT_COMMANDED_INTERVAL            = 3,
    DATA_XMIT_COMMANDED_BUFFER_INTERVAL     = 4,
    DATA_XMIT_BUFFER                        = 5,
    DATA_XMIT_INTERVAL                      = 6,
    DATA_XMIT_BUFFER_INTERVAL               = 7
};

__END_SYS

#endif
