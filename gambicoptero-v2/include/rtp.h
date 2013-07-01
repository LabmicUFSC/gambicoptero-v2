#ifndef __rtp_h
#define __rtp_h

#include <system/config.h>

#ifdef __NIC_H

#include <udp.h>
#include <utility/random.h>

__BEGIN_SYS

class RTP
{
public:
    static const int PAYLOAD_SIZE = 512;

    enum Payload_Type
    {
        PCMU = 0,      _1016 = 1,     G721 = 2,         GSM = 3,
        G723 = 4,      DVI4_8 = 5,    DVI4_16 = 6,      LPC = 7,
        PCMA = 8,      G722 = 9,      L16_Stereo = 10,  L16_Mono = 11,
        QCELP = 12,    CN = 13,       MPA = 14,         G728 = 15,
        DVI4_11 = 16,  DVI4_22 = 17,  G729 = 18,        CellB = 25,
        JPEG = 26,     NV = 28,       H261 = 31,        MPV = 32,
        MP2T = 33,     H263 = 34,     PAYLOAD_INVALID = 0x7f
    };

    RTP() : _channel(UDP::Address(IP::instance()->address(), 0), UDP::Address(Traits<IP>::BROADCAST, 0)) {}

    void set_remote(IP_Address address, u16 port) { _channel.remote(UDP::Address(address, port)); }
    void set_local(IP_Address address, u16 port) { _channel.local(UDP::Address(address, port)); }
    void set_payload(Payload_Type payload_type) { _snd_header._payload_type = payload_type; }

    int send(const char *payload, u32 size, u8 marker = 0);
    int receive(char *payload, u32 size);


    class Header
    {
        friend class RTP;
        static const u8 VERSION_NUMBER = 2;
        static const u8 CSRC_COUNT = 0;

    public:
        Header() : _csrc_count(CSRC_COUNT), _extensions(0), _padding(0), _version_number(VERSION_NUMBER),
            _payload_type(PAYLOAD_INVALID), _marker(0)
        {
            _sequence_number = CPU::htons((u16) Pseudo_Random::random() & 0x07ff);
            _timestamp = CPU::htonl((u32) Pseudo_Random::random() & 0x007fffff);
            _ssrc_id = CPU::htonl((u32) Pseudo_Random::random() & 0x007fffff);
        }

        friend Debug & operator << (Debug & db, const Header & h) {
            db << "{ver=" << h._version_number
               << ",pad=" << h._padding
               << ",ext=" << h._extensions
               << ",cc=" << h._csrc_count
               << ",marker="  << h._marker
               << ",ptype=" << h._payload_type
               << ",seq=" << CPU::ntohs(h._sequence_number)
               << ",timestamp=" << CPU::ntohl(h._timestamp)
               << ",ssrc_id=" << CPU::ntohl(h._ssrc_id)
               << "}";
            return db;
        }

    private:
        u8 _csrc_count:4;
        u8 _extensions:1;
        u8 _padding:1;
        u8 _version_number:2;

        u8 _payload_type:7;
        u8 _marker:1;

        u16 _sequence_number;
        u32 _timestamp;
        u32 _ssrc_id;
    };

private:
    UDP::Channel _channel;
    Header _snd_header;
    char _packet[PAYLOAD_SIZE];
};

__END_SYS

#endif

#endif
