#ifndef __sip_manager_h
#define __sip_manager_h

#include <system/config.h>

#ifdef __NIC_H

#include <sip_defs.h>
#include <sip_message.h>
#include <mutex.h>
#include <udp.h>
#include <utility/list.h>

__BEGIN_SYS

class SIP_User_Agent;
typedef int (*USER_CALLBACK)(SIP_Event_Callback event, SIP_User_Agent *ua, const char *remote);

class SIP_Manager
{
private:
    SIP_Manager() : _channel(UDP::Address(IP::instance()->address(), SIP_PORT), UDP::Address(Traits<IP>::BROADCAST, SIP_PORT)),
        _host_ip(0), _host_port(0), _thread_receive(0) { _snd_buffer[0] = 0; _rcv_buffer[0] = 0; }

public:
    ~SIP_Manager();

    static SIP_Manager *get_instance();
    static void delete_instance();
    bool init();

    static USER_CALLBACK _callback;
    static void register_user_handler(USER_CALLBACK callback) { SIP_Manager::_callback = callback; }

    SIP_User_Agent *create_user_agent(const char *uri);
    SIP_User_Agent *get_user_agent(const char *uri);

    const char *get_host_ip() { return _host_ip; }
    unsigned short get_host_port() { return _host_port; }

    int send_message(SIP_Message *msg);
    static int receive_thread();

    static void random(char *buffer);

private:
    Simple_List<SIP_User_Agent> _ua;

    static bool _terminated;
    static SIP_Manager *_instance;

    char _snd_buffer[MAX_MSG_SIZE + 1];
    char _rcv_buffer[MAX_MSG_SIZE + 1];
    UDP::Channel _channel;

    char *_host_ip;
    unsigned short _host_port;

    Thread *_thread_receive;
};

__END_SYS

#endif

#endif
