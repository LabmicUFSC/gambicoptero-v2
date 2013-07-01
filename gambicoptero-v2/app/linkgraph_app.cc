#include <machine.h>
#include <mutex.h>
#include <neighborhood.h>
#include <network.h>
#include <utility/observer.h>

#define SINK true
#define SENSOR false

__USING_SYS


class LinkApp: Conditional_Observer
{
public:
    LinkApp(bool is_sink, const Network::Address & sink_address, const Network::Address & my_address)
        : _sink_address(sink_address), _my_address(my_address)
    {
        _id = Traits<CMAC<Radio_Wrapper> >::ADDRESS;
        net = new Network();
        net->protocol(PROT);
        net->attach(this,PROT);
        net->address(_my_address);
        if (is_sink) {
            led_green();
            sink();
        } else {
            led_red();
            sensor();
        }
    }

    ~LinkApp()
    {
        delete net;
    }

    enum {
        PROT = 21
    };

private:
    void sink()
    {
        while (true) {
            Alarm::delay(5000000);

            short msg[50];

            unsigned int size =
                Neighborhood::get_instance()->neighborhood(msg, 50);

            if (size == 0)
                continue;

            mut_display.lock();
            cout << "!" << (int) _id << "|" << (int) size << "|";
            for (int i = 0; i < (int) size; i++) {
                cout << (int) msg[i] << "|";
            }
            cout << "\n";
            mut_display.unlock();
        }
    }

    void update(Conditionally_Observed * o, int p)
    {
        Network::Address from;

        int size = net->receive(&from, msg.msg_c, sizeof(msg)) / 2;

        if (size < 2)
            return;

        mut_display.lock();
        cout << "!" << (int) msg.msg_s[0] << "|" << (int) (size - 1) << "|";
        for (int i = 1; i < size; i++) {
            cout << (int) msg.msg_s[i] << "|";
        }
        cout << "\n";
        mut_display.unlock();
    }

    void sensor()
    {
        msg.msg_s[0] = _id;

        for (unsigned int i = 1; i < 50; i++) {
            msg.msg_s[i] = 0;
        }

        short * p_msg_s = &(msg.msg_s[1]);

        while (true) {
            unsigned int size =
                Neighborhood::get_instance()->neighborhood(p_msg_s, 48);

            net->send(_sink_address, msg.msg_c, 2 + (size * 2));

            cout << "sent: ";

            mut_display.lock();
            cout << (int) _id << "|" << (int) size << "|";
            for (int i = 1; i < (int) (size + 1); i++) {
                cout << (int) msg.msg_s[i] << "|";
            }
            cout << "\n";
            mut_display.unlock();

            Alarm::delay(5000000);
        }
    }

    void led_red() {
        unsigned int *GPIO_BASE = (unsigned int*)0x80000000;
        *GPIO_BASE = 1 << 23;
    }

    void led_green() {
        unsigned int *GPIO_BASE = (unsigned int*)0x80000000;
        *GPIO_BASE = 1 << 24;
    }

private:
    union Msg {
        char msg_c[100];
        short msg_s[50];
    };

    Msg msg;
    unsigned short _id;

    Network::Address _sink_address;
    Network::Address _my_address;

    OStream cout;
    Mutex mut_display;
    Network * net;

};

int main() {
    Network::Address sink_address(10,0,1,0);
    Network::Address my_address(10,0,1,1);

    LinkApp app(SINK, sink_address, sink_address);
    //LinkApp app(SENSOR, sink_address, my_address);

    return 0;
}

