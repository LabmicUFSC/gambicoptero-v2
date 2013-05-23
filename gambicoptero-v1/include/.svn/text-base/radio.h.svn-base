// EPOS Radio Mediator Common Package

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <nic.h>
#include <utility/string.h>

#ifndef __radio_h
#define __radio_h

__BEGIN_SYS

class Radio_Common: public NIC_Common
{
protected:
    Radio_Common() {}

public:
    typedef NIC_Common::Address<1> Address;
    typedef unsigned char Protocol;

    static const unsigned int MTU = 24;  
    static const unsigned int HEADER_SIZE = 6;
    static const unsigned int TRAILER_SIZE = 2;
    static const Address BROADCAST;

    typedef char PDU[MTU];

    class Frame {
    public:
    Frame() {}

    Frame(const Address & dst, const Address & src,
          const Protocol & prot, const unsigned char & size) {
        header(dst, src, prot, size);
    }

    Frame(const Address & dst, const Address & src,
          const Protocol & prot, const void * data,
          const unsigned char & size)
    {
        header(dst, src, prot, size);
        memcpy(_data, data, size);
        _crc = CRC::crc16((char*)this, sizeof(Frame)-2);
    }
    
    Frame(const Address & dst, const Address & src,
          const Protocol & prot, const void * data, 
          const unsigned char & size, const char & tx_pow,
          const char & rss)
    {
        header(dst, src, prot, size);
        memcpy(_data, data, size);
        _tx_pow = tx_pow;
        _rss = rss;
        _crc = CRC::crc16((char*)this, sizeof(Frame)-2);
    }

    friend Debug & operator << (Debug & db, const Frame & f) {
        db << "{" << Address(f._dst)
           << "," << Address(f._src)
           << "," << f._prot
           << "," << f._len
           << "," << f._data << "}";
        return db;
    }
    
    private:
    void header(const Address & dst, const Address & src,
            const unsigned char & prot, const unsigned char & len)
    {
        _src = src;
        _dst = dst;
        _prot = prot;
        _len = len;
    }

    public:
    Address _src;
    Address _dst;
    unsigned char _prot;
    unsigned char _len;
    unsigned char _tx_pow;
    unsigned char _rss;
    char _data[MTU];
    unsigned short _crc;
    };

    // Frame types
    enum {
    EPOS_LP = 0x01,
    IP,
    ARP,
    RARP
    };

    struct Statistics: public NIC_Common::Statistics {
    Statistics() : tx_time(0), rx_time(0), dropped_packets(0), total_tx_packets(0) {}

    friend Debug & operator << (Debug & db, const Statistics & s) {
        db << "{rxp=" << s.rx_packets
           << ",rxb=" <<  s.rx_bytes
           << ",rxt=" <<  s.rx_time
           << ",txp=" <<  s.tx_packets
           << ",txb=" <<  s.tx_bytes
           << ",txt=" <<  s.tx_time
           << ",drop=" <<  s.dropped_packets
           << ",ttxp=" <<  s.total_tx_packets
           << "}";
        return db;
    }
    
    friend OStream & operator << (OStream & db, const Statistics & s) {
            db << "{rxp=" << s.rx_packets
               << ",rxb=" <<  s.rx_bytes
               << ",rxt=" <<  s.rx_time
               << ",txp=" <<  s.tx_packets
               << ",txb=" <<  s.tx_bytes
               << ",txt=" <<  s.tx_time
               << ",drop=" <<  s.dropped_packets
               << ",ttxp=" <<  s.total_tx_packets
               << "}";
            return db;
        }

    unsigned long tx_time;
    unsigned long rx_time;
    unsigned int dropped_packets;
    unsigned int total_tx_packets;
    };

};

__END_SYS

#ifdef __RADIO_H
#include __RADIO_H
#endif

#endif
