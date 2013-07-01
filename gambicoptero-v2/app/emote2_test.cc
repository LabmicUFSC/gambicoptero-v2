#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <mach/mc13224v/emote2_startup.h>

__USING_SYS

const unsigned char SINK_ID  = 0x01;
const unsigned int DATA_SIZE = 32;

union MSG_STRUCT {
    char c[DATA_SIZE];
    unsigned char uc[DATA_SIZE];
    short s[DATA_SIZE/2];
    unsigned short us[DATA_SIZE/2];
    int i[DATA_SIZE/4];
    unsigned int ui[DATA_SIZE/4];
} msg;


NIC * nic;
OStream cout;

void sensor(unsigned char id) {

    cout << "Sensor id = " << (int) id << "\n";

    EMote2_Startup startup;
    Battery batt;

    startup.set_green();

    for (unsigned int i = 0; i < DATA_SIZE; i++) {
        msg.uc[i] = i;
    }

    msg.uc[0] = id;

    unsigned char c = 0;

    while (!startup.get_button1() && !startup.get_button2()) {
        msg.uc[1] = c++;
        msg.us[1] = batt.charge();
        msg.ui[1] = startup.temperature();
        msg.ui[2] = startup.x_acceleration();
        msg.ui[3] = startup.y_acceleration();
        msg.ui[4] = startup.z_acceleration();


        int r;
        while ((r = nic->send(NIC::BROADCAST, (NIC::Protocol) 1, &(msg.uc), DATA_SIZE)) != 11)
            cout << "failed " << r << "\n";

        cout << "tx done\n";

        Alarm::delay(1000000);
    }

    startup.set_green();
    Alarm::delay(1000000);
    startup.clear_green();

    startup.set_red();
    Alarm::delay(1000000);
    startup.clear_red();

    startup.set_rgb(255,0,0);
    Alarm::delay(1000000);
    startup.set_rgb(0,255,0);
    Alarm::delay(1000000);
    startup.set_rgb(0,0,255);
    Alarm::delay(1000000);
    startup.clear_rgb();

    cout << "Press Button 1\n";
    while(!startup.get_button1());
    cout << "Press Button 2\n";
    while(!startup.get_button2());

    startup.set_green();
    startup.set_red();
    startup.set_rgb(255,255,255);

    while(true);
}

void sink() {
    NIC::Protocol prot;
    NIC::Address src;

    EMote2_Startup startup;
    startup.set_red();

    cout << "Sink\n";

    while (true) {
        while(!(nic->receive(&src, &prot, &(msg.uc), DATA_SIZE) > 0))
            cout << "failed\n";

        cout << "\n##########\n";
        cout << "Protocol:"     << (int) prot   << "\n";
        cout << "Sender id: "   << (int) msg.uc[0] << "\n";
        cout << "Msg number: "  << (int) msg.uc[1] << "\n";
        cout << "Battery: "     << (int) msg.us[1] << " \%\n";
        cout << "Temperature: " << (int) msg.ui[1] << " C\n";
        cout << "Accelerometer X: " << (int) msg.ui[2] << "\n";
        cout << "Accelerometer Y: " << (int) msg.ui[3] << "\n";
        cout << "Accelerometer Z: " << (int) msg.ui[4] << "\n";
    }
}

int main() {
    nic = new NIC();

//    sink();
    sensor(1);
}

