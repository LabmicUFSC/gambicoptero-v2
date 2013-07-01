#include <system/config.h>

#ifdef __NIC_H

#include <dhcp.h>
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

    return 0;
}

#else

int main() { return 0; }

#endif
