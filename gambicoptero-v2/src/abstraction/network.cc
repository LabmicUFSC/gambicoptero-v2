// EPOS Network Abstraction Implementation

#include <network.h>

#ifdef __network_h

__BEGIN_SYS

Network::Network():
    Network::Base(0)
{ }

Network::Network(unsigned int id):
    Network::Base(id)
{ }

Network::~Network()
{ }

__END_SYS

#endif /* __network_h */
