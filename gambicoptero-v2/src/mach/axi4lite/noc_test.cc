
#include <utility/ostream.h>
#include <ic.h>
#include <noc.h>
#include <semaphore.h>

__USING_SYS

OStream cout;
NOC noc;
Semaphore rx_sem(0);

enum {
    ECHO_P0_LOCAL_ADDR = 0x6,
    ECHO_P1_LOCAL_ADDR = 0x4
};


void print_info(){

    NOC::Info const& info = noc.info();

    cout << "NoC info: "
         << info.local_addr << ", "
         << info.router_x_addr << ", "
         << info.router_y_addr << ", "
         << info.net_x_size << ", "
         << info.net_y_size << ", "
         << info.data_width << "\n";
}

void send_pkt(unsigned int dst_addr, unsigned int data){
    cout << "TX: Sending pkt: DST_L=" << dst_addr << " DATA=" << data << "\n";
    NOC::Address addr;
    addr.router_x = 0;
    addr.router_y = 0;
    addr.local = dst_addr;
    noc.send(&addr, &data);
    cout << "TX: Pkt sent\n";
    Thread::yield();
}

const unsigned int N_PKTS = 16;

int receive_pkt(){

    NOC::Address addr;
    unsigned int data;

    for (unsigned int var = 0; var < N_PKTS; ++var){
        cout << "RX: Waiting pkt... \n";
        rx_sem.p();
        noc.receive(&addr, &data);
        cout << "RX: Pkt received:\n";
        cout << "RX:     Header: "
                << "DST_X=" << addr.router_x << ", "
                << "DST_Y="<< addr.router_y << ", "
                << "DST_L="<< addr.local << "\n";
        cout << "RX:     Data: " << data << "\n";
    }

    return 0;
}

void int_handler(unsigned int interrupt){
    //receive_pkt();
    rx_sem.v();
}

int main() {

    cout << "RTSNoC test\n\n";

    cout << "Setup interrupt and rx thread\n";
    noc.receive_int(&int_handler);

    Thread *rx_thead = new Thread(&receive_pkt);

    print_info();
    cout << "\n";

    for (unsigned int i = 0; i < N_PKTS/2; ++i) {
        send_pkt(ECHO_P0_LOCAL_ADDR, i);
        send_pkt(ECHO_P1_LOCAL_ADDR, ~i);
    }

    cout << "Waiting last packets\n";
    rx_thead->join();

    cout << "\nThe end!\n";

    *((volatile unsigned int*)0xFFFFFFFC) = 0;

    return 0;
}
