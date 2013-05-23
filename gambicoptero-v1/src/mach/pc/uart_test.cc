// EPOS PC UART Mediator Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <uart.h>

__USING_SYS

int main()
{
    OStream cout;

    cout << "PC_UART test\n\n";

    PC_UART uart(115200, 8, 0, 1);

    cout << "Loopback transmission test (conf = 115200 8N1):";
    uart.loopback(true);

    for(int i = 0; i < 256; i++) {
	uart.put(i);
	int c = uart.get();
	if(c != i)
	    cout << " failed (" << c << ", should be " << i << ")!\n";
    }
    cout << " passed!\n";

    cout << "Link transmission test (conf = 9200 8N1):";
    uart.config(9600, 8, 0, 1);
    uart.loopback(false);

    for(int i = 0; i < 256; i++) {
	uart.put(i);
        for(int j = 0; j < 0xffffff; j++);
	int c = uart.get();
	if(c != i)
	    cout << " failed (" << c << ", should be " << i << ")!\n";
    }
    cout << " passed!\n";
    
    return 0;
}
