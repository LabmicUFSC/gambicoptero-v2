#include <nic.h>
#include <uart.h>
#include <alarm.h>

__USING_SYS

NIC * nic;
UART uart;
OStream cout;


unsigned int data0_mask;
char ac_params[3]; // fan, temp, mode

typedef Machine::IO IO;

void set_pin(int pin = 10) {
    data0_mask &= ~(1 << pin);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void clear_pin(int pin = 10) {
    data0_mask |= (1 << pin);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void init_io() {
    CPU::out32(IO::GPIO_PAD_DIR0, 0u | (1 << 10));
    set_pin();
}

void delay10u(volatile unsigned int count) {
	volatile unsigned int inner_count;
	for (volatile unsigned int i = 0; i < count; i++) {
		inner_count = 18;
		while (inner_count--) {};
	}
}

void put_L() {
    clear_pin();
    delay10u(440+29);
    set_pin();
    delay10u(440+29);
}

void put_S() {
    clear_pin();
    delay10u(65+4);
    set_pin();
    delay10u(520+35);
}

void put_0() {
    clear_pin();
    delay10u(60+4);
    set_pin();
    delay10u(60+4);
}

void put_1() {
    clear_pin();
	delay10u(60+4);
    set_pin();
    delay10u(155+10);
}

void put_either(char either) {
    if(either) put_1();
    else put_0();
}

void put_c(char c) {
    for(int i = 7; i >= 0; i--) {
        put_either(c & (1 << i));
    }
}

void send_command(char A, char B, char C) {
    put_L();
    put_c(A);
    put_c(~A);
    put_c(B);
    put_c(~B);
    put_c(C);
    put_c(~C);
    put_S();
    put_L();
    put_c(A);
    put_c(~A);
    put_c(B);
    put_c(~B);
    put_c(C);
    put_c(~C);
    put_S();
}

void turn_off() {
    send_command(0xb2, 0x7b, 0xe0);
}

void set_temp() {
    char B = 0x1f | ((ac_params[0] & 0x07) << 5);
    char C = 0x00 | ((ac_params[1] & 0x0f) << 4) | ((ac_params[2] & 0x03) << 2);
    send_command(0xb2, B, C);
}

enum FanSpeed {
    Settled = 0x00,
    High    = 0x01,
    Middle  = 0x02,
    Low     = 0x04,
    Auto    = 0x05
};

enum Mode {
    Cooling     = 0x00,
    Dehumifying = 0x1,
    Fan         = 0x1,
    ModeAuto    = 0x2,
    Heating     = 0x3,
};

enum Temperature {
    Degrees17 = 0x0,
    Degrees18 = 0x1,
    Degrees19 = 0x3,
    Degrees20 = 0x2,
    Degrees21 = 0x6,
    Degrees22 = 0x7,
    Degrees23 = 0x5,
    Degrees24 = 0x4,
    Degrees25 = 0xc,
    Degrees26 = 0xd,
    Degrees27 = 0x9,
    Degrees28 = 0x8,
    Degrees29 = 0xa,
    Degrees30 = 0xb,
    FanMode   = 0xe
};

char _temperature[15] = {
    Degrees17,
    Degrees18,
    Degrees19,
    Degrees20,
    Degrees21,
    Degrees22,
    Degrees23,
    Degrees24,
    Degrees25,
    Degrees26,
    Degrees27,
    Degrees28,
    Degrees29,
    Degrees30,
    FanMode
};

void restore_params() {
    if(!Flash::read(Flash::DATA_BASE, &(ac_params[0]), 1))
        ac_params[0] = Middle;
    if(!Flash::read(Flash::DATA_BASE+1, &(ac_params[1]), 1))
        ac_params[1] = Degrees22;
    if(!Flash::read(Flash::DATA_BASE+2, &(ac_params[2]), 1))
        ac_params[2] = Cooling;
}

void store_params() {
    Flash::write(Flash::DATA_BASE, &(ac_params[0]), 3);
}


int gateway() {
    char msg[3];
    int r;

    CPU::out32(IO::GPIO_PAD_DIR0, 0u | (1 << 10));

    while (true) {
        msg[0] = uart.get(); // dest
        msg[1] = uart.get(); // slot
        msg[2] = uart.get(); // value

        kout << "sending\n";
        kout << "dest = " << msg[0] << ", slot = " << msg[1] << ", value = " << msg[2] << "\n";
        while ((r = nic->send(NIC::BROADCAST, (NIC::Protocol) 1, &msg, sizeof(msg))) != 11)
            cout << "failed " << r << "\n";

        cout << "tx done\n";
    }

    return 0;
}

void ac_onoff(char value) {
	kout << "ac_onoff\n";
	if(value) set_temp();
	else turn_off();
}

void ac_temperature(char value) {
	kout << "ac_temperature\n";
	ac_params[1] = _temperature[value];
//	store_params();
	set_temp();
}

void process_message(char msg[3]) {
	switch(msg[1]) {
	case 0x0a: ac_onoff(msg[2]); break;
	case 0x0b: ac_temperature(msg[2]); break;
	}
}

int sensor() {
    init_io();
    data0_mask = 0;
//    restore_params();

    NIC::Protocol prot;
    NIC::Address src;

    ac_params[0] = Middle;
    ac_params[1] = Degrees22;
    ac_params[2] = Cooling;

    char msg[3];

    while (true) {
	kout << "waiting commmand...\n";
        while (!(nic->receive(&src, &prot, &msg, sizeof(msg)) > 0));

        kout << "dest = " << msg[0] << ", slot = " << msg[1] << ", value = " << msg[2] << "\n";

	CPU::int_disable();
	process_message(msg);
	CPU::int_enable();
	
    }

    return 0;
}

int main() {
    nic = new NIC();

    //gateway();
    sensor();

    return 0;
}

