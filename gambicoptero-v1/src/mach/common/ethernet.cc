// EPOS Ethernet Mediator Common Package Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <ethernet.h>

__BEGIN_SYS

const Ethernet::Address Ethernet::BROADCAST(0xff, 0xff, 0xff,
					    0xff, 0xff, 0xff);
Ethernet::Observed Ethernet::_observed;

__END_SYS

