#ifndef ICMP_H

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define ICMP_H

#include <cpu.h>
#include <ip.h>

__BEGIN_SYS

class ICMP_SingleNIC {
public:
    static ICMP * instance();
protected:
    inline ICMP_SingleNIC(IP * ip) ;
    
    IP * ip() const { return IP::instance(); }
private:
    static ICMP * _instance;
};

class ICMP_MultiNIC {
public:
    static ICMP * instance(unsigned int i=0);
protected:
    ICMP_MultiNIC(IP * ip) : _ip(ip) {}
    
    IP * ip() const { return _ip; }
private:
    IP * _ip;
    static ICMP * _instance[Traits<NIC>::NICS::Length];
};

class ICMP : public IF<IP::multiNIC, ICMP_MultiNIC, ICMP_SingleNIC>::Result,
             public IP::Observer,
             public Data_Observed<IP::Address> {
public:
    typedef Data_Observer<IP::Address> Observer;
    typedef Data_Observed<IP::Address> Observed;
    
    typedef IF<IP::multiNIC, ICMP_MultiNIC, ICMP_SingleNIC>::Result Base;
    
    static const unsigned short ICMP_ID = 1; // IP sub-protocol identifier

    typedef unsigned char Code;
    typedef unsigned char Type;

    enum /*Types*/{
        ECHO_REPLY = 0,
        UNREACHABLE =  3,
        SOURCE_QUENCH = 4,
        REDIRECT = 5,
        ALTERNATE_ADDRESS = 6,
        ECHO = 8,
        ROUTER_ADVERT = 9,
        ROUTER_SOLIC = 10,
        TIME_EXCEEDED = 11,
        PARAMETER_PROBLEM = 12,
        TIMESTAMP = 13,
        TIMESTAMP_REPLY = 14,
        INFO_REQUEST = 15,
        INFO_REPLY = 16,
        ADDRESS_MASK_REQ = 17,
        ADDRESS_MASK_REP = 18,
        TRACEROUTE = 30,
        DGRAM_ERROR = 31,
        MOBILE_HOST_REDIR = 32,
        IPv6_WHERE_ARE_YOU = 33,
        IPv6_I_AM_HERE = 34,
        MOBILE_REG_REQ = 35,
        MOBILE_REG_REP = 36,
        DOMAIN_NAME_REQ = 37,
        DOMAIN_NAME_REP = 38,
        SKIP = 39
    };

    enum /*Unreacheble Codes*/ {
        NETWORK_UNREACHABLE = 0,
        HOST_UNREACHABLE = 1,
        PROTOCOL_UNREACHABLE = 2,
        PORT_UNREACHABLE = 3,
        FRAGMENTATION_NEEDED = 4,
        ROUTE_FAILED = 5,
        NETWORK_UNKNOWN = 6,
        HOST_UNKNOWN = 7,
        HOST_ISOLATED = 8,
        NETWORK_PROHIBITED = 9,
        HOST_PROHIBITED = 10,
        NETWORK_TOS_UNREACH = 11,
        HOST_TOS_UNREACH = 12,
        ADMIN_PROHIBITED = 13,
        PRECEDENCE_VIOLATION = 14,
        PRECEDENCE_CUTOFF = 15
    };

    class Packet {
        friend class ICMP;
    protected:
        unsigned char _type;
        unsigned char _code;
        unsigned short _checksum;
        unsigned short _id;
        unsigned short _sequence;
        char _data[56];
    public:
        Packet(Type type,Code code,
               unsigned short id,unsigned short seq,
               const char * data = 0,short size = 56);

        const char* raw() {
            return reinterpret_cast<const char*>(this);
        }

        Type type() { return _type; }
        Code code() { return _code; }
        unsigned short id() { return CPU::htons(_id); }
        unsigned short sequence() { return CPU::htons(_sequence); }
        unsigned short checksum() { return _checksum; }

        char * data() { return _data; }
    };

    ICMP(IP* ip = 0);
    ~ICMP();
    
    void update(Data_Observed<IP::Address> *ob, long c, IP::Address src,
            IP::Address dst, void *data, unsigned int size);
            
    void send(IP::Address to,Packet & pkt) {
        send(ip()->address(),to,pkt);    
    }
    
    void send(IP::Address from,IP::Address to,Packet& pkt);
    
};

// This is here because we cannot use static_cast before ICMP is defined
ICMP_SingleNIC::ICMP_SingleNIC(IP * ip) {
    _instance = (ICMP*)(this);
}
    

__END_SYS

#endif // ICMP_H
