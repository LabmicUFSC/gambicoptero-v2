#include <tcp.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <utility/string.h>

__USING_SYS

OStream cout;

void web_client() {
    
    TCP::Channel channel;

    channel.bind(55000 + Pseudo_Random::random() % 10000);
    
    if (!channel.connect(TCP::Address("74.125.234.84:80")))
    {
        cout << "Connection failed\n!";
        return;
    }
    
    
    channel.send("GET / HTTP/1.1\n\rHost: www.google.com\n\r\n\r",40);  
    
    char * data = new char[4096];
    
    int size = channel.receive(data,4096);
    
    cout << "Received "<<size<<" bytes: " << endl;
    int p;
    for(p=0;p<size;p++)
        cout << *data++;

    channel.close();
};

int main()
{
    IP * ip = IP::instance();

    ip->set_address(IP::Address(10,0,2,15));
    ip->set_gateway(IP::Address(10,0,2,2));
    ip->set_netmask(IP::Address(255,255,255,0));

    web_client();

    delete ip; // kill IP thread
}
