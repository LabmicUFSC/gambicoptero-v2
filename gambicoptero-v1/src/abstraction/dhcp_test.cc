#include <dhcp.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <alarm.h>

__USING_SYS

OStream cout;

int main()
{
    IP ip(0);

    ip.set_address(IP::NULL);
    ip.set_gateway(IP::NULL);
    ip.set_netmask(IP::NULL);

    DHCP::Client dhcpc;
    
    dhcpc.configure();
    Alarm::delay(5000000);

    cout << "IP Addr: " << dhcpc.address() << endl;
    cout << "Netmask: " << dhcpc.netmask() << endl;
    cout << "Gateway: " << dhcpc.gateway() << endl;
    cout << "DNS:     " << dhcpc.nameserver() << endl;
    
    if (dhcpc.address() == IP::NULL)
        cout << "@result = failed\n";
    else
        cout << "@result = passed\n";
}
