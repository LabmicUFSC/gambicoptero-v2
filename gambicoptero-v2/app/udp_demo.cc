/*
 * UDP Sample Application
 * Author: Rodrigo V. Raimundo <rodrigovr@lisha.ufsc.br>
 *
 * Compile this application with a different IP for each node.
 *
 */

#include <alarm.h>
#include <udp.h>

#include <utility/ostream.h>

__USING_SYS

OStream cout;

class MyUDPSocket : public UDP::Socket {
public:
    MyUDPSocket(UDP::Address addr) : 
        UDP::Socket(addr, UDP::Address(~0,555)) 
    { 
        cout << "Created socket\n";
    }

    void bcast() {
        cout << "Broadcasting Hello world!\n";
        const char * msg = "Hello world!";
        remote(UDP::Address(IP::instance()->broadcast(), 555));
        send(msg, 12);
    }

protected:
    void received(const UDP::Address& from, const char* msg, unsigned int len) {
        cout << "Received " << msg << " from " << from << endl;
    }
};

int main() {
    IP * ip = IP::instance();
    cout << "UDP Demo running on: " << ip->address() << endl;

    UDP::Address addr(ip->address(), 555);
    MyUDPSocket socket(addr);

    while (1) {
        Alarm::delay(3000000);
        socket.bcast();
    }
}
