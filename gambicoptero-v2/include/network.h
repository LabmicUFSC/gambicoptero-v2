// EPOS Network Abstraction Declarations

#ifndef __network_h
#define __network_h

#include <elp.h>
#include <router.h>

__BEGIN_SYS

class Network:
    public SWITCH<Traits<Network>::NETWORK_PROTOCOL,
                CASE<Traits<Network>::ROUTER,
                    Router,
                CASE<DEFAULT,
                    ELP
                > >
           >::Result
{
public:
    typedef
        SWITCH<Traits<Network>::NETWORK_PROTOCOL,
            CASE<Traits<Network>::ROUTER,
                Router,
            CASE<DEFAULT,
                ELP
            > >
        >::Result Base;

    Network();
    Network(unsigned int unit);
    ~Network();

    using Base::send;
    using Base::receive;
    using Base::update;

    const Address & address() { return Base::address(); }

    void address(const Address & address) { Base::address(address); }

};

__END_SYS

#endif /* __network_h */

