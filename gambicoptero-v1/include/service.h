#ifndef __service_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __service_h

#include <system/meta.h>
#include <adhop.h>
#include <arp.h>

__BEGIN_SYS

template <int SERVICE>
class Service
{
public:
    template <typename Link_Layer, typename Network_Layer>
    class Network_Service:
        public SWITCH<SERVICE,
                     CASE<Traits<Services>::ADHOP,
                         ADHOP<Link_Layer,Network_Layer>,
                     CASE<Traits<Services>::BCast,
                         BCast<Link_Layer,Network_Layer>,
                     CASE<DEFAULT,
                         ARP<Link_Layer,Network_Layer>
                     > > >
               >::Result
    {
    public:
        typedef typename
            SWITCH<SERVICE,
                  CASE<Traits<Services>::ADHOP,
                      ADHOP<Link_Layer,Network_Layer>,
                  CASE<Traits<Services>::BCast,
                      BCast<Link_Layer,Network_Layer>,
                  CASE<DEFAULT,
                      ARP<Link_Layer,Network_Layer>
                  > > >
            >::Result Base;

        Network_Service(Link_Layer* nic, Network_Layer* network):
            Base(nic, network)
        { }
    };
};

__END_SYS

#endif

