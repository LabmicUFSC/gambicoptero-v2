#include <rtp.h>

#ifdef __NIC_H

__BEGIN_SYS

int RTP::receive(char *payload, u32 size)
{
    UDP::Address src;

    int ret = _channel.receive(&src, _packet, PAYLOAD_SIZE);

    if (ret > (int) sizeof(Header))
    {
        Header *header = reinterpret_cast<Header *>(_packet);
        db<RTP>(TRC) << "RTP::receive: " << header << "\n";

        char *data = _packet + sizeof(Header);
        unsigned int length = ret - sizeof(Header);
        if (length > size)
            length = size;

        memcpy(payload, data, length);
    }

    return ret;
}

int RTP::send(const char *payload, u32 size, u8 marker)
{
    _snd_header._marker = marker;

    db<RTP>(TRC) << "RTP::send: " << _snd_header << "\n";

    SegmentedBuffer data(payload, size);
    SegmentedBuffer pdu(&_snd_header, sizeof(Header), &data);

    u32 length = pdu.total_size();
    char buffer[length];
    pdu.copy_to(buffer, length);

    _channel.send(buffer, length);

    u32 timestamp = CPU::ntohl(_snd_header._timestamp);
    timestamp += size;
    _snd_header._timestamp = CPU::htonl(timestamp);

    u16 sequence_number = CPU::ntohs(_snd_header._sequence_number);
    sequence_number++;
    _snd_header._sequence_number = CPU::htons(sequence_number);

    return 0;
}

__END_SYS

#endif
