// EPOS Ethernet Mediator Common Package Implementation

#include <ethernet.h>

__BEGIN_SYS

const Ethernet::Address Ethernet::BROADCAST(0xff, 0xff, 0xff,
					    0xff, 0xff, 0xff);
Ethernet::Observed Ethernet::_observed;

__END_SYS
