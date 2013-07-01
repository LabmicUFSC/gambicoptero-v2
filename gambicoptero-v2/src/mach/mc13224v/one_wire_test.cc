// EPOS MC13224V_One_Wire Test Program

#include <one_wire.h>
#include <utility/ostream.h>

__USING_SYS

OStream cout;

int loopback() {

    static const int ONE_WIRE_PIN = 12;

    cout << "I'm a dummy One-Wire loopback program running on pin " << ONE_WIRE_PIN << ".\n";
    cout << "I'll return every byte different from 0x00\n\n";

    One_Wire wire(ONE_WIRE_PIN);

    unsigned char c = 1;
    while(true) {
        if((c = wire.get()) != 0x00)
            wire.put(c);
    }

    cout << "I'm done!\n";

    return 0;

}

int uart_relay() {

    static const int ONE_WIRE_PIN = 12;

    One_Wire wire(ONE_WIRE_PIN);
    UART uart;

    cout << "I'm a bidirectional OneWire-UART relay program running on pin " << ONE_WIRE_PIN << ".\n\n";

    unsigned char c1, c2;
    while(true) {
        cout << "Type sth in...";
        c1 = uart.get();
        wire.put(c1);
        while((c2 = wire.get()) != c1);
        cout << "Received back: " << c2 << ".\n\n";
    }

    cout << "I'm done!\n";

    return 0;

}

int main(void) {

//    return loopback();
    return uart_relay();
}
