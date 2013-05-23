#ifndef __network_service_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __network_service_h

#include <utility/debug.h>
#include <utility/buffer.h>

__BEGIN_SYS

template <class Link_Layer, class Network_Layer>
class Network_Service_Common
{
protected:
    typedef typename Link_Layer::Address    Link_Address;
    typedef typename Network_Layer::Address Network_Address;

    typedef typename Link_Layer::Protocol   Protocol;

public:
    Network_Service_Common(Link_Layer* nic, Network_Layer* network):
        _nic(nic), _network(network)
    {
        _my_nic_address = nic->address();
        _my_network_address = network->address();
    }
    
    virtual ~Network_Service_Common() {}

    virtual void update(const Network_Address& la,
            const Link_Address& pa) = 0;

    virtual Link_Address resolve(const Network_Address& la,
            SegmentedBuffer * pdu) = 0;

    virtual void received(const Link_Address& pa, Protocol proto,
            const char* data, int size) = 0;

protected:
    Link_Layer* _nic;
    Network_Layer* _network;
    Link_Address _my_nic_address;
    Network_Address _my_network_address;
};

__END_SYS

#endif

