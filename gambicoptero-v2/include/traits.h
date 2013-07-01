

#ifndef __traits_h
#define __traits_h

#include <system/config.h>

__BEGIN_SYS

template <class Imp>
struct Traits
{
    static const bool enabled = true;
    static const bool debugged = true;
    static const bool power_management = false;
};


/*
 *
 * Utilities
 *
 */
template <> struct Traits<Debug>
{
    static const bool error   = true;
    static const bool warning = true;
    static const bool info    = false;
    static const bool trace   = false;
};

template <> struct Traits<Lists>: public Traits<void>
{
    static const bool debugged = false;
};

template <> struct Traits<Spin>: public Traits<void>
{
    static const bool debugged = false;
};

template <> struct Traits<Heap>: public Traits<void>
{
    static const bool priority_alloc = false;
};


/*
 *
 * System parts
 *
 */
template <> struct Traits<Boot>: public Traits<void>
{
};

template <> struct Traits<Setup>: public Traits<void>
{
};

template <> struct Traits<Init>: public Traits<void>
{
};

template <> struct Traits<System>: public Traits<void>
{
};


/*
 *
 * Common Mediators 
 *
 */
template <> struct Traits<Serial_Display>: public Traits<void>
{
    static const bool enabled = true;
    static const int COLUMNS = 80;
    static const int LINES = 24;
    static const int TAB_SIZE = 8;
};

template <> struct Traits<CMAC<Radio_Wrapper> >: public Traits<void>
{
    static const bool debugged = false;

    static const bool time_triggered = true;
    static const bool auto_rx        = true;    // automatically listen to the channel. obs: must have time_triggered = true
    static const bool coordinator    = false;
    static const bool ack            = false;
    static const bool csma           = true;
    static const bool rts_cts        = false;

    static const unsigned long  SLEEPING_PERIOD = 500;  // ms
    static const unsigned long  TIMEOUT         = 400;   // ms
    static const unsigned long  BACKOFF         = 2;     // ms
    static const unsigned short ADDRESS         = 0x0001;
    static const unsigned int   MTU             = 118;
    static const unsigned int   BUFFER_SIZE     = 5;     // of packets
};


/*
 *
 * Abstractions
 *
 */
template <> struct Traits<Thread>: public Traits<void>
{
    typedef Scheduling_Criteria::Priority Criterion;
    static const bool smp = false;
    static const bool trace_idle = false;
    static const unsigned int QUANTUM = 10000; // us
};

template <> struct Traits<Single_Core_Alarm>: public Traits<void>
{
    static const bool idle_waiting = true;
    static const bool enabled = true;
};

template <> struct Traits<SMP_Alarm>: public Traits<void>
{
    static const bool idle_waiting = true;
    static const bool enabled = true;
};

template <> struct Traits<Synchronizer>: public Traits<void>
{
};


/*
 *
 * Network Abstractions
 *
 */

// Services
template <> struct Traits<Services>: public Traits<void>
{
    static const bool enabled = true;

    // Network services
    enum {
        ARP = 0,
        ADHOP
    };

    static const int SERVICE = ARP;
};

// List of Neighbors
template <> struct Traits<Neighborhood>: public Traits<void>
{
    static const bool enabled = true;

    static const unsigned int MAX_NEIGHBORS = 5;
    static const unsigned int EXPIRE = 10;
};

// Network Protocols
template <> struct Traits<Network>: public Traits<Services>
{
    static const unsigned long ADDRESS   = 0x0a00010f;   // 10.0.1.15
    static const unsigned long NETMASK   = 0xffffff00;   // 255.255.255.0
    static const unsigned long BROADCAST = 0x0a0001ff;   // 10.0.1.255

    static const unsigned int   TRIES = 3;
    static const unsigned int   TIMEOUT = 1000000; // 1s

    // Network Protocols
    enum {
        ELP,
        ROUTER
    };

    static const int NETWORK_PROTOCOL = ELP;
};

template <> struct Traits<IP>: public Traits<Services>
{
    static const unsigned long ADDRESS   = 0x0a00020f;   // 10.0.2.15
    static const unsigned long NETMASK   = 0xffffff00;   // 255.255.255.0
    static const unsigned long BROADCAST = 0x0a0002ff;   // 10.0.2.255

    static const bool forwarding    = false;
    static const bool fragmentation = false;
    static const bool spawn_thread  = true;

    // Network configuration method
    enum {
        STATIC,
        LINK_LOCAL,
        DHCP
    };

    static const short         CONFIG   = STATIC;
    static const unsigned int  OPT_SIZE = 0; // options size in 32-bit words
    static const unsigned char DEF_TTL  = 0x40; // time-to-live
    static const unsigned int  MAX_FRAGMENTS = 1;
};

template <> struct Traits<ICMP> : public Traits<IP>
{
    static const bool echo_reply = true;
};

// Transport Protocols
template <> struct Traits<UDP> : public Traits<IP>
{
    static const bool checksum = false;
};

template <> struct Traits<TCP> : public Traits<IP>
{
    static const bool checksum = true;
};

__END_SYS

#ifdef __ARCH_TRAITS_H
#include __ARCH_TRAITS_H
#endif

#ifdef __MACH_TRAITS_H
#include __MACH_TRAITS_H
#endif

#endif
